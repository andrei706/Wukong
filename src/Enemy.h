

#ifndef OOP_ENEMY_H
#define OOP_ENEMY_H

#include "Character_Stats.h"
#include "Tool.h"

#include <string>


class Enemy {
    std::string Name;
    Character_Stats Stats{10, 5, 5 };
    Tool Weapon{"Sword", 3, 0.6f, 10};
    bool Damaged = false; //Daca a si-a primit damage de la atacul player-ului cu sabia sau nu
    int Experience = 5;

    sf::RectangleShape Sprite;
    sf::Vector2f Position = {200.f, 100.f};
public:

    explicit Enemy(const std::string& name_);

    Enemy(const Enemy& other);

    ~Enemy();

    friend std::ostream & operator<<(std::ostream & out, const Enemy & object);

    Enemy& operator=(const Enemy& other);

    sf::FloatRect GetEnemyHitbox();

    void ChangeDamagedStatus();

    bool GetDamagedStatus() const;

    float GetDamage() const;

    int GetExperience() const;

    bool TakeDamage (float Damage_Points);

    void AssignWeapon (const Tool& other);

    void AssignStats (const Character_Stats& other);

    void SetPosition(float x, float y);

    void ShowSprite(sf::RenderWindow& window) const;


    void HandleMovement(const sf::Vector2f& PlayerPosition, float deltaTime, float deltaTimeMultiplier);
};


#endif //OOP_ENEMY_H