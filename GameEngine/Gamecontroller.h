#pragma once
#include "board/Board.h"
#include "protocols/GameState.h"
#include "protocols/SpecialOperations.h"
#include "Sidepanel/CaptureTracker.h"
#include <cmath>
#include <iostream>
#include <utility>

using namespace std;

class GameController {
private:
    Board board;              
    Color currentTurn;        
    GameState stateManager;   
    bool gameRunning;         
    pair<int, int> enPassantTarget; 
    CaptureTracker tracker;
    int halfMoveClock = 0;

    bool isInputSelectionValid(int sx, int sy) {
        Piece* p = board.getPiece(sx, sy);
        if (!p || p->getColor() != currentTurn) {
            cout << "XD Selection / Turn Error!\n";
            return false;
        }
        return true;
    }

    void generateRawGridCache(Piece* rawGrid[8][8]) {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) rawGrid[r][c] = board.getPiece(r, c);
        }
    }

    void switchTurn() {
        currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
        if (stateManager.isCheck(currentTurn, board)) {
            if (stateManager.isCheckmate(currentTurn, board)) {
                cout << "\n XD CHECKMATE! Game Over.\n";
                gameRunning = false;
            } else {
                cout << "XD Warning: King is in CHECK!\n";
            }
        } else if (stateManager.isStalemate(currentTurn, board)) {
            cout << "\n XD STALEMATE! Draw.\n";
            gameRunning = false;
        }
    }

public:
    GameController() : currentTurn(Color::White), gameRunning(true), enPassantTarget({-1, -1}) {}

    Board& getBoard() { return board; }
    bool isGameActive() const { return gameRunning; }

    CaptureTracker getCaptureTracker(){
        return tracker;
    }

    bool handleMove(int sx, int sy, int ex, int ey) {
        if (!gameRunning) return false;
        if (!isInputSelectionValid(sx, sy)) return false;

        Piece* p = board.getPiece(sx, sy);
        bool isCapture = (board.getPiece(ex, ey) != nullptr);
        bool isPawn = (p && p->getType() == PieceType::Pawn);
        Color movingColor = p->getColor();

        if (SpecialOperations::tryCastling(sx, sy, ex, ey, currentTurn, board, stateManager)) { 
            switchTurn(); 
            return true; 
        }
        if (SpecialOperations::tryEnPassant(sx, sy, ex, ey, enPassantTarget, board)) { 
            enPassantTarget = {-1, -1}; 
            switchTurn(); 
            return true; 
        }

        Piece* rawGrid[8][8];
        generateRawGridCache(rawGrid);

        Piece* movingPiece = board.getPiece(sx, sy);
        if (!movingPiece->isValidMove(sx, sy, ex, ey, rawGrid)) {
            cout << "XD Geometric path validation failed.\n";
            return false;
        }

        if (SpecialOperations::violatesSelfCheckGuard(sx, sy, ex, ey, currentTurn, board, stateManager)) { 
            cout << "XD Illegal: Exposes King!\n"; 
            return false; 
        }

        // --- STANDARD RESOLUTION ---
        Piece* targetPiece = board.getPiece(ex, ey);
        if (targetPiece) {
            PieceType capturedType = targetPiece->getType();
            delete targetPiece;
            tracker.logCapture(capturedType, movingColor);
        }

        board.movePieceOnMatrix(sx, sy, ex, ey);
        bool promoted = SpecialOperations::handlePromotion(ex, ey, board);

        enPassantTarget = {-1, -1};
        if (!promoted && board.getPiece(ex, ey)->getType() == PieceType::Pawn && abs(sx - ex) == 2) {
            enPassantTarget = {(sx + ex) / 2, sy};
        }

        if (isPawn || isCapture) {
            halfMoveClock = 0; // Reset counter completely
        } else {
            halfMoveClock++; // Increment for vanilla piece steps
        }

        if (halfMoveClock >= 100) { // 100 half-moves = 50 full turns
            cout << "🏳️ Game Draw via 50-Move Rule Limit Triggered!\n";
            return false;
        }

        cout << "[noice move] Move successful!\n";
        switchTurn();
        return true;
    }
    
};
