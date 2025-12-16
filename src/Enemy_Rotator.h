
#ifndef OOP_ENEMY_ROTATOR_H
#define OOP_ENEMY_ROTATOR_H

#include "Enemy.h"

class Enemy_Rotator : public Enemy{
    float RotationSpeed;
    bool isRotatingRight = false;
public:
    explicit Enemy_Rotator(const std::string &name, float RotationSpeed_ = 30.0f, bool isRotatingRight = false);

    Enemy_Rotator(const Enemy_Rotator& other);

    std::shared_ptr<Enemy> clone() const override;

    Enemy_Rotator& operator=(Enemy_Rotator other);

    friend void swap(Enemy_Rotator& first, Enemy_Rotator& second) noexcept;

    void HandleActions(const sf::Vector2f &PlayerPosition, float deltaTime, float deltaTimeMultiplier) override;
};


#endif //OOP_ENEMY_ROTATOR_H