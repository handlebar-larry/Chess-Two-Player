#pragma once
#include "GameState.h"
#include "../../comon.h"
#include "../board/Board.h"
#include "../Pieces/Queen.h"
#include "../Pieces/Rook.h"
#include "../Pieces/Bishop.h"
#include "../Pieces/Knight.h"

using namespace std;

/**
 * @class SpecialOperations
 * @brief Isolated handler for multi-step special rules (Castling, En Passant, Promotion, Pins).
 * This completely declutters the Main Controller.
 */
class SpecialOperations {
public:
    static bool tryCastling(int startX, int startY, int endX, int endY, Color currentTurn, Board& board, const GameState& stateManager) {
        Piece* movingPiece = board.getPiece(startX, startY);
        if (movingPiece->getType() == PieceType::King && abs(startY - endY) == 2) {
            if (movingPiece->getHasMoved() || stateManager.isCheck(currentTurn, board)) return false;

            for(int col = min(startY, endY); col <= max(startY, endY); ++col) {
                if (col != startY && board.getPiece(startX, col) != nullptr) return false;
                if (violatesSelfCheckGuard(startX, startY, endX, col, currentTurn, board, stateManager)) return false;
            }

            int rookStartCol = (endY == 6) ? 7 : 0;
            int rookEndCol   = (endY == 6) ? 5 : 3;
            Piece* rook = board.getPiece(startX, rookStartCol);

            if (rook && !rook->getHasMoved() && board.getPiece(startX, rookEndCol) == nullptr) {
                board.movePieceOnMatrix(startX, startY, endX, endY);
                board.movePieceOnMatrix(startX, rookStartCol, startX, rookEndCol);
                cout << "-_- Castling executed!\n";
                return true;
            }
        }
        return false;
    }

    static bool tryEnPassant(int startX, int startY, int endX, int endY, pair<int,int>& epTarget, Board& board) {
        Piece* movingPiece = board.getPiece(startX, startY);
        if (movingPiece->getType() == PieceType::Pawn && endX == epTarget.first && endY == epTarget.second) {
            delete board.getPiece(startX, endY);
            board.setPiece(startX, endY, nullptr);
            board.movePieceOnMatrix(startX, startY, endX, endY);
            cout << "-_- En Passant capture executed!\n";
            return true;
        }
        return false;
    }

    static bool violatesSelfCheckGuard(int startX, int startY, int endX, int endY, Color currentTurn, Board& board, const GameState& stateManager) {
        Piece* movingPiece = board.getPiece(startX, startY);
        Piece* targetPiece = board.getPiece(endX, endY);

        board.setPiece(endX, endY, movingPiece);
        board.setPiece(startX, startY, nullptr);
        
        bool leavesKingInCheck = stateManager.isCheck(currentTurn, board);
        
        board.setPiece(startX, startY, movingPiece);
        board.setPiece(endX, endY, targetPiece);
        return leavesKingInCheck;
    }

    static bool handlePromotion(int endX, int endY, Board& board) {
        Piece* movedPiece = board.getPiece(endX, endY);
        if (movedPiece->getType() == PieceType::Pawn && (endX == 7 || endX == 0)) {
            cout << "\n -_- PAWN PROMOTION! Enter choice (Q, R, B, N): ";
            char choice; cin >> choice; cin.ignore();
            Color pColor = movedPiece->getColor();
            delete board.getPiece(endX, endY);

            if (choice == 'R' || choice == 'r') board.setPiece(endX, endY, new Rook(pColor));
            else if (choice == 'B' || choice == 'b') board.setPiece(endX, endY, new Bishop(pColor));
            else if (choice == 'N' || choice == 'n') board.setPiece(endX, endY, new Knight(pColor));
            else board.setPiece(endX, endY, new Queen(pColor));
            return true;
        }
        return false;
    }
};