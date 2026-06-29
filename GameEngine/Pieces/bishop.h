#pragma once
#include "piece.h"
#include "../protocols/MoveRules.h"

class Bishop : public Piece {
public:
    Bishop(Color c) : Piece(c, PieceType::Bishop) {}
    bool isValidMove(int startX, int startY, int endX, int endY, Piece* board[8][8]) override {
        if (board[endX][endY] != nullptr && board[endX][endY]->getColor() == color) return false;
        return MoveRules::isDiagonalPathClear(startX, startY, endX, endY, board);
    }
};