

#ifndef OOP_PLAYER_CLASS_H
#define OOP_PLAYER_CLASS_H
#include "Character_Stats.h"
#include "Tool.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Player_Class {
    int Experience, Gauge = 0;
    bool Invincibility = false, inAttack = false;
    Character_Stats Stats{30, 2, 0};
    Tool Pole{"Pole", 5, 0.4f, 40, 2};

    sf::RectangleShape Sprite;
    sf::Vector2f Position = {100.f, 100.f};
    sf::Vector2f Size = {50.f, 50.f};
    sf::Angle Rotation;

    sf::Time InvincibilityTime = sf::seconds(1.0f);
    sf::Clock ClockInvincibilityTime;

private:
    bool MakeInvincibile();

public:
    explicit Player_Class(int Experience_ = 0, float InvincibilityTime_ = 1.0f);
    friend std::ostream & operator<<(std::ostream & out, const Player_Class & object);
    void ShowSprite(sf::RenderWindow& window) const;
    sf::RectangleShape& GetSprite();
    sf::Vector2f GetPosition() const;
    float GetHealth() const;
    void AddExperience(int Value);
    void TakeDamage(float Value);
    std::vector<Attack_Hitbox> GetHitboxes() const;
    void ClearAttackHitboxes();
    float HandleAttack();
    void HandleMovement(sf::RenderWindow &window, float deltaTime = 0.016, float deltaTimeMultiplier = 62.5);
};


#endif //OOP_PLAYER_CLASS_H