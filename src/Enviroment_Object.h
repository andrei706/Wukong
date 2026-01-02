#ifndef OOP_ENVIROMENT_OBJECT_H
#define OOP_ENVIROMENT_OBJECT_H

#include "Game_Exceptions.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Enviroment_Object {
    sf::Texture Texture;
    sf::RectangleShape Sprite;

public:
    Enviroment_Object(sf::Vector2f pos, sf::Vector2f size,
                      const std::string& texturePath);

    void ShowSprite(sf::RenderWindow& window) const;

    friend std::ostream & operator<<(std::ostream & out, const Enviroment_Object & object);
};

#endif