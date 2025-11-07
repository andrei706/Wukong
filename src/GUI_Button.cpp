//
// Created by dismi on 11/7/2025.
//

#include "GUI_Button.h"

GUI_Button::GUI_Button(std::string Name_, const std::string &TexturePath_, sf::Vector2f InitialPosition, sf::Vector2f InitialSize)
    : Name(std::move(Name_)), Texture(TexturePath_) {
    if (!Texture.loadFromFile(TexturePath_)) {
        std::cerr << "Error: Could not load texture from " << TexturePath_ << std::endl;
    }

    Sprite.setTexture(&Texture); // texture is a sf::Texture
    Sprite.setTextureRect(sf::IntRect({0, 0}, {200, 80}));
    Sprite.setPosition(InitialPosition);
    Sprite.setSize(InitialSize);

    InitialSize.x = (float)Texture.getSize().x;
    InitialSize.y = (float)Texture.getSize().y;
}

const std::string& GUI_Button::GetName() const {
    return Name;
}


void GUI_Button::ShowSprite(sf::RenderWindow &window) const {
    window.draw(Sprite);
}

sf::FloatRect GUI_Button::getBounds() const {
    return Sprite.getGlobalBounds();
}

bool GUI_Button::isClicked(sf::Vector2i mousePos, Key_Manager & KeyManager) {
    sf::Vector2f floatMousePos = static_cast<sf::Vector2f>(mousePos); //Converteste din sf::vector2i in sf::vector2f pentru verificare
    if (getBounds().contains(floatMousePos)) {
        Sprite.setFillColor(sf::Color(128, 128, 128));
    }
    else
        Sprite.setFillColor(sf::Color(255, 255, 255));
    if ( getBounds().contains(floatMousePos) && KeyManager.CheckInput("LeftMouseButton")) {
        Sprite.setFillColor(sf::Color(255, 255, 255));
        return true;
    }
    return false;
}
