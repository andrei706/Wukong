

#ifndef OOP_PLAYER_CLASS_H
#define OOP_PLAYER_CLASS_H
#include "Character_Stats.h"
#include "Tool.h"
#include "Tool_Punch.h"
#include "Key_Manager.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "Tool_Ranged.h"

class Player_Class {
    int Experience, Gauge = 0;
    bool Invincibility = false, inAttack = false, inRangedAttack = false;
    float SpeedMultiplier = 0.f;
    Character_Stats Stats{500, 2, 0};
    //Tool_Punch Pole{"Pole", 5, 0.4f, 40, 2};
    Tool Pole{"Pole", 5, 0.7f, 40, 2};
    Tool_Ranged Blast{"Blast", 5, 0.5f, 1, 10, 30, 10, 2};

    sf::RectangleShape Sprite;
    sf::Vector2f Position = {100.f, 100.f};
    sf::Vector2f Size = {50.f, 50.f};
    sf::Angle Rotation;

    sf::Time InvincibilityTime = sf::seconds(1.0f), InAttackTime = sf::seconds(0.0f);
    sf::Clock ClockInvincibilityTime, AttackCooldown, RangedCooldown;
    std::vector<std::shared_ptr<Attack_Hitbox>> ActiveHitboxes;

    bool isDodging = false;
    sf::Vector2f DodgeDirection;
    float DodgeSpeed = 10.f;

    sf::Time DodgeDuration = sf::seconds(0.2f), DodgeCooldown = sf::seconds(1.4f);
    sf::Clock ClockDodgeDuration, ClockDodgeCooldown;

    void StartDodge(sf::Vector2f inputDirection);
    void UpdateInvincibility();
    void MakeInvincible(float Seconds);
    void HandleMovement(sf::RenderWindow &window, float deltaTime = 0.016, float deltaTimeMultiplier = 62.5);
    void HandleDodge(Key_Manager &keyManager);
    void ClearAttackHitboxes();
    float HandleAttack(Key_Manager& KeyManager);

public:
    explicit Player_Class(int Experience_ = 0, float InvincibilityTime_ = 1.0f);
    friend std::ostream & operator<<(std::ostream & out, const Player_Class & object);
    void ShowSprite(sf::RenderWindow& window) const;
    void SetPosition(sf::Vector2f position);
    sf::RectangleShape& GetSprite();
    sf::Vector2f GetPosition() const;
    float GetHealth() const;
    void AddExperience(int Value);
    void TakeDamage(float Value);
    void RestoreHealth(float Value);
    const std::vector<std::shared_ptr<Attack_Hitbox>>& GetHitboxes();
    void Update(sf::RenderWindow &window, float deltaTime, float deltaTimeMultiplier, Key_Manager &keyManager);


};


#endif //OOP_PLAYER_CLASS_H