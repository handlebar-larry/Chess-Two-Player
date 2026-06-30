#pragma once

#include "ViewTypes.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

class GraphicsRenderer {
private:
    sf::Texture boardTexture;
    std::optional<sf::Sprite> boardSprite;
    std::map<std::string, sf::Texture> pieceTextures;

    const int TILE_SIZE = 120;

    bool loadTexture(sf::Texture& texture, const std::vector<std::string>& paths) {
        for (const auto& path : paths) {
            if (texture.loadFromFile(path)) {
                return true;
            }
        }
        return false;
    }

    bool loadFont(sf::Font& font, const std::vector<std::string>& paths) {
        for (const auto& path : paths) {
            if (font.openFromFile(path)) {
                return true;
            }
        }
        return false;
    }

public:
    GraphicsRenderer() {
        if (!loadTexture(boardTexture, {
            "../InteractiveBoard/assets/chessboard.png"
        })) {
            std::cerr << "RUNTIME ERROR: Could not find chessboard asset\n";
            std::exit(EXIT_FAILURE);
        }
        boardSprite.emplace(boardTexture);

        std::string colors[] = {"white", "black"};
        std::string types[] = {"king", "queen", "bishop", "knight", "rook", "pawn"};

        for (const auto& color : colors) {
            for (const auto& type : types) {
                std::string key = color + "-" + type;

                if (!loadTexture(pieceTextures[key], {
                    "../InteractiveBoard/assets/" + key + ".png"
                })) {
                    std::cerr << "RUNTIME ERROR: Failed to load asset: " << key << "\n";
                    std::exit(EXIT_FAILURE);
                }
            }
        }
    }

    void drawGame(sf::RenderWindow& window, const view::BoardSnapshot& pieces) {
        if (boardSprite) {
            window.draw(*boardSprite);
        }

        for (const auto& piece : pieces) {
            sf::Sprite spriteToDraw = getSpriteForPiece(piece.type, piece.color);
            spriteToDraw.setPosition({
                static_cast<float>(piece.col * TILE_SIZE) + 3.0f,
                static_cast<float>(piece.row * TILE_SIZE) + 5.0f
            });
            window.draw(spriteToDraw);
        }

        sf::Font boardFont;
        if (!loadFont(boardFont, {
            "../InteractiveBoard/Sidepanel/arial.ttf"
        })) {
            std::cerr << "RUNTIME ERROR: Could not find board marker font\n";
            return;
        }

        sf::Text rankFileText(boardFont);
        rankFileText.setCharacterSize(18);
        rankFileText.setFillColor(sf::Color(35, 35, 35));

        for (int c = 0; c < 8; ++c) {
            rankFileText.setString(static_cast<char>('a' + c));
            rankFileText.setPosition({
                static_cast<float>(c * TILE_SIZE) + 5.f,
                static_cast<float>(8 * TILE_SIZE) - 20.f
            });
            window.draw(rankFileText);
        }

        for (int r = 0; r < 8; ++r) {
            rankFileText.setString(static_cast<char>('8' - r));
            rankFileText.setPosition({
                static_cast<float>(8 * TILE_SIZE) - 15.f,
                static_cast<float>(r * TILE_SIZE) + 5.f
            });
            window.draw(rankFileText);
        }
    }

    sf::Sprite getSpriteForPiece(view::PieceType type, view::Color color) {
        std::string colorKey = (color == view::Color::White) ? "white" : "black";
        std::string typeKey;

        switch (type) {
            case view::PieceType::Queen:  typeKey = "queen"; break;
            case view::PieceType::King:   typeKey = "king"; break;
            case view::PieceType::Bishop: typeKey = "bishop"; break;
            case view::PieceType::Knight: typeKey = "knight"; break;
            case view::PieceType::Rook:   typeKey = "rook"; break;
            case view::PieceType::Pawn:   typeKey = "pawn"; break;
        }

        return sf::Sprite(pieceTextures[colorKey + "-" + typeKey]);
    }
};
