#pragma once
#include <bits/stdc++.h>

enum class Color { White, Black };
enum class PieceType { Pawn, Rook, Knight, Bishop, Queen, King };

/**
 * @class Piece
 * @brief The abstract base class representing a generic chess piece.
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

    // Constant Getters for Tightly Encapsulated Properties
    Color getColor() const { return color; }
    PieceType getType() const { return type; }
    bool getHasMoved() const { return hasMoved; }
    
    // State mutation tracker
    void markAsMoved() { hasMoved = true; }

    /**
     * @brief Pure virtual function to enforce geometric move path limits per piece.
     */
    virtual bool isValidMove(int startX, int startY, int endX, int endY, Piece* board[8][8]) = 0;
};