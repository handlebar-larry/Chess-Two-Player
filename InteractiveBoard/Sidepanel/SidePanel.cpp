#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../../GameEngine/Pieces/piece.cpp"
#include "../GraphicsRenderer.cpp"

class SidePanel {
private:
    float startX;
    float panelWidth;
    float panelHeight;
    GraphicsRenderer renderer;

    sf::RectangleShape background;
    sf::Font font;
    sf::Text whiteScoreText;
    sf::Text blackScoreText;
    sf::Text imbalanceText;

    // Helper method to link up with your existing asset/sprite map
    sf::Sprite getMiniSprite(PieceType type, Color color);

public:
    SidePanel(float boardSize, float width, float height);
    ~SidePanel() = default;

    // The main frontend engine calls this every frame to draw the UI state
    void render(sf::RenderWindow& window, 
                int whiteScore, int blackScore, int imbalance,
                const std::vector<PieceType>& whiteCaptured, 
                const std::vector<PieceType>& blackCaptured);
};

// ============================================================================
// 🎯 FIXED POSITION: Defining getMiniSprite first satisfies the compiler!
// ============================================================================
sf::Sprite SidePanel::getMiniSprite(PieceType type, Color color) {
    return renderer.getSpriteForPiece(type, color);
}

SidePanel::SidePanel(float boardSize, float width, float height) 
    : whiteScoreText(font), blackScoreText(font), imbalanceText(font) {
    startX = boardSize;
    panelWidth = width;
    panelHeight = height;

    // 1. Dark Gray background layout block
    background.setSize(sf::Vector2f(panelWidth, panelHeight));
    background.setPosition({startX, 0});
    background.setFillColor(sf::Color(35, 35, 35)); 

    // 2. Load the UI text font 
    if (!font.openFromFile("Sidepanel/arial.ttf")) { 
        if (!font.openFromFile("InteractiveBoard/Sidepanel/arial.ttf")) {
            std::cerr << "⚠️ Frontend Error: Failed to find font at Sidepanel/arial.ttf\n";
        }
    }

    // 3. Configure Layout Overlays
    whiteScoreText.setCharacterSize(20);
    whiteScoreText.setFillColor(sf::Color::White);
    whiteScoreText.setPosition({startX + 20, panelHeight - 60});

    blackScoreText.setCharacterSize(20);
    blackScoreText.setFillColor(sf::Color::White);
    blackScoreText.setPosition({startX + 20, 30});

    imbalanceText.setCharacterSize(18);
    imbalanceText.setFillColor(sf::Color(200, 200, 200));
    imbalanceText.setPosition({startX + 20, panelHeight / 2 - 10});
}

void SidePanel::render(sf::RenderWindow& window, 
                       int whiteScore, int blackScore, int imbalance,
                       const std::vector<PieceType>& whiteCaptured, 
                       const std::vector<PieceType>& blackCaptured) {
    
    // Draw background layout panel frame
    window.draw(background);

    // Update dynamically calculated labels
    whiteScoreText.setString("White Points: " + std::to_string(whiteScore));
    blackScoreText.setString("Black Points: " + std::to_string(blackScore));
    
    std::string imbStr = (imbalance >= 0) ? "+" + std::to_string(imbalance) : std::to_string(imbalance);
    imbalanceText.setString("Material Imbalance: " + imbStr);

    window.draw(blackScoreText);
    window.draw(whiteScoreText);
    window.draw(imbalanceText);

    // 🏆 Row Display: Show Black pieces that White has eliminated (Top Zone)
    int rowY = 70;
    for (size_t i = 0; i < blackCaptured.size(); ++i) {
        sf::Sprite icon = getMiniSprite(blackCaptured[i], Color::White);
        icon.setScale({0.5f, 0.5f}); // Scale down to fit neatly into the side panel grid array
        
        float x = startX + 20 + (i % 6) * 45;
        float y = rowY + (i / 6) * 45;
        icon.setPosition({x, y});
        window.draw(icon);
    }

    // 🏆 Row Display: Show White pieces that Black has eliminated (Bottom Zone)
    rowY = panelHeight - 120;
    for (size_t i = 0; i < whiteCaptured.size(); ++i) {
        sf::Sprite icon = getMiniSprite(whiteCaptured[i], Color::Black);
        icon.setScale({0.5f, 0.5f});
        
        float x = startX + 20 + (i % 6) * 45;
        float y = rowY - (i / 6) * 45; // Grow upward vertically
        icon.setPosition({x, y});
        window.draw(icon);
    }
}