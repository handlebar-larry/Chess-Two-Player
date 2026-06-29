#pragma once
#include "piece.h"

class Knight : public Piece {
public:
    Knight(Color c) : Piece(c, PieceType::Knight) {}
    bool isValidMove(int startX, int startY, int endX, int endY, Piece* board[8][8]) override {

        if (board[endX][endY] != nullptr && board[endX][endY]->getColor() == color) return false;
        int dX = abs(startX - endX);
        int dY = abs(startY - endY);
        return (dX == 2 && dY == 1) || (dX == 1 && dY == 2);
    }
};