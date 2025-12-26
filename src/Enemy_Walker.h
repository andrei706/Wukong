
#ifndef OOP_ENEMY_WALKER_H
#define OOP_ENEMY_WALKER_H

#include "Enemy.h"

class Enemy_Walker : public Enemy {
protected:
    void HandleActions(const sf::Vector2f &PlayerPosition, float deltaTime, float deltaTimeMultiplier) override;
public:
    explicit Enemy_Walker(const std::string & name);

    Enemy_Walker(const Enemy_Walker &other);

    Enemy_Walker& operator=(Enemy_Walker other);

    friend void swap(Enemy_Walker& first, Enemy_Walker& second) noexcept;

    std::shared_ptr<Enemy> clone() const override;
};


#endif //OOP_ENEMY_WALKER_H