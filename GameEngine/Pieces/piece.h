#pragma once
#include "../../comon.h"

enum class Color { White, Black };
enum class PieceType { Pawn, Rook, Knight, Bishop, Queen, King };

/**
 * @class Piece
 * The abstract base class representing a generic chess piece.
 * Follows the Liskov Substitution Principle (LSP).
 */
class Piece {
protected:
    Color color;
    PieceType type;
    bool hasMoved;

public:
    Piece(Color c, PieceType t) : color(c), type(t), hasMoved(false) {}
    virtual ~Piece() {}

    Color getColor() const { return color; }
    PieceType getType() const { return type; }
    bool getHasMoved() const { return hasMoved; }
    
    void markAsMoved() { hasMoved = true; }

    virtual bool isValidMove(int startX, int startY, int endX, int endY, Piece* board[8][8]) = 0;
};