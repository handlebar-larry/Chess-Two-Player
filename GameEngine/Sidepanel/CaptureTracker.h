#ifndef CAPTURE_TRACKER_H
#define CAPTURE_TRACKER_H

#include "../Pieces/piece.h"
#include <vector>

using namespace std;

class CaptureTracker {
private:
    vector<PieceType> whiteCaptured;
    vector<PieceType> blackCaptured;
    int whiteScore;
    int blackScore;

    int getPieceValue(PieceType type) const;

public:
    CaptureTracker();

    void logCapture(PieceType type, Color attackerColor);
    int getPointImbalance() const;
    void reset();

    const vector<PieceType>& getWhiteCapturedList() const;
    const vector<PieceType>& getBlackCapturedList() const;
    int getWhiteScore() const;
    int getBlackScore() const;
};

#endif
