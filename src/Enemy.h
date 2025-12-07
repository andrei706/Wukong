

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
    Attack_Warning AttackWarning;

    static int id;
    int LocalId;

protected:
    std::shared_ptr<Tool> Weapon = std::make_shared<Tool>("Sword", 3.0f, 0.6f, 10.0f);

    bool Damaged = false; //Daca a si-a primit dmg de la atacul player-ului sau nu ca sa nu-si primeasca in continuu dmg
    bool inAttack = false;
    Character_Stats Stats{10, 5, 5 };
    bool getAttackReady = false;
    int Experience = 5;
    float DamagedTimer = 2.0f;

    sf::RectangleShape Sprite;
    sf::Vector2f Position = {200.f, 100.f};
    sf::Clock ActionClock, AttackWarningClock, CooldownClock, DamagedClock;

    void HandleMeleeAttack(bool canAttack, sf::Vector2f direction, std::shared_ptr<Tool> UsedWeapon);
    //void HandleRangedAttack(bool canAttack, sf::Vector2f direction, std::shared_ptr<Tool> UsedWeapon, int Burst);
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

    const std::vector<std::shared_ptr<Attack_Hitbox>>& GetHitboxes();

    void ChangeDamagedStatus(bool Value = true, float Seconds = 0.5f);

    bool GetDamagedStatus() const;

    //int GetLocalId() const;

    int GetExperience() const;

    bool TakeDamage (float Damage_Points);

    void AssignWeapon (const std::shared_ptr<Tool>& toolPtr);

    void AssignStats (const Character_Stats& other);

    void SetPosition(float x, float y);

    void ShowSprite(sf::RenderWindow& window) const;

    void RenderHitboxes(sf::RenderWindow& window) const;

    void Update(const sf::Vector2f& PlayerPosition, float deltaTime, float deltaTimeMultiplier);

    void DisplayInfo(std::ostream &out) const {
        out<<std::endl << "Enemy Name: " << Name << std::endl
                << "Weapon: " << *Weapon << std::endl
                << "Stats: " << Stats << std::endl
                << "ID: " << LocalId << std::endl;
    }

    // static void InitializeID() {
    //     Enemy::id = 0;
    // }
    //
    // static void IncreaseID() {
    //     Enemy::id++;
    //     if (Enemy::id == 2001) {
    //         Enemy::id = 0;
    //     }
    // }


};


#endif //OOP_ENEMY_H