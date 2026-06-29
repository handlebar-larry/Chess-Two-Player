#include "../../comon.h"
#include "../../GameEngine/Pieces/piece.h"
#include "../GraphicsRenderer.h"

using namespace std;

class SidePanel {
private:
    float startX, panelWidth, panelHeight;
    GraphicsRenderer renderer;

    sf::RectangleShape background;
    sf::Font font;
    unique_ptr<sf::Text> advantageText;
    unique_ptr<sf::Text> historyTitleText;
    float scrollOffset = 0.f;

    // Helper method to link up with your existing asset/sprite map
    sf::Sprite getMiniSprite(PieceType type, Color color);

public:
    SidePanel(float boardSize, float width, float height);
    ~SidePanel() = default;

    void render(sf::RenderWindow& window, int pointImbalance, 
                       const std::vector<std::string>& moveHistory, 
                       const std::vector<PieceType>& whiteCaptured, 
                       const std::vector<PieceType>& blackCaptured);
    void handleScroll(float delta);
    
};

sf::Sprite SidePanel::getMiniSprite(PieceType type, Color color) {
    return renderer.getSpriteForPiece(type, color);
}

SidePanel::SidePanel(float x, float width, float height) 
    : startX(x), panelWidth(width), panelHeight(height) {
    
    background.setSize({panelWidth, panelHeight});
    background.setPosition({startX, 0.f}); // (960, 0)
    background.setFillColor(sf::Color(35, 35, 35));

    if (!font.openFromFile("Sidepanel/arial.ttf")) { 
        std::cerr << "Error loading font\n";
    }

    advantageText = std::make_unique<sf::Text>(font);
    advantageText->setCharacterSize(18);
    advantageText->setFillColor(sf::Color::White);
    // 🎯 FIXED: startX (960) + 30px padding takki yeh side panel ke andar dikhe, board par nahi!
    advantageText->setPosition({startX + 30.f, 30.f}); 

    historyTitleText = std::make_unique<sf::Text>(font);
    historyTitleText->setString("Moves Log:");
    historyTitleText->setCharacterSize(18);
    historyTitleText->setFillColor(sf::Color(150, 150, 150));
    // 🎯 FIXED: Alignment inside the panel bounds
    historyTitleText->setPosition({startX + 30.f, 90.f});
}

void SidePanel::handleScroll(float delta) {
    scrollOffset += delta * 20.f; // Adjust scroll sensitivity speed
    if (scrollOffset > 0.f) scrollOffset = 0.f; // Clamp top boundary
}

void SidePanel::render(sf::RenderWindow& window, int pointImbalance, 
                       const std::vector<std::string>& moveHistory, 
                       const std::vector<PieceType>& whiteCaptured, 
                       const std::vector<PieceType>& blackCaptured) {
    // 1. Clear application viewport background layout panel
   window.draw(background);

    // 🎯 Step A: Render Captured Pieces First to calculate dynamic positions!
    
    // Render White's Captured Pieces (Captured by Black side)
    float whiteStartX = 990.f;
    for (size_t i = 0; i < whiteCaptured.size(); ++i) {
        sf::Sprite capSprite = renderer.getSpriteForPiece(whiteCaptured[i], Color::Black); 
        capSprite.setScale({0.22f, 0.22f});
        capSprite.setPosition({whiteStartX + (i * 18.f), 880.f}); 
        window.draw(capSprite);
    }

    // Render Black's Captured Pieces (Captured by White side)
    float blackStartX = 990.f;
    for (size_t i = 0; i < blackCaptured.size(); ++i) {
        sf::Sprite capSprite = renderer.getSpriteForPiece(blackCaptured[i], Color::White);
        capSprite.setScale({0.22f, 0.22f});
        capSprite.setPosition({blackStartX + (i * 18.f), 65.f}); 
        window.draw(capSprite);
    }


    // 🎯 Step B: Dynamic Text Positioning (Chess.com Side-Lock Layout)
    // Calculate exactly where the trailing edge of the sprites array ends
    float whiteEndX = whiteStartX + (whiteCaptured.size() * 14.f) + 10.f;
    float blackEndX = blackStartX + (blackCaptured.size() * 14.f) + 10.f;

    if (pointImbalance > 0) {
        // White is in advantage: Lock it on White's line (Y = 65) right after their pieces
        advantageText->setString("+" + std::to_string((pointImbalance)));
        advantageText->setFillColor(sf::Color::White);
        advantageText->setPosition({whiteStartX, 860.f}); 
        window.draw(*advantageText);
    } 
    else if (pointImbalance < 0) {
        // Black is in advantage: Lock it on Black's line (Y = 90) right after their pieces
        advantageText->setString("+" + std::to_string(abs(pointImbalance))); 
        advantageText->setFillColor(sf::Color(180, 180, 180));
        advantageText->setPosition({blackStartX, 45.f}); 
        window.draw(*advantageText);
    } 
    else {
        // Tied game (0): You can hide it or keep a clean neutral marker on the dashboard title line
        advantageText->setString(""); 
    }


    // 4. Render Moves Log Header Title
    historyTitleText->setPosition({990.f, 130.f}); 
    window.draw(*historyTitleText);


    // 5. Render moves ledger dynamically
    unsigned int moveCounter = 1;
    float currentY = 170.f + scrollOffset; 

    sf::Text moveNumberText(font);
    moveNumberText.setCharacterSize(16);
    moveNumberText.setFillColor(sf::Color(130, 130, 130));

    sf::Text whiteMoveText(font);
    whiteMoveText.setCharacterSize(16);
    whiteMoveText.setFillColor(sf::Color(240, 240, 240));

    sf::Text blackMoveText(font);
    blackMoveText.setCharacterSize(16);
    blackMoveText.setFillColor(sf::Color(190, 190, 190));

    for (size_t i = 0; i < moveHistory.size(); i += 2) {
        if (currentY > panelHeight - 100.f) {
            break; 
        }

        if (currentY >= 160.f) {
            moveNumberText.setString(std::to_string(moveCounter) + ".");
            moveNumberText.setPosition({990.f, currentY});
            window.draw(moveNumberText);

            whiteMoveText.setString(moveHistory[i]);
            whiteMoveText.setPosition({1030.f, currentY});
            window.draw(whiteMoveText);

            if (i + 1 < moveHistory.size()) {
                blackMoveText.setString(moveHistory[i + 1]);
                blackMoveText.setPosition({1140.f, currentY}); 
                window.draw(blackMoveText);
            }
        }
        
        currentY += 28.f; 
        moveCounter++;
    }
}