//
// Created by dismi on 11/7/2025.
//

#include "GUI_Button.h"

GUI_Button::GUI_Button(sf::Sprite Sprite_, std::string Name_, const std::string &TexturePath_): Name(std::move(Name_)), Texture(TexturePath_), Sprite(std::move(Sprite_)) {
    if (!Texture.loadFromFile(TexturePath_)) {
        std::cerr << "Error: Could not load texture from " << TexturePath_ << std::endl;
    }

    Sprite.setTexture(Texture);
    Sprite.setPosition(InitialPosition);

    InitialSize.x = (float)Texture.getSize().x;
    InitialSize.y = (float)Texture.getSize().y*2;

    Sprite.setScale({4.f, 2.f});
}

void GUI_Button::ShowSprite(sf::RenderWindow &window) const {
    window.draw(Sprite);
}

sf::FloatRect GUI_Button::getBounds() const {
    return Sprite.getGlobalBounds();
}

bool GUI_Button::isClicked(sf::Vector2i mousePos, Key_Manager KeyManager) const {
    sf::Vector2f floatMousePos = static_cast<sf::Vector2f>(mousePos); //Converteste din sf::vector2i in sf::vector2f pentru verificare
    return getBounds().contains(floatMousePos) && KeyManager.CheckInput("LeftMouseButton");
}
