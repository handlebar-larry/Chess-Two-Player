#pragma once
#include "Piece.cpp"
#include "../MoveRules.cpp"

class Rook : public Piece {
public:
    Rook(Color c) : Piece(c, PieceType::Rook) {}
    bool isValidMove(int startX, int startY, int endX, int endY, Piece* board[8][8]) override {
        if (board[endX][endY] != nullptr && board[endX][endY]->getColor() == color) return false;
        return MoveRules::isStraightPathClear(startX, startY, endX, endY, board);
    }
};