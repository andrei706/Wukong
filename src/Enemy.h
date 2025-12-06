

#ifndef OOP_ENEMY_H
#define OOP_ENEMY_H

#include "Character_Stats.h"
#include "Tool.h"
#include "Attack_Warning.h"

#include <string>
#include <memory>
#include <utility>


class Enemy {
    std::string Name;
    Character_Stats Stats{10, 5, 5 };
    Attack_Warning AttackWarning;

protected:
    std::shared_ptr<Tool> Weapon = std::make_shared<Tool>("Sword", 3.0f, 0.6f, 10.0f);


    bool Damaged = false; //Daca a si-a primit dmg de la atacul player-ului sau nu ca sa nu-si primeasca in continuu dmg
    bool inAttack = false;
    bool getAttackReady = false;
    int Experience = 5;
    float DamagedTimer = 2.0f;

    sf::RectangleShape Sprite;
    sf::Vector2f Position = {200.f, 100.f};
    sf::Clock ActionClock, AttackWarningClock, CooldownClock, DamagedClock;

    void HandleMeleeAttack(bool canAttack, sf::Vector2f direction);
    virtual void HandleActions(const sf::Vector2f& PlayerPosition, float deltaTime, float deltaTimeMultiplier);
public:

    explicit Enemy(const std::string& name_);

    Enemy(const Enemy& other);

    virtual ~Enemy();

    friend std::ostream & operator<<(std::ostream & out, const Enemy & object);

    virtual std::shared_ptr<Enemy> clone() const;

    friend void swap(Enemy& first, Enemy& second) noexcept;

    Enemy& operator=(Enemy other);

    sf::FloatRect GetEnemyHitbox();

    void ChangeDamagedStatus(bool Value = true, float Seconds = 0.5f);

    bool GetDamagedStatus() const;

    float GetDamage() const;

    int GetExperience() const;

    bool TakeDamage (float Damage_Points);

    void AssignWeapon (const std::shared_ptr<Tool>& toolPtr);

    void AssignStats (const Character_Stats& other);

    void SetPosition(float x, float y);

    void ShowSprite(sf::RenderWindow& window) const;

    void RenderHitboxes(sf::RenderWindow& window) const;

    void Update(const sf::Vector2f& PlayerPosition, float deltaTime, float deltaTimeMultiplier);




};


#endif //OOP_ENEMY_H