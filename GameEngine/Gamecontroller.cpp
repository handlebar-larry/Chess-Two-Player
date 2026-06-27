#pragma once
#include "board/Board.cpp"
#include "protocols/GameState.cpp"
#include "protocols/SpecialOperations.cpp"
#include <iostream>
#include <utility>
#include <cmath>

class GameController {
private:
    Board board;              
    Color currentTurn;        
    GameState stateManager;   
    bool gameRunning;         
    std::pair<int, int> enPassantTarget; 

    // Helper: Validates core inputs and ownership
    bool isInputSelectionValid(int sx, int sy) {
        Piece* p = board.getPiece(sx, sy);
        if (!p || p->getColor() != currentTurn) {
            std::cout << "❌ Selection / Turn Error!\n";
            return false;
        }
        return true;
    }

    // Helper: Syncs board representation for piece geometric math overrides
    void generateRawGridCache(Piece* rawGrid[8][8]) {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) rawGrid[r][c] = board.getPiece(r, c);
        }
    }

    void switchTurn() {
        currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
        if (stateManager.isCheck(currentTurn, board)) {
            if (stateManager.isCheckmate(currentTurn, board)) {
                std::cout << "\n💥 CHECKMATE! Game Over.\n";
                gameRunning = false;
            } else {
                std::cout << "⚠️ Warning: King is in CHECK!\n";
            }
        } else if (stateManager.isStalemate(currentTurn, board)) {
            std::cout << "\n🤝 STALEMATE! Draw.\n";
            gameRunning = false;
        }
    }

public:
    GameController() : currentTurn(Color::White), gameRunning(true), enPassantTarget({-1, -1}) {}

    Board& getBoard() { return board; }
    bool isGameActive() const { return gameRunning; }

    /**
     * 🔗 THE NEW INTERACTIVE FRONTEND LINK
     * This replaces handlePlayerPrompt(). Instead of pausing for terminal input, 
     * it processes the raw array coordinates passed natively from SFML mouse clicks.
     */
    bool handleMove(int sx, int sy, int ex, int ey) {
        if (!gameRunning) return false;
        if (!isInputSelectionValid(sx, sy)) return false;

        // 1. Check for special operations FIRST before strict path rules evaluate
        if (SpecialOperations::tryCastling(sx, sy, ex, ey, currentTurn, board, stateManager)) { 
            switchTurn(); 
            return true; 
        }
        if (SpecialOperations::tryEnPassant(sx, sy, ex, ey, enPassantTarget, board)) { 
            enPassantTarget = {-1, -1}; 
            switchTurn(); 
            return true; 
        }

        // 2. Standard geometric validation cache check follows
        Piece* rawGrid[8][8];
        generateRawGridCache(rawGrid);

        Piece* movingPiece = board.getPiece(sx, sy);
        if (!movingPiece->isValidMove(sx, sy, ex, ey, rawGrid)) {
            std::cout << "❌ Geometric path validation failed.\n";
            return false;
        }

        if (SpecialOperations::violatesSelfCheckGuard(sx, sy, ex, ey, currentTurn, board, stateManager)) { 
            std::cout << "❌ Illegal: Exposes King!\n"; 
            return false; 
        }

        // --- STANDARD RESOLUTION ---
        Piece* targetPiece = board.getPiece(ex, ey);
        if (targetPiece) delete targetPiece;

        board.movePieceOnMatrix(sx, sy, ex, ey);
        bool promoted = SpecialOperations::handlePromotion(ex, ey, board);

        // Update history tracking states
        enPassantTarget = {-1, -1};
        if (!promoted && board.getPiece(ex, ey)->getType() == PieceType::Pawn && std::abs(sx - ex) == 2) {
            enPassantTarget = {(sx + ex) / 2, sy};
        }

        std::cout << "✅ Move successful!\n";
        switchTurn();
        return true;
    }
};