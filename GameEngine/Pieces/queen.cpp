#pragma once
#include "Piece.cpp"
#include "../MoveRules.cpp"

class Queen : public Piece {
public:
    Queen(Color c) : Piece(c, PieceType::Queen) {}
    bool isValidMove(int startX, int startY, int endX, int endY, Piece* board[8][8]) override {
        if (board[endX][endY] != nullptr && board[endX][endY]->getColor() == color) return false;
        return MoveRules::isStraightPathClear(startX, startY, endX, endY, board) || 
               MoveRules::isDiagonalPathClear(startX, startY, endX, endY, board);
    }
};