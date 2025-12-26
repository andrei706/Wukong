

#ifndef OOP_ENEMY_H
#define OOP_ENEMY_H

#include "Character_Stats.h"
#include "Tool.h"
#include "Attack_Warning.h"
#include "Tool_Ranged.h"
#include "Game_Exceptions.h"

#include <string>
#include <memory>
#include <utility>


class Enemy {
    std::string Name;
    Attack_Warning AttackWarning;

    static int id;
    int LocalId;

    std::vector<std::shared_ptr<Attack_Hitbox>> ActiveHitboxes;

    void MoveSafely(const std::vector<std::shared_ptr<Enemy>>& otherEnemies);

protected:
    std::shared_ptr<Tool> MeleeWeapon = std::make_shared<Tool>("Sword", 3.0f, 0.6f, 10.0f);
    std::shared_ptr<Tool> RangedWeapon = std::make_shared<Tool_Ranged>("Blast", 3.0f, 0.6f, 10.0f);

    bool Damaged = false; //Daca a si-a primit dmg de la atacul player-ului sau nu ca sa nu-si primeasca in continuu dmg
    bool inAttack = false;
    Character_Stats Stats{10, 5, 5 };
    bool getAttackReady = false;
    int Experience = 5;
    float DamagedTimer = 2.0f;

    sf::RectangleShape Sprite;
    sf::Vector2f Position = {200.f, 100.f};
    sf::Clock ActionClock, AttackWarningClock, CooldownClock, DamagedClock;

    sf::Vector2f IntendedMovement;

    void HandleMeleeAttack(bool canAttack, sf::Vector2f direction);
    void HandleRangedAttack(bool canAttack, sf::Vector2f direction);
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

    std::string GetName() const {
        return Name;
    }

    bool GetDamagedStatus() const;

    int GetLocalId() const;

    int GetExperience() const;

    sf::Vector2f GetPosition() const;

    bool TakeDamage (float Damage_Points);

    void AssignWeapon (const std::shared_ptr<Tool>& toolPtr, bool Melee = true);

    void AssignStats (const Character_Stats& other);

    void SetPosition(float x, float y);

    void ShowSprite(sf::RenderWindow& window) const;

    void RenderHitboxes(sf::RenderWindow& window) const;

    void Update(const sf::Vector2f& PlayerPosition, float deltaTime, float deltaTimeMultiplier, const std::vector<std::shared_ptr<Enemy>>& otherEnemies);

    void DisplayInfo(std::ostream &out) const;

    static void AssignID(const std::shared_ptr<Enemy> &enemy);
};


#endif //OOP_ENEMY_H