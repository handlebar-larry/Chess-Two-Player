#include <vector>
#include "../Pieces/piece.cpp" // Adjust to your actual PieceType enum path

class CaptureTracker {
private:
    std::vector<PieceType> whiteCaptured; // Pieces captured BY White (Black pieces)
    std::vector<PieceType> blackCaptured; // Pieces captured BY Black (White pieces)
    int whiteMaterialPoints;
    int blackMaterialPoints;

    int getPieceValue(PieceType type) const;

public:
    CaptureTracker();
    ~CaptureTracker() = default;

    // Core loggers called by the engine loop
    void logCapture(PieceType type, Color attackerColor);
    void reset();

    // Data getters for your future frontend sidepanel to read from
    int getWhiteScore() const { return whiteMaterialPoints; }
    int getBlackScore() const { return blackMaterialPoints; }
    
    // Returns positive if White is winning, negative if Black is winning
    int getPointImbalance() const; 

    const std::vector<PieceType>& getWhiteCapturedList() const { return whiteCaptured; }
    const std::vector<PieceType>& getBlackCapturedList() const { return blackCaptured; }
};

CaptureTracker::CaptureTracker() : whiteMaterialPoints(0), blackMaterialPoints(0) {}

int CaptureTracker::getPieceValue(PieceType type) const {
    switch (type) {
        case PieceType::Pawn:   return 1;
        case PieceType::Knight: return 3;
        case PieceType::Bishop: return 3;
        case PieceType::Rook:   return 5;
        case PieceType::Queen:  return 9;
        default: return 0;
    }
}

void CaptureTracker::logCapture(PieceType type, Color attackerColor) {
    int value = getPieceValue(type);
    
    if (attackerColor == Color::White) {
        whiteCaptured.push_back(type);
        whiteMaterialPoints += value;
    } else {
        blackCaptured.push_back(type);
        blackMaterialPoints += value;
    }
}

int CaptureTracker::getPointImbalance() const {
    // Standard imbalance: White points minus Black points
    return whiteMaterialPoints - blackMaterialPoints;
}

void CaptureTracker::reset() {
    whiteCaptured.clear();
    blackCaptured.clear();
    whiteMaterialPoints = 0;
    blackMaterialPoints = 0;
}