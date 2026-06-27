#pragma once
#include "Board.cpp"
#include "../Pieces/Pawn.cpp"
#include "../Pieces/Rook.cpp"
#include "../Pieces/Knight.cpp"
#include "../Pieces/Bishop.cpp"
#include "../Pieces/Queen.cpp"
#include "../Pieces/King.cpp"

class BoardInitializer {
public:
    static void setupStandardGame(Board& board) {
        // Populate BLACK Major Pieces (Row 0 - Top of screen)
        board.setPiece(0, 0, new Rook(Color::Black));   board.setPiece(0, 1, new Knight(Color::Black));
        board.setPiece(0, 2, new Bishop(Color::Black)); board.setPiece(0, 3, new Queen(Color::Black));
        board.setPiece(0, 4, new King(Color::Black));   board.setPiece(0, 5, new Bishop(Color::Black));
        board.setPiece(0, 6, new Knight(Color::Black)); board.setPiece(0, 7, new Rook(Color::Black));
        
        // Populate BLACK Pawns (Row 1)
        for (int j = 0; j < 8; j++) board.setPiece(1, j, new pawn(Color::Black));

        // Populate WHITE Pawns (Row 6)
        for (int j = 0; j < 8; j++) board.setPiece(6, j, new pawn(Color::White));

        // Populate WHITE Major Pieces (Row 7 - Bottom of screen)
        board.setPiece(7, 0, new Rook(Color::White));   board.setPiece(7, 1, new Knight(Color::White));
        board.setPiece(7, 2, new Bishop(Color::White)); board.setPiece(7, 3, new Queen(Color::White));
        board.setPiece(7, 4, new King(Color::White));   board.setPiece(7, 5, new Bishop(Color::White));
        board.setPiece(7, 6, new Knight(Color::White)); board.setPiece(7, 7, new Rook(Color::White));
    }
};