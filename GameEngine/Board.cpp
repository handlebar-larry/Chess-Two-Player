#pragma once
#include <bits/stdc++.h>
#include "Pieces/Piece.cpp"

/**
 * @class Board
 * @brief Encapsulates the 8x8 grid matrix storage layout.
 * Enforces raw memory security boundaries.
 */
class Board {
private:
    Piece* grid[8][8];

public:
    Board() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) grid[i][j] = nullptr;
        }
    }

    ~Board() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (grid[i][j] != nullptr) {
                    delete grid[i][j];
                    grid[i][j] = nullptr;
                }
            }
        }
    }

    Piece* getPiece(int x, int y) const {
        if (x < 0 || x > 7 || y < 0 || y > 7) return nullptr;
        return grid[x][y];
    }

    void setPiece(int x, int y, Piece* piece) {
        if (x >= 0 && x <= 7 && y >= 0 && y <= 7) grid[x][y] = piece;
    }

    void movePieceOnMatrix(int startX, int startY, int endX, int endY) {
        Piece* movingPiece = grid[startX][startY];
        grid[endX][endY] = movingPiece;
        grid[startX][startY] = nullptr;
        if (movingPiece) movingPiece->markAsMoved();
    }
};