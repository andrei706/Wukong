//
// Created by Andrei on 12/7/2025.
//

#include "Enemy_Walker.h"

Enemy_Walker::Enemy_Walker(const std::string &name): Enemy(name) {
    Sprite.setFillColor(sf::Color::Green);
}

Enemy_Walker::Enemy_Walker(const Enemy_Walker &other): Enemy(other) {}

void Enemy_Walker::HandleActions(const sf::Vector2f &PlayerPosition, float deltaTime, float deltaTimeMultiplier) {
    sf::Vector2f direction = PlayerPosition - Sprite.getPosition();

    // float radians = std::atan2(direction.y, direction.x);
    // float angleDegrees = radians * 180.f / 3.14f;

    if (inAttack == false && getAttackReady == false) {
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (distance > 25.0f) {

            sf::Vector2f unitDirection = direction / distance;
            float speed = Stats.GetSpeed();

            if (Damaged) {
                speed /= 3.0f;
            }
            sf::Vector2f movement = unitDirection * speed * deltaTime * deltaTimeMultiplier;

            Sprite.move(movement);

            // Sprite.setRotation(sf::degrees(angleDegrees));
        }
        else {
            HandleMeleeAttack(1, direction, Weapon);
        }
    }
    HandleMeleeAttack(0, direction, Weapon);
}

std::shared_ptr<Enemy> Enemy_Walker::clone() const {
    return std::make_shared<Enemy_Walker>(*this);
}

Enemy_Walker & Enemy_Walker::operator=(Enemy_Walker other) {
    swap(*this, other);
    return *this;
}

void swap(Enemy_Walker &first, Enemy_Walker &second) noexcept {
    std::swap(static_cast<Enemy&>(first), static_cast<Enemy&>(second));
}
