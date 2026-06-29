#include "GraphicsRenderer.h"
#include "../GameEngine/GameController.h" 
#include "../GameEngine/board/BoardInitializer.h" 
#include "Sidepanel/SidePanel.h"
#include "../comon.h"

int main() {
    // 120px grid spacing matched to your high-res asset bounds
    const int TILE_SIZE = 120;

    const int BOARD_SIZE = 960;
    const int SIDEBAR_WIDTH = 300;
    const int WINDOW_WIDTH = BOARD_SIZE + SIDEBAR_WIDTH;
    const int WINDOW_HEIGHT = BOARD_SIZE;

    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Two Player Chess Engine");

    GameController engine;
    GraphicsRenderer renderer;
    
    // 🎯 INITIALIZE SIDE PANEL: Position it exactly where the board ends (x = 960)
    SidePanel sidePanel(BOARD_SIZE, SIDEBAR_WIDTH, WINDOW_HEIGHT);

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

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    int mouseX = mousePressed->position.x;
                    int mouseY = mousePressed->position.y;

                    // 🛡️ CRITICAL GUARD: If user clicks inside the sidebar UI region, 
                    // intercept it so it doesn't trigger out-of-bounds board array access!
                    if (mouseX >= BOARD_SIZE) {
                        std::cout << "XD Sidebar interaction detected (Ignored for piece movement).\n";
                        continue; 
                    }

                    // Translates screen pixels accurately to 0-7 board matrix index coordinates
                    int clickedCol = mouseX / TILE_SIZE; 
                    int clickedRow = mouseY / TILE_SIZE; 

                    if (!isPieceSelected) {
                        // FIRST CLICK: Select a piece from the board matrix
                        if (engine.getBoard().getPiece(clickedRow, clickedCol) != nullptr) {
                            startX = clickedRow;
                            startY = clickedCol;
                            isPieceSelected = true;
                            std::cout << "-_- Selected Piece at Matrix Row: " << startX << ", Col: " << startY << "\n";
                        }
                    } else {
                        // SECOND CLICK: Destination targeting
                        int endX = clickedRow;
                        int endY = clickedCol;
                        isPieceSelected = false; // Reset selection token immediately

                        std::cout << "-_- Requesting Move to Matrix Row: " << endX << ", Col: " << endY << "\n";
                        
                        // 🔗 ENGINE BRIDGE: Passes execution directly to your backend controller.
                        if (engine.handleMove(startX, startY, endX, endY)) {
                            std::cout << "-_- Engine validated move! State updated.\n";
                        } else {
                            std::cout << "XD Illegal move rejected by backend rules.\n";
                        }
                    }
                }
            }
        }

        window.clear();

        // 1. Draw the latest validated board configuration
        renderer.drawGame(window, engine.getBoard());

        // 2. 🔗 RENDERING THE UI SIDE PANEL: 
        // Feed it your independent CaptureTracker instance tracking vectors natively!
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