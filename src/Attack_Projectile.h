#ifndef OOP_ATTACK_PROJECTILE_H
#define OOP_ATTACK_PROJECTILE_H

#include "Attack.h"

class Attack_Projectile : public Attack {
    sf::Vector2f Velocity;
    bool DestroyOnHit = false;
public:
    explicit Attack_Projectile(float DamageValue_,
                               sf::Vector2f Size_ = {100, 20},
                               sf::Vector2f Position_ = {0, 0},
                               sf::Angle Angle_ = sf::degrees(0),
                               sf::Vector2f Velocity_ = {0.f, 0.f},
                               float Lifetime_ = 5,
                               float Cooldown_ = 0.5f,
                               bool DestroyOnHit_ = true);

    void UpdateBehavior(float deltaTime) override;


    friend std::ostream & operator<<(std::ostream & out, const Attack_Projectile & object);

};

#endif