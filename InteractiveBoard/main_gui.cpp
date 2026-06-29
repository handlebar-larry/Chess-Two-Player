#include "GraphicsRenderer.h"
#include "../GameEngine/GameController.h" 
#include "../GameEngine/board/BoardInitializer.h" 
#include "Sidepanel/SidePanel.h"
#include "../comon.h"

int main() {
    const int TILE_SIZE = 120;
    const int BOARD_SIZE = 960;
    const int SIDEBAR_WIDTH = 400;

    // 🎯 The fixed logical blueprint coordinate space (1360 x 960)
    const float VIRTUAL_WIDTH = static_cast<float>(BOARD_SIZE + SIDEBAR_WIDTH); 
    const float VIRTUAL_HEIGHT = static_cast<float>(BOARD_SIZE);

    sf::RenderWindow window(sf::VideoMode({1360, 960}), 
                            "Two Player Chess Engine", 
                            sf::Style::Default);

    // Lock down the game canvas coordinates at their native scaling aspect ratios
    sf::View gameView(sf::FloatRect({0.f, 0.f}, {VIRTUAL_WIDTH, VIRTUAL_HEIGHT}));

    window.setView(window.getDefaultView());
    window.clear(sf::Color(35, 35, 35));

    window.setView(gameView);

    GameController engine;
    GraphicsRenderer renderer;
    
    // Initialize Side Panel exactly where the board ends (x = 960)
    SidePanel sidePanel(BOARD_SIZE, SIDEBAR_WIDTH, VIRTUAL_HEIGHT);

    // Sets up the initial 32 piece layout matrices on your backend board state
    BoardInitializer::setupStandardGame(engine.getBoard());

    bool isPieceSelected = false;
    int startX = -1, startY = -1;

    std::cout << "-_- Connected Interactive Board & Sidepanel to Engine Loop!\n";

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // 🎯 THE PERFECT BALANCED PILLARBOX CENTERING RESIZE HANDLE
            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                float windowWidth = static_cast<float>(resized->size.x);
                float windowHeight = static_cast<float>(resized->size.y);

                float targetRatio = VIRTUAL_WIDTH / VIRTUAL_HEIGHT;
                float currentRatio = windowWidth / windowHeight;

                float viewportX = 0.f, viewportY = 0.f;
                float viewportWidth = 1.f, viewportHeight = 1.f;

                // Monitor aspect is too wide -> Add equal margins on left and right sides
                if (currentRatio > targetRatio) {
                    viewportWidth = targetRatio / currentRatio;
                    viewportX = (1.f - viewportWidth) / 2.f; 
                } 
                // Monitor aspect is too tall -> Add equal margins on top and bottom sides
                else {
                    viewportHeight = currentRatio / targetRatio;
                    viewportY = (1.f - viewportHeight) / 2.f;
                }

                // Apply the balanced margins straight onto our persistent game view
                gameView.setViewport(sf::FloatRect({viewportX, viewportY}, {viewportWidth, viewportHeight}));
                window.setView(gameView);
            }

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    // 🎯 Map screen pixel clicks cleanly into our letterboxed virtual coordinate zone
                    sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    // Intercept sidebar interaction clicks safely
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

                        std::cout << "-_- Requesting Move to Matrix Row: " << endX << ", Col: " << endY << "\n";
                        
                        if (engine.handleMove(startX, startY, endX, endY)) {
                            std::cout << "-_- Engine validated move! State updated.\n";
                        } else {
                            std::cout << "XD Illegal move rejected by backend rules.\n";
                        }
                    }
                }
            }
        }

        // 🎯 MATCHING PLATES GRAPHICS COLOR EXTENSION
        // Wipe the entire application display layout to your exact panel slate-gray color!
        window.clear(sf::Color(35, 35, 35));

        // 1. Draw the chessboard configuration
        renderer.drawGame(window, engine.getBoard());

        // 2. Render the UI Sidepanel elements safely using your exact matching asset map
        auto tracker = engine.getCaptureTracker(); 
        sidePanel.render(window, 
                         tracker.getWhiteScore(), 
                         tracker.getBlackScore(), 
                         tracker.getPointImbalance(),
                         tracker.getWhiteCapturedList(), 
                         tracker.getBlackCapturedList());

        window.display();
    }

    return 0;
}
