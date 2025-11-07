
#ifndef OOP_GUI_BUTTON_H
#define OOP_GUI_BUTTON_H

#include "Key_Manager.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <utility>

class GUI_Button {
    std::string Name;
    sf::Texture Texture;
    sf::RectangleShape Sprite;
    sf::Vector2f InitialPosition = {100.f, 100.f};
    sf::Vector2f InitialSize;

public:
    // Constructor takes string by value (cleaner for small strings)
    explicit GUI_Button(
        std::string Name_ = "ButtonGUI",
        const std::string& TexturePath_ = "data/textures/buttons/default_texture.jpeg",
        sf::Vector2f InitialPosition_ = {500.f, 500.f},
        sf::Vector2f InitialSize_ = {200.f, 80.f}
        );

    std::string const GetName() const;

    void ShowSprite(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;

    bool isClicked(sf::Vector2i mousePos, Key_Manager & KeyManager);
};


#endif //OOP_GUI_BUTTON_H