
#ifndef OOP_ATTACK_WARNING_H
#define OOP_ATTACK_WARNING_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Attack_Warning {
    sf::RectangleShape Sprite;
    sf::Texture Texture;
    sf::Clock Timer;

    bool Visible;
    float VisibilityTime;
    std::string TexturePath;

public:
    explicit Attack_Warning(bool visible_ = false, float VisibilityTime_ = 0, const std::string &TexturePath_ = "data/textures/markers/attack_marker.png");

    void SetPosition(sf::Vector2f position_);

    void ShowSprite(sf::RenderWindow &window) const;

    void Update();

    void SetVisibility(bool value, float VisibilityTime_ = 0.5f);
};


#endif //OOP_ATTACK_WARNING_H