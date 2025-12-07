
#include "Enemy_Ranger.h"

Enemy_Ranger::Enemy_Ranger(const std::string &name): Enemy(name) {
    Sprite.setFillColor(sf::Color::Yellow);
}

std::shared_ptr<Enemy> Enemy_Ranger::clone() const {
    return std::make_shared<Enemy_Ranger>(*this);
}

void Enemy_Ranger::HandleActions(const sf::Vector2f &PlayerPosition, float deltaTime, float deltaTimeMultiplier) {
    sf::Vector2f direction = PlayerPosition - Sprite.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Calculăm viteza standard
    float speed = Stats.GetSpeed();
    if (Damaged) {
        speed /= 2.0f;
    }

    // Logica de patrulare și fugă
    if (getAttackReady == false) {

        if (distance < 150.0f && !isFleeing) {
            isFleeing = true;

            if (TargetX < 640.0f) {
                TargetX = 1000.0f;
            } else {
                TargetX = 200.0f;
            }
        }

        if (std::abs(Sprite.getPosition().x - TargetX) < 10.0f) {
            isFleeing = false;
        }

        sf::Vector2f movement(0.f, 0.f);

        // A. Mișcarea Orizontală (către TargetX)
        if (std::abs(Sprite.getPosition().x - TargetX) > 5.0f) {
            float dirX = (TargetX > Sprite.getPosition().x) ? 1.0f : -1.0f;
            movement.x = dirX * speed * (isFleeing ? 1.5f : 1.0f);
        }

        if (Sprite.getPosition().y > 600.0f) VerticalDirection = -1;
        if (Sprite.getPosition().y < 100.0f) VerticalDirection = 1;

        if (inAttack) {
            speed /= 2.0f;
        }

        movement.y = VerticalDirection * speed;

        Sprite.move(movement * deltaTime * deltaTimeMultiplier);
    }
    if (!isFleeing)
        HandleMeleeAttack(1, direction, Weapon);
}

Enemy_Ranger & Enemy_Ranger::operator=(Enemy_Ranger other) {
    swap(*this, other);
    return *this;
}

Enemy_Ranger::Enemy_Ranger(const Enemy_Ranger &other)
    : Enemy(other), // Apelam Copy Ctor-ul din baza!
      TargetX(other.TargetX),
      VerticalDirection(other.VerticalDirection),
      isFleeing(other.isFleeing)
{}

void swap(Enemy_Ranger &first, Enemy_Ranger &second) noexcept {
    using std::swap;
    swap(static_cast<Enemy&>(first), static_cast<Enemy&>(second));
    swap(first.TargetX, second.TargetX);
    swap(first.VerticalDirection, second.VerticalDirection);
    swap(first.isFleeing, second.isFleeing);
}
