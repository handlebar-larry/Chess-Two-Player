#pragma once
#include "../Pieces/piece.h"
#include <cmath>

using namespace std;

/**
 * @class MoveRules
 * @brief Handles spatial collision and line-of-sight path vectors.
 * Single Responsibility: Only evaluates if a geometric path is physically blocked.
 */
class MoveRules {
public:
    static bool isStraightPathClear(int startX, int startY, int endX, int endY, Piece* board[8][8]) {
        if (startX != endX && startY != endY) return false; 

        int stepX = (endX == startX) ? 0 : ((endX > startX) ? 1 : -1);
        int stepY = (endY == startY) ? 0 : ((endY > startY) ? 1 : -1);

        int currX = startX + stepX;
        int currY = startY + stepY;

        while (currX != endX || currY != endY) {
            if (board[currX][currY] != nullptr) return false;
            currX += stepX;
            currY += stepY;
        }
        return true;
    }

    static bool isDiagonalPathClear(int startX, int startY, int endX, int endY, Piece* board[8][8]) {
        if (abs(startX - endX) != abs(startY - endY)) return false; 

        int stepX = (endX > startX) ? 1 : -1;
        int stepY = (endY > startY) ? 1 : -1;

        int currX = startX + stepX;
        int currY = startY + stepY;

        while (currX != endX && currY != endY) {
            if (board[currX][currY] != nullptr) return false;
            currX += stepX;
            currY += stepY;
        }
        return true;
    }
};
