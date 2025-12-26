
#ifndef OOP_ENEMY_RANGER_H
#define OOP_ENEMY_RANGER_H

#include "Enemy.h"

class Enemy_Ranger : public Enemy {
    float TargetX = 200.0f;
    int VerticalDirection = 1;
    bool isFleeing = false;
    //sf::Clock RangedWeaponClock;
protected:
    void HandleActions(const sf::Vector2f &PlayerPosition, float deltaTime, float deltaTimeMultiplier) override;

public:
    explicit Enemy_Ranger(const std::string &name);

    Enemy_Ranger(const Enemy_Ranger& other);

    std::shared_ptr<Enemy> clone() const override;

    Enemy_Ranger& operator=(Enemy_Ranger other);

    friend void swap(Enemy_Ranger& first, Enemy_Ranger& second) noexcept;

};


#endif //OOP_ENEMY_RANGER_H