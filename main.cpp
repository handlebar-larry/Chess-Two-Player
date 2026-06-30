#include "InteractiveBoard/GraphicsRenderer.h"
#include "InteractiveBoard/Sidepanel/SidePanel.h"
#include "InteractiveBoard/ViewTypes.h"
#include "GameEngine/Gamecontroller.h"
#include "GameEngine/board/BoardInitializer.h"
#include "GameEngine/Sidepanel/MoveHistory.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace {

view::Color toViewColor(Color color) {
    return color == Color::White ? view::Color::White : view::Color::Black;
}

view::PieceType toViewPieceType(PieceType type) {
    switch (type) {
        case PieceType::Pawn:   return view::PieceType::Pawn;
        case PieceType::Rook:   return view::PieceType::Rook;
        case PieceType::Knight: return view::PieceType::Knight;
        case PieceType::Bishop: return view::PieceType::Bishop;
        case PieceType::Queen:  return view::PieceType::Queen;
        case PieceType::King:   return view::PieceType::King;
    }

    return view::PieceType::Pawn;
}

view::BoardSnapshot buildBoardSnapshot(const Board& board) {
    view::BoardSnapshot snapshot;
    snapshot.reserve(32);

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece* piece = board.getPiece(row, col);
            if (piece) {
                snapshot.push_back({
                    row,
                    col,
                    toViewPieceType(piece->getType()),
                    toViewColor(piece->getColor())
                });
            }
        }
    }

    return snapshot;
}

view::CapturedPieces buildCapturedPieces(const std::vector<PieceType>& capturedPieces) {
    view::CapturedPieces viewPieces;
    viewPieces.reserve(capturedPieces.size());

    for (PieceType type : capturedPieces) {
        viewPieces.push_back(toViewPieceType(type));
    }

    return viewPieces;
}

} // namespace

int main() {
    const int TILE_SIZE = 120;
    const int BOARD_SIZE = 960;
    const int SIDEBAR_WIDTH = 400;
    const float VIRTUAL_WIDTH = static_cast<float>(BOARD_SIZE + SIDEBAR_WIDTH);
    const float VIRTUAL_HEIGHT = static_cast<float>(BOARD_SIZE);

    std::vector<std::string> moveHistory;

    sf::RenderWindow window(
        sf::VideoMode({1360, 960}),
        "Two Player Chess Engine",
        sf::Style::Default
    );

    sf::View gameView(sf::FloatRect({0.f, 0.f}, {VIRTUAL_WIDTH, VIRTUAL_HEIGHT}));
    window.setView(gameView);

    GameController engine;
    BoardInitializer::setupStandardGame(engine.getBoard());

    GraphicsRenderer renderer;
    SidePanel sidePanel(BOARD_SIZE, SIDEBAR_WIDTH, VIRTUAL_HEIGHT);

    bool isPieceSelected = false;
    int startX = -1;
    int startY = -1;

    std::cout << "-_- Connected Interactive Board & Sidepanel to Engine Loop!\n";

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                float windowWidth = static_cast<float>(resized->size.x);
                float windowHeight = static_cast<float>(resized->size.y);
                float targetRatio = VIRTUAL_WIDTH / VIRTUAL_HEIGHT;
                float currentRatio = windowWidth / windowHeight;

                float viewportX = 0.f;
                float viewportY = 0.f;
                float viewportWidth = 1.f;
                float viewportHeight = 1.f;

                if (currentRatio > targetRatio) {
                    viewportWidth = targetRatio / currentRatio;
                    viewportX = (1.f - viewportWidth) / 2.f;
                } else {
                    viewportHeight = currentRatio / targetRatio;
                    viewportY = (1.f - viewportHeight) / 2.f;
                }

                gameView.setViewport(sf::FloatRect({viewportX, viewportY}, {viewportWidth, viewportHeight}));
                window.setView(gameView);
            }

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    if (worldPos.x >= BOARD_SIZE || worldPos.x < 0 || worldPos.y < 0 || worldPos.y >= VIRTUAL_HEIGHT) {
                        std::cout << "XD Interface boundary interaction detected (Ignored for piece movement).\n";
                        continue;
                    }

                    int clickedCol = static_cast<int>(worldPos.x) / TILE_SIZE;
                    int clickedRow = static_cast<int>(worldPos.y) / TILE_SIZE;

                    if (!isPieceSelected) {
                        if (engine.getBoard().getPiece(clickedRow, clickedCol) != nullptr) {
                            startX = clickedRow;
                            startY = clickedCol;
                            isPieceSelected = true;
                            std::cout << "-_- Selected Piece at Matrix Row: " << startX << ", Col: " << startY << "\n";
                        }
                    } else {
                        int endX = clickedRow;
                        int endY = clickedCol;
                        isPieceSelected = false;

                        Piece* movingPiece = engine.getBoard().getPiece(startX, startY);
                        PieceType movingPieceType = movingPiece->getType();
                        bool pieceThere = engine.getBoard().getPiece(endX, endY) != nullptr;

                        if (engine.handleMove(startX, startY, endX, endY)) {
                            std::cout << "-_- Engine validated move! State updated.\n";
                            moveHistory.push_back(generateMoveString(
                                startX,
                                startY,
                                endX,
                                endY,
                                movingPieceType,
                                pieceThere,
                                engine.getBoard()
                            ));
                        } else {
                            std::cout << "XD Illegal move rejected by backend rules.\n";
                        }
                    }
                }
            }

            if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (mouseWheel->wheel == sf::Mouse::Wheel::Vertical) {
                    sidePanel.handleScroll(mouseWheel->delta);
                }
            }
        }

        window.clear(sf::Color(35, 35, 35));

        renderer.drawGame(window, buildBoardSnapshot(engine.getBoard()));

        CaptureTracker tracker = engine.getCaptureTracker();
        sidePanel.render(
            window,
            tracker.getPointImbalance(),
            moveHistory,
            buildCapturedPieces(tracker.getWhiteCapturedList()),
            buildCapturedPieces(tracker.getBlackCapturedList())
        );

        window.display();
    }

    return 0;
}
