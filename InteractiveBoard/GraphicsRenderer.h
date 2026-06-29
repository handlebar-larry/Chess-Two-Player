#pragma once
#include "../GameEngine/board/Board.h" 
#include "../comon.h"

class GraphicsRenderer {
private:
    sf::Texture boardTexture;
    std::optional<sf::Sprite> boardSprite;
    
    // A structural map to hold our individual piece textures in VRAM
    std::map<std::string, sf::Texture> pieceTextures;

    const int TILE_SIZE = 120; // 960 / 8

public:
    GraphicsRenderer() {
        // 1. Load Background Board
        if (!boardTexture.loadFromFile("assets/chessboard.png")) {
            std::cerr << "XD RUNTIME ERROR: Could not find assets/chessboard.png\n";
            std::exit(EXIT_FAILURE);
        }
        boardSprite.emplace(boardTexture);

        // 2. Load all 12 independent piece textures safely
        std::string colors[] = {"white", "black"};
        std::string types[] = {"king", "queen", "bishop", "knight", "rook", "pawn"};

        for (const auto& color : colors) {
            for (const auto& type : types) {
                std::string key = color + "-" + type;
                std::string filePath = "assets/" + key + ".png"; // e.g., "assets/white_pawn.png"

                if (!pieceTextures[key].loadFromFile(filePath)) {
                    std::cerr << "❌ RUNTIME ERROR: Failed to load asset: " << filePath << "\n";
                    std::exit(EXIT_FAILURE);
                }
            }
        }
        std::cout << "-_- All 12 standalone 128x128 assets mapped cleanly into memory!\n";
    }

    void drawGame(sf::RenderWindow& window, const Board& board) {
        
        if (boardSprite) {
            window.draw(*boardSprite); 
        }

        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                Piece* piece = board.getPiece(r, c);
                if (piece != nullptr) {
                    sf::Sprite spriteToDraw = getSpriteForPiece(piece->getType(), piece->getColor());
                    
                    // Center positioning math:
                    // 120px square tile - 100px downscaled piece = 20px leftover space.
                    // Dividing by 2 gives a uniform 10px clearance border on all sides!
                    float posX = static_cast<float>(c * TILE_SIZE) + 3.0f;
                    float posY = static_cast<float>(r * TILE_SIZE) + 5.0f;
                    
                    spriteToDraw.setPosition({posX, posY}); 
                    window.draw(spriteToDraw);
                }
            }
        }

        sf::Font boardFont;
        if (!boardFont.openFromFile("Sidepanel/arial.ttf")) {
            std::cerr << "XD RUNTIME ERROR: Board markers nahi mile! Path check kijiye.\n";
        }

        sf::Text rankFileText(boardFont);
        rankFileText.setFont(boardFont);
        rankFileText.setCharacterSize(18);

        // 1. Draw Files (a-h) along the bottom edge of Row index 7
        for (int c = 0; c < 8; ++c) {
            rankFileText.setString(static_cast<char>('a' + c));
            // Alternating text color contrast depending on tile background colors
            rankFileText.setFillColor(sf::Color(35, 35, 35)); 
            rankFileText.setPosition({static_cast<float>(c * TILE_SIZE) + 5.f, static_cast<float>(8 * TILE_SIZE) - 20.f});
            window.draw(rankFileText);
        }

        // 2. Draw Ranks (1-8) along the right side of Column index 7
        for (int r = 0; r < 8; ++r) {
            rankFileText.setString(static_cast<char>('8' - r));
            rankFileText.setFillColor(sf::Color(35, 35, 35));
            rankFileText.setPosition({static_cast<float>(8 * TILE_SIZE) - 15.f, static_cast<float>(r * TILE_SIZE) + 5.f});
            window.draw(rankFileText);
        }
        
    }
    sf::Sprite getSpriteForPiece(PieceType type, Color color) {
        // FORCE STRINGS TO LOWERCASE TO MATCH YOUR "white-king.png" DIRECTORY FILENAMES
        std::string colorKey = (color == Color::White) ? "white" : "black";
        
        std::string typeKey = "";
        switch (type) {
            case PieceType::Queen:  typeKey = "queen"; break; 
            case PieceType::King:   typeKey = "king"; break; 
            case PieceType::Bishop: typeKey = "bishop"; break; 
            case PieceType::Knight: typeKey = "knight"; break;
            case PieceType::Rook:   typeKey = "rook"; break;
            case PieceType::Pawn:   typeKey = "pawn"; break;
        }

        // Pull out the standalone texture safely using lower-case hyphen mappings
        sf::Sprite sprite(pieceTextures[colorKey + "-" + typeKey]); 

        return sprite;
    }
};