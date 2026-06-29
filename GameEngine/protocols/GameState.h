#pragma once
#include "../../comon.h"
#include "../board/Board.h"

using namespace std;

/**
 * @class GameState
 * @brief Evaluates match condition heuristics (Check, Checkmate, Stalemate).
 */
class GameState {
private:
    void syncRawGrid(const Board& board, Piece* rawGrid[8][8]) const {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) rawGrid[r][c] = board.getPiece(r, c);
        }
    }

public:
    pair<int, int> findKing(Color kingColor, const Board& board) const {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Piece* piece = board.getPiece(i, j);
                if (piece && piece->getType() == PieceType::King && piece->getColor() == kingColor) {
                    return {i, j};
                }
            }
        }
        return {-1, -1}; 
    }

    bool isCheck(Color kingColor, const Board& board) const {
        pair<int, int> kingPos = findKing(kingColor, board);
        if (kingPos.first == -1) return false; 

        Piece* rawGrid[8][8];
        syncRawGrid(board, rawGrid);

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Piece* piece = board.getPiece(i, j);
                if (piece && piece->getColor() != kingColor) {
                    if (piece->isValidMove(i, j, kingPos.first, kingPos.second, rawGrid)) return true;
                }
            }
        }
        return false;
    }

    bool isSafe(Color kingColor, const Board& board) const {
        pair<int, int> kingPos = findKing(kingColor, board);
        if (kingPos.first == -1) return false;  

        Piece* rawGrid[8][8];
        syncRawGrid(board, rawGrid);

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Piece* piece = board.getPiece(i, j);
                if (piece && piece->getColor() != kingColor) {
                    if (piece->isValidMove(i, j, kingPos.first, kingPos.second, rawGrid)) return true;
                }
            }
        }
        return false;
    }

    bool simulationBreaksCheck(int startX, int startY, int endX, int endY, Color kingColor, Board& board) {
        Piece* movingPiece = board.getPiece(startX, startY);
        Piece* targetPiece = board.getPiece(endX, endY); 

        board.setPiece(endX, endY, movingPiece);
        board.setPiece(startX, startY, nullptr);

        bool safe = !isCheck(kingColor, board);

        board.setPiece(startX, startY, movingPiece);
        board.setPiece(endX, endY, targetPiece);
        return safe;
    }

    bool isCheckmate(Color kingColor, Board& board) {
        if (!isCheck(kingColor, board)) return false;

        Piece* rawGrid[8][8];
        syncRawGrid(board, rawGrid);

        for (int startX = 0; startX < 8; startX++) {
            for (int startY = 0; startY < 8; startY++) {
                Piece* piece = board.getPiece(startX, startY);
                if (piece && piece->getColor() == kingColor) {
                    for (int endX = 0; endX < 8; endX++) {
                        for (int endY = 0; endY < 8; endY++) {
                            if (piece->isValidMove(startX, startY, endX, endY, rawGrid)) {
                                if (simulationBreaksCheck(startX, startY, endX, endY, kingColor, board)) return false; 
                            }
                        }
                    }
                }
            }
        }
        return true; 
    }

    bool isStalemate(Color kingColor, Board& board) {
        if (isCheck(kingColor, board)) return false;

        Piece* rawGrid[8][8];
        syncRawGrid(board, rawGrid);

        for (int startX = 0; startX < 8; startX++) {
            for (int startY = 0; startY < 8; startY++) {
                Piece* piece = board.getPiece(startX, startY);
                if (piece && piece->getColor() == kingColor) {
                    for (int endX = 0; endX < 8; endX++) {
                        for (int endY = 0; endY < 8; endY++) {
                            if (piece->isValidMove(startX, startY, endX, endY, rawGrid)) {
                                if (simulationBreaksCheck(startX, startY, endX, endY, kingColor, board)) return false; 
                            }
                        }
                    }
                }
            }
        }
        return true; 
    }
};