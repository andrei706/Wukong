
#ifndef OOP_ENEMY_AMBIDEXTROUS_H
#define OOP_ENEMY_AMBIDEXTROUS_H

#include "Enemy.h"

class Enemy_Ambidextrous : public Enemy{
    bool isMeleeMode = false;
    float strafeTimer = 0.0f;
    int strafeDirection = 1;
protected:
    void HandleActions(const sf::Vector2f &PlayerPosition, float deltaTime, float deltaTimeMultiplier) override;

public:
    explicit Enemy_Ambidextrous(const std::string& name);

    Enemy_Ambidextrous(const Enemy_Ambidextrous& other);
    Enemy_Ambidextrous& operator=(Enemy_Ambidextrous other);
    [[nodiscard]] std::shared_ptr<Enemy> clone() const override;
    friend void swap(Enemy_Ambidextrous& first, Enemy_Ambidextrous& second) noexcept;
};


#endif //OOP_ENEMY_AMBIDEXTROUS_H