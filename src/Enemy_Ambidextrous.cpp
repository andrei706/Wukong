
#include "Enemy_Ambidextrous.h"

void Enemy_Ambidextrous::HandleActions(const sf::Vector2f &PlayerPosition, float deltaTime, float deltaTimeMultiplier) {
    sf::Vector2f direction = PlayerPosition - Sprite.getPosition();
    float distance = std::sqrt(std::pow(direction.x, 2) + std::pow(direction.y, 2));
    sf::Vector2f unitDir = (distance > 0) ? direction / distance : sf::Vector2f(0, 0);

    if (distance < 100.0f) {
        isMeleeMode = true;
    } else if (distance > 100.0f) {
        isMeleeMode = false;
    }

    if (inAttack == false && getAttackReady == false) {
        if (isMeleeMode) {
            IntendedMovement = unitDir * Stats.GetSpeed() * deltaTime * deltaTimeMultiplier;
            if (distance < 30.0f) HandleMeleeAttack(1, direction);
        }
        else {
            if (distance > 250.0f) {
                IntendedMovement = unitDir * Stats.GetSpeed() * deltaTime * deltaTimeMultiplier;
            }
            else if (distance < 150.0f) {
                IntendedMovement = -unitDir * Stats.GetSpeed() * deltaTime * deltaTimeMultiplier;
            }
            else {
                srand(time(NULL));
                strafeTimer += deltaTime;
                if (strafeTimer >= 1.0f) {
                    if (rand() % 100 < 40) {
                        strafeDirection *= -1;
                    }
                    strafeTimer = 0.0f;
                }

                sf::Vector2f strafeDir(-unitDir.y * (float)strafeDirection, unitDir.x * (float)strafeDirection);
                IntendedMovement = strafeDir * (Stats.GetSpeed() * 0.8f) * deltaTime * deltaTimeMultiplier;
            }
            HandleRangedAttack(true, direction);
        }
    }
    HandleMeleeAttack(0, direction);
}

Enemy_Ambidextrous::Enemy_Ambidextrous(const std::string &name): Enemy(name) {
    Sprite.setFillColor(sf::Color::Magenta);
}

Enemy_Ambidextrous::Enemy_Ambidextrous(const Enemy_Ambidextrous& other) : Enemy(other) {
    this->isMeleeMode = other.isMeleeMode;
}

Enemy_Ambidextrous& Enemy_Ambidextrous::operator=(Enemy_Ambidextrous other) {
    swap(*this, other);
    return *this;
}

std::shared_ptr<Enemy> Enemy_Ambidextrous::clone() const {
    return std::make_shared<Enemy_Ambidextrous>(*this);
}

void swap(Enemy_Ambidextrous& first, Enemy_Ambidextrous& second) noexcept {
    using std::swap;
    swap(static_cast<Enemy&>(first), static_cast<Enemy&>(second));
    swap(first.isMeleeMode, second.isMeleeMode);
}