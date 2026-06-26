#include <bits/stdc++.h>
#include "GameController.cpp"
#include "BoardInitializer.cpp" // Handles the piece construction internally

using namespace std;

void printBoard(const Board& board) {
    cout << "\n   0  1  2  3  4  5  6  7\n  -------------------------\n";
    for (int i = 0; i < 8; i++) {
        cout << i << " |"; 
        for (int j = 0; j < 8; j++) {
            Piece* piece = board.getPiece(i, j);
            if (!piece) { cout << " . "; continue; }
            
            char symbol = 'P';
            if (piece->getType() == PieceType::Rook) symbol = 'R';
            else if (piece->getType() == PieceType::Knight) symbol = 'N';
            else if (piece->getType() == PieceType::Bishop) symbol = 'B';
            else if (piece->getType() == PieceType::Queen) symbol = 'Q';
            else if (piece->getType() == PieceType::King) symbol = 'K';

            if (piece->getColor() == Color::Black) symbol = tolower(symbol);
            cout << " " << symbol << " ";
        }
        cout << "|\n";
    }
    cout << "  -------------------------\n";
}

int main() {
    GameController controller;
    
    // Single function call to initialize the entire map layout!
    BoardInitializer::setupStandardGame(controller.getBoard());

    cout << "===============================================\n";
    cout << "   CLEAN SOLID CHESS PROTOTYPE INSTANCE RUNNING\n";
    cout << "===============================================\n";

    while (controller.isGameActive()) {
        printBoard(controller.getBoard());
        controller.handlePlayerPrompt();
    }

    return 0;
}