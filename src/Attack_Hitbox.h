
#ifndef OOP_ATTACK_HITBOX_H
#define OOP_ATTACK_HITBOX_H
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>


class Attack_Hitbox {
    float DamageValue;

    sf::RectangleShape Sprite;
    sf::Vector2f Size = {100, 20};
    sf::Vector2f Position = {100.f, 100.f};
    sf::Angle Rotation;

public:
    explicit Attack_Hitbox(float DamageValue_, sf::Vector2f Size_ = {100, 20}, sf::Vector2f Position_ = {0, 0}, sf::Angle Angle_ = sf::degrees(0));

    friend std::ostream & operator<<(std::ostream & out, const Attack_Hitbox & object);

    float GetDamageValue() const;

    sf::FloatRect GetBounds() const;

    void ShowSprite(sf::RenderWindow& window) const;
};


#endif //OOP_ATTACK_HITBOX_H