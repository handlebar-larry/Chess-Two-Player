#pragma once
#include "Piece.cpp"
#include <cmath>

class pawn : public Piece {
public:
    pawn(Color c) : Piece(c, PieceType::Pawn) {}

    bool isValidMove(int startX, int startY, int endX, int endY, Piece* board[8][8]) override {
        // 🎯 THE DIRECTION FLIP FIX:
        // White moves UP the matrix lines (Row 6 -> Row 5, so -1)
        // Black moves DOWN the matrix lines (Row 1 -> Row 2, so +1)
        int direction = (color == Color::White) ? -1 : 1;
        int startRow = (color == Color::White) ? 6 : 1;

        // Forward 1 Step Space
        if (startY == endY && endX == startX + direction && board[endX][endY] == nullptr) return true;
        
        // Initial Double Step Space Jump
        if (startY == endY && startX == startRow && endX == startX + (2 * direction)) {
            if (board[startX + direction][startY] == nullptr && board[endX][endY] == nullptr) return true;
        }
        
        // Diagonal Captures
        if (std::abs(startY - endY) == 1 && endX == startX + direction) {
            if (board[endX][endY] != nullptr && board[endX][endY]->getColor() != color) return true;
        }
        return false;
    }
};