#include <gtest/gtest.h>
#include "../GameEngine/Gamecontroller.h"
#include "../GameEngine/board/Board.h"
#include "../GameEngine/protocols/GameState.h"
#include "../GameEngine/Pieces/pawn.h"
#include "../GameEngine/Pieces/king.h"
#include "../GameEngine/Pieces/rook.h"
#include "../GameEngine/Pieces/bishop.h"
#include <iostream>
#include <sstream>

// Test Fixture to set up a clean board sandbox for every individual test case
class ChessEngineTest : public ::testing::Test {
protected:
    GameController controller;
    Board& board;

    ChessEngineTest() : board(controller.getBoard()) {}

    void SetUp() override {
        // Clear the default initialization board setup to allow precise custom vector placement
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                board.setPiece(r, c, nullptr);
            }
        }
    }
};

// ============================================================================
// 1. EDGE CASE: CASTLING THROUGH ATTEMPTS TO EXPOSE THE KING
// ============================================================================

TEST_F(ChessEngineTest, DeniesCastlingWhenPathIsAttackedByEnemySniper) {
    // Setup: White King at e1 (7,4), White Rook at h1 (7,7)
    board.setPiece(7, 4, new King(Color::White));
    board.setPiece(7, 7, new Rook(Color::White));

    // 🎯 FIXED BISHOP VECTOR: Placed at a6 (2,0). 
    // This perfectly projects a diagonal vector straight through the f1 (7,5) transit square.
    board.setPiece(2, 0, new Bishop(Color::Black));

    // Attempt Kingside Castle (King from e1 to g1)
    bool castled = controller.handleMove(7, 4, 7, 6);
    
    EXPECT_FALSE(castled) << "❌ Bug: Engine allowed castling through a square guarded by an enemy piece!";
    EXPECT_EQ(board.getPiece(7, 4)->getType(), PieceType::King) << "King should not have shifted location.";
}

TEST_F(ChessEngineTest, DeniesCastlingOutofActiveCheck) {
    // Setup: White King at e1 (7,4), White Rook at h1 (7,7)
    board.setPiece(7, 4, new King(Color::White));
    board.setPiece(7, 7, new Rook(Color::White));

    // Black Rook at e8 (0,4) pinning the King in check down the open e-file
    board.setPiece(0, 4, new Rook(Color::Black));

    // Attempt Castling out of active check state
    bool castled = controller.handleMove(7, 4, 7, 6);
    
    EXPECT_FALSE(castled) << "❌ Bug: Engine allowed castling out of an active check condition!";
}

// ============================================================================
// 2. EDGE CASE: SELF-CHECK VIOLATIONS GUARD
// ============================================================================

TEST_F(ChessEngineTest, BlocksMoveThatExposesOwnKingToAnAbsolutePin) {
    // Setup: White King at e1 (7,4), White Rook at e2 (6,4) acting as absolute shield
    board.setPiece(7, 4, new King(Color::White));
    board.setPiece(6, 4, new Rook(Color::White));

    // Black Rook at e8 (0,4) establishing an absolute pin down the e-file
    board.setPiece(0, 4, new Rook(Color::Black));

    // Attempt to move the pinned White Rook sideways to d2 (6,3), which would expose the King
    bool illegalMove = controller.handleMove(6, 4, 6, 3);

    EXPECT_FALSE(illegalMove) << "❌ Bug: Engine allowed a piece to break an absolute pin and expose its King!";
    EXPECT_NE(board.getPiece(6, 4), nullptr) << "The shielding piece must remain in place.";
}

// ============================================================================
// 3. EDGE CASE: EN PASSANT VALIDATION LIFECYCLE
// ============================================================================

TEST_F(ChessEngineTest, ExecutesValidEnPassantAndClearsTargetMatrix) {
    // Setup: White Pawn advanced to 5th rank at e5 (3,4)
    board.setPiece(3, 4, new pawn(Color::White));
    // Black pawn at its starting square d7 (1,3)
    board.setPiece(1, 3, new pawn(Color::Black));

    // Place extra passive pieces to execute valid turn cycle pacing
    board.setPiece(7, 0, new Rook(Color::White));
    board.setPiece(0, 0, new Rook(Color::Black));

    // 🎯 FIXED TURN PACING CYCLE: 
    // 1. White plays a dummy rook move to burn their first turn. Active turn cycles to Black.
    controller.handleMove(7, 0, 7, 1);

    // 2. Black executes their initial double-step move from d7 to d5 (3,3). Target registers!
    bool blackMoved = controller.handleMove(1, 3, 3, 3); 
    ASSERT_TRUE(blackMoved) << "Black pawn setup move failed.";

    // 3. White executes En Passant capture diagonally into d6 (2,3)
    bool epCaptured = controller.handleMove(3, 4, 2, 3);

    EXPECT_TRUE(epCaptured) << "❌ Bug: Legal En Passant capture sequence failed to execute.";
    EXPECT_EQ(board.getPiece(3, 3), nullptr) << "The skipped Black pawn at d5 was not safely deleted from memory!";
    EXPECT_EQ(board.getPiece(2, 3)->getType(), PieceType::Pawn) << "White pawn should now rest on the target capture grid.";
}

TEST_F(ChessEngineTest, DeniesEnPassantOnSubsequentTurns) {
    board.setPiece(3, 4, new pawn(Color::White));
    board.setPiece(1, 3, new pawn(Color::Black));
    board.setPiece(7, 0, new Rook(Color::White)); 
    board.setPiece(0, 7, new Rook(Color::Black));

    controller.handleMove(7, 0, 7, 1); // 1. White burns a turn. Turn -> Black.
    controller.handleMove(1, 3, 3, 3); // 2. Black pawn double steps to d5. Turn -> White.
    controller.handleMove(7, 1, 7, 2); // 3. White ignores the EP and plays a separate rook move. Turn -> Black.
    controller.handleMove(0, 7, 0, 6); // 4. Black plays a dummy move. Turn -> White.

    // 5. White attempts a delayed En Passant capture on d6 now that the opportunity window closed
    bool lateEp = controller.handleMove(3, 4, 2, 3);

    EXPECT_FALSE(lateEp) << "❌ Bug: Engine allowed an En Passant capture after the immediate opportunity passed!";
}

// ============================================================================
// 4. EDGE CASE: PAWN PROMOTION CORNER CASES
// ============================================================================

TEST_F(ChessEngineTest, TriggersPawnPromotionAtBackRankStandardResolution) {
    // Setup: White pawn sitting on the brink of glory at a7 (1,0)
    board.setPiece(1, 0, new pawn(Color::White));

    std::stringstream mockInput("Q\n"); 
    std::streambuf* oldCin = std::cin.rdbuf(mockInput.rdbuf());

    // Step pawn into the 8th rank at a8 (0,0)
    bool moved = controller.handleMove(1, 0, 0, 0);

    EXPECT_TRUE(moved);
    EXPECT_EQ(board.getPiece(0, 0)->getType(), PieceType::Queen) 
        << "❌ Bug: Pawn reaching back rank did not convert into a Queen operation class!";
}

TEST_F(ChessEngineTest, TriggersPawnPromotionToKnight) {
    // Setup: White pawn sitting on the brink of glory at b7 (1,1)
    board.setPiece(1, 1, new pawn(Color::White));

    // 🎯 AUTOMATE INPUT: Feed "N" or "Knight" (whatever your game engine checks for)
    std::stringstream mockInput("N\n"); 
    std::streambuf* oldCin = std::cin.rdbuf(mockInput.rdbuf()); 

    // Step pawn into the 8th rank
    bool moved = controller.handleMove(1, 1, 0, 1);

    // Restore cin
    std::cin.rdbuf(oldCin);

    EXPECT_TRUE(moved);
    EXPECT_EQ(board.getPiece(0, 1)->getType(), PieceType::Knight) 
        << "❌ Bug: Under-promotion logic failed to read input or instantiate a Knight!";
}
