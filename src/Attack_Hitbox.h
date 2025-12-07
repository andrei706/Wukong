
#ifndef OOP_ATTACK_HITBOX_H
#define OOP_ATTACK_HITBOX_H
#include <iostream>
#include <utility>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>


class Attack_Hitbox {
protected:
    float DamageValue;

    sf::RectangleShape Sprite;
    sf::Vector2f Size = {100, 20};
    sf::Vector2f Position = {100.f, 100.f};
    sf::Angle Rotation;
    sf::Texture Texture;

    float Lifetime;

    std::vector<std::pair<int, int>> Attacked;

    bool canDamage = true;
    bool isActive = true;
public:
    explicit Attack_Hitbox(float DamageValue_, sf::Vector2f Size_ = {100, 20}, sf::Vector2f Position_ = {0, 0}, sf::Angle Angle_ = sf::degrees(0)
        , float Lifetime_ = 0.1, std::string TexturePath = "data/textures/attack_assets/slash_sprite.png");

    virtual ~Attack_Hitbox() = default;

    friend std::ostream & operator<<(std::ostream & out, const Attack_Hitbox & object);

    float GetDamageValue(int EnemyId);

    sf::FloatRect GetBounds() const;

    void ShowSprite(sf::RenderWindow& window) const;

    virtual void UpdateBehavior(float deltaTime);

    bool IsActive() const {
        return isActive;
    }

    bool Update(float deltaTime) {
        UpdateBehavior(deltaTime);
        return isActive;
    }
};


#endif //OOP_ATTACK_HITBOX_H