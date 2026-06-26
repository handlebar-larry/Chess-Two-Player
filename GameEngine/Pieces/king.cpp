#pragma once
#include "Piece.cpp"
#include <cmath>

class King : public Piece {
public:
    King(Color c) : Piece(c, PieceType::King) {}
    bool isValidMove(int startX, int startY, int endX, int endY, Piece* board[8][8]) override {
        if (board[endX][endY] != nullptr && board[endX][endY]->getColor() == color) return false;
        return (std::abs(startX - endX) <= 1 && std::abs(startY - endY) <= 1);
    }
};