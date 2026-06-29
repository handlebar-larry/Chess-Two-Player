#include "CaptureTracker.h"
#include "../../comon.h"

using namespace std;

CaptureTracker::CaptureTracker() {
    reset();
}

void CaptureTracker::reset() {
    whiteCaptured.clear();
    blackCaptured.clear();
    whiteScore = 0;
    blackScore = 0;
}

int CaptureTracker::getPieceValue(PieceType type) const {
    switch (type) {
        case PieceType::Pawn:   return 1;
        case PieceType::Knight: return 3;
        case PieceType::Bishop: return 3;
        case PieceType::Rook:   return 5;
        case PieceType::Queen:  return 9;
        case PieceType::King:   return 0;
        default:                return 0;
    }
}

void CaptureTracker::logCapture(PieceType type, Color attackerColor) {
    int value = getPieceValue(type);
    if (attackerColor == Color::White) {
        whiteCaptured.push_back(type);
        whiteScore += value;
    } else {
        blackCaptured.push_back(type);
        blackScore += value;
    }
}

int CaptureTracker::getPointImbalance() const {
    return whiteScore - blackScore; 
}

const vector<PieceType>& CaptureTracker::getWhiteCapturedList() const { return whiteCaptured; }
const vector<PieceType>& CaptureTracker::getBlackCapturedList() const { return blackCaptured; }
int CaptureTracker::getWhiteScore() const { return whiteScore; }
int CaptureTracker::getBlackScore() const { return blackScore; }