#include <SFML/Graphics.hpp>
#include "GraphicsRenderer.cpp"
#include "../GameEngine/GameController.cpp" 
#include "../GameEngine/BoardInitializer.cpp" 
#include <iostream>
#include <optional>

int main() {
    // 120px grid spacing matched to your high-res asset bounds
    const int TILE_SIZE = 120;
    
    // Window scaled directly to match your 960x960 board texture perfectly
    sf::RenderWindow window(sf::VideoMode({960, 960}), "SOLID C++ Chess Engine GUI");

    GameController engine;
    GraphicsRenderer renderer;

    // Sets up the initial 32 piece layout matrices on your backend board state
    BoardInitializer::setupStandardGame(engine.getBoard());

    bool isPieceSelected = false;
    int startX = -1, startY = -1;

    std::cout << "🚀 Connected Interactive Board to Engine Loop! Click a piece to begin.\n";

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    int mouseX = mousePressed->position.x;
                    int mouseY = mousePressed->position.y;

                    // Translates screen pixels accurately to 0-7 board matrix index coordinates
                    int clickedCol = mouseX / TILE_SIZE; 
                    int clickedRow = mouseY / TILE_SIZE; 

                    if (!isPieceSelected) {
                        // FIRST CLICK: Select a piece from the board matrix
                        if (engine.getBoard().getPiece(clickedRow, clickedCol) != nullptr) {
                            startX = clickedRow;
                            startY = clickedCol;
                            isPieceSelected = true;
                            std::cout << "🎯 Selected Piece at Matrix Row: " << startX << ", Col: " << startY << "\n";
                        }
                    } else {
                        // SECOND CLICK: Destination targeting
                        int endX = clickedRow;
                        int endY = clickedCol;
                        isPieceSelected = false; // Reset selection token immediately

                        std::cout << "📍 Requesting Move to Matrix Row: " << endX << ", Col: " << endY << "\n";
                        
                        // 🔗 ENGINE BRIDGE:
                        // Instead of moving blindly, we pass the path directly to your backend controller.
                        // Change 'handleMove' to your engine's exact processing method name if needed.
                        if (engine.handleMove(startX, startY, endX, endY)) {
                            std::cout << "✅ Engine validated move! State updated.\n";
                        } else {
                            std::cout << "❌ Illegal move rejected by backend rules.\n";
                        }
                    }
                }
            }
        }

        // Draw the latest validated board configuration
        renderer.drawGame(window, engine.getBoard());
    }

    return 0;
}