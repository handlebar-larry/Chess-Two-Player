#pragma once
#include "piece.h"
#include <cmath>

using namespace std;

class pawn : public Piece {
public:
    pawn(Color c) : Piece(c, PieceType::Pawn) {}

    bool isValidMove(int startX, int startY, int endX, int endY, Piece* board[8][8]) override {
        
        int direction = (color == Color::White) ? -1 : 1;
        int startRow = (color == Color::White) ? 6 : 1;

        if (startY == endY && endX == startX + direction && board[endX][endY] == nullptr) return true;
        
        if (startY == endY && startX == startRow && endX == startX + (2 * direction)) {
            if (board[startX + direction][startY] == nullptr && board[endX][endY] == nullptr) return true;
        }
        
        if (abs(startY - endY) == 1 && endX == startX + direction) {
            if (board[endX][endY] != nullptr && board[endX][endY]->getColor() != color) return true;
        }
        return false;
    }
};
