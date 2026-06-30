#pragma once

#include <vector>

namespace view {

enum class Color {
    White,
    Black
};

enum class PieceType {
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King
};

struct Piece {
    int row;
    int col;
    PieceType type;
    Color color;
};

using BoardSnapshot = std::vector<Piece>;
using CapturedPieces = std::vector<PieceType>;

} // namespace view
