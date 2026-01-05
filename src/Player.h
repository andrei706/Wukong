

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

class Player {
    int Experience, Gauge = 50;
    bool Invincibility = false, inAttack = false, inRangedAttack = false, inRangedBallAttack = false;
    float SpeedMultiplier = 0.f;
    Character_Stats Stats{150, 2.5, 0};
    Tool Pole{"Pole", 5, 0.5f, 40, 2};
    Tool_Ranged Blast{"Blast", 5, 0.2f, 1, 10, 1, 1.0f, 30, 15, 4, true};
    Tool_Ranged SpiritBall{"SpiritBall", 3, 1.5f, 100, 10, 1, 0.5, 75,
    1.5f, 10, false};

    sf::RectangleShape Sprite;
    sf::Texture PlayerTexture;
    sf::Vector2f Position = {100.f, 100.f};
    sf::Vector2f Size = {50.f, 50.f};
    sf::Angle Rotation;

    //Pentru animatie
    sf::Vector2i frameSize = {50, 50}; // Dimensiunea unui frame
    int VisualID = 0;
    sf::Vector2i FrameSize = {50, 50};
    float AnimationTimer = 0.f;
    int WalkToggle = 0;
    bool FacingRight = true;

    sf::Time InvincibilityTime = sf::seconds(1.0f), InAttackTime = sf::seconds(0.0f);
    sf::Clock ClockInvincibilityTime, AttackCooldown, RangedCooldown, BallRangedCooldown;
    std::vector<std::shared_ptr<Attack>> ActiveHitboxes;

    bool isDodging = false;
    sf::Vector2f DodgeDirection;
    float DodgeSpeed = 12.f;

    sf::RectangleShape DashCooldownSprite;
    sf::Texture DashCooldownTexture;

    sf::Time DodgeDuration = sf::seconds(0.2f), DodgeCooldown = sf::seconds(1.4f);
    sf::Clock ClockDodgeDuration, ClockDodgeCooldown;

    void UpdateAnimation(float dt);
    void StartDodge(sf::Vector2f inputDirection);
    void UpdateInvincibility();
    void MakeInvincible(float Seconds);
    void HandleMovement(sf::RenderWindow &window, float deltaTime = 0.016, float deltaTimeMultiplier = 62.5);
    void HandleDodge(Key_Manager &keyManager);
    float HandleAttack(Key_Manager& KeyManager);
    void UpgradeWeapon(const std::string& Name, float Bonus_Damage, float Damage_Multiplier);
    void UpdateDashSprite();

public:
    explicit Player(int Experience_ = 0, float InvincibilityTime_ = 1.0f);
    friend std::ostream & operator<<(std::ostream & out, const Player & object);
    void ShowSprite(sf::RenderWindow& window) const;
    void SetPosition(sf::Vector2f position);
    sf::RectangleShape& GetSprite();
    sf::Vector2f GetPosition() const;
    void Restart();
    void PauseClocks(bool Pause = true);
    float GetHealth() const;
    int GetGauge() const;
    void AddExperience(int Value);
    int GetExperience() const;
    bool TakeDamage(float Value);
    void RestoreHealth(float Value);
    const std::vector<std::shared_ptr<Attack>>& GetHitboxes();
    void ApplyUpgrades(const std::vector<std::pair<std::string, float>>& upgrades);
    void Update(sf::RenderWindow &window, float deltaTime, float deltaTimeMultiplier, Key_Manager &keyManager);
};


#endif //OOP_PLAYER_CLASS_H