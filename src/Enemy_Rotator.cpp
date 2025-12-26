
#include "Enemy_Rotator.h"

Enemy_Rotator::Enemy_Rotator(const std::string &name, float RotationSpeed_, bool isRotatingRight)
    : Enemy(name), RotationSpeed(RotationSpeed_), isRotatingRight(isRotatingRight) {}

Enemy_Rotator::Enemy_Rotator(const Enemy_Rotator& other) : Enemy(other) {
    this->RotationSpeed = other.RotationSpeed;
    this->isRotatingRight = other.isRotatingRight;
}

Enemy_Rotator& Enemy_Rotator::operator=(Enemy_Rotator other) {
    swap(*this, other);
    return *this;
}

void Enemy_Rotator::SwapRotation() {
    isRotatingRight = !isRotatingRight;
}

void Enemy_Rotator::HandleActions(const sf::Vector2f &PlayerPosition, float deltaTime, float deltaTimeMultiplier) {
    sf::Vector2f PlayerDirection = PlayerPosition - Sprite.getPosition();
    float distance = std::sqrt(PlayerDirection.x * PlayerDirection.x + PlayerDirection.y * PlayerDirection.y);

    if (distance < 1500.0f) {
        float angleChange = RotationSpeed * deltaTime * deltaTimeMultiplier;

        if (isRotatingRight) {
            Sprite.rotate(sf::degrees(angleChange));
        } else {
            Sprite.rotate(sf::degrees(-angleChange));
        }

        float angleRadians = Sprite.getRotation().asRadians();

        sf::Vector2f direction(std::cos(angleRadians), std::sin(angleRadians));
        HandleRangedAttack(true, direction);
    }
}

void swap(Enemy_Rotator& first, Enemy_Rotator& second) noexcept {
    using std::swap;
    swap(static_cast<Enemy&>(first), static_cast<Enemy&>(second));
    swap(first.RotationSpeed, second.RotationSpeed);
    swap(first.isRotatingRight, second.isRotatingRight);
}

std::shared_ptr<Enemy> Enemy_Rotator::clone() const {
    return std::make_shared<Enemy_Rotator>(*this);
}
