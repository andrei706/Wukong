

#ifndef OOP_ENEMY_AGILEBOSS_H
#define OOP_ENEMY_AGILEBOSS_H

#include "Enemy.h"

class Enemy_AgileBoss : public Enemy {
    float TargetX = 200.0f;
    bool isMeleePhase = true;
    bool isRangedPhase = false;
    bool nextTargetRight = true;
    int verticalDirection = 1;
    float phaseTimer = 0.0f;

protected:
    void HandleActions(const sf::Vector2f &PlayerPosition, float deltaTime, float deltaTimeMultiplier) override;

public:
    explicit Enemy_AgileBoss(const std::string &name);
    Enemy_AgileBoss(const Enemy_AgileBoss& other);
    std::shared_ptr<Enemy> clone() const override;
    Enemy_AgileBoss& operator=(Enemy_AgileBoss other);
    friend void swap(Enemy_AgileBoss& first, Enemy_AgileBoss& second) noexcept;
};


#endif //OOP_ENEMY_AGILEBOSS_H