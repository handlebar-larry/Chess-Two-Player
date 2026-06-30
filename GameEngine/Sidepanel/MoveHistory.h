#include "../Pieces/piece.h"
#include "../protocols/GameState.h"
#include <string>

using namespace std;

string generateMoveString(int startRow, int startCol, int endRow, int endCol, PieceType type, bool isCapture, Board& board) {
    string moveStr = "";
    
    // 1. Prefix for pieces (Pawns have no prefix)
    if (type == PieceType::Knight) moveStr += "N";
    else if (type == PieceType::Bishop) moveStr += "B";
    else if (type == PieceType::Rook)   moveStr += "R";
    else if (type == PieceType::Queen)  moveStr += "Q";
    else if (type == PieceType::King)   moveStr += "K";

    // 2. Capture symbol
    if (isCapture) {
        if (type == PieceType::Pawn) {
            moveStr += static_cast<char>('a' + startCol); // Pawns show originating file on capture (e.g., exd5)
        }
        moveStr += "x";
    }

    // 3. Destination square
    moveStr += static_cast<char>('a' + endCol);
    moveStr += static_cast<char>('8' - endRow);

    GameState tempState;

    Color King=board.getPiece(endRow, endCol)->getColor()==Color::White?Color::Black:Color::White;
    if(tempState.isCheckmate(King,board)){
        moveStr += "#";
    } 
    else if(tempState.isCheck(King,board)){
        moveStr += "+";
    }

    return moveStr;
}

