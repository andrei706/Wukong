

#include "Enemy_AgileBoss.h"

Enemy_AgileBoss::Enemy_AgileBoss(const std::string &name) : Enemy(name) {}

std::shared_ptr<Enemy> Enemy_AgileBoss::clone() const {
    return std::make_shared<Enemy_AgileBoss>(*this);
}

void Enemy_AgileBoss::HandleActions(const sf::Vector2f &PlayerPosition, float deltaTime, float deltaTimeMultiplier) {
    sf::Vector2f myPos = Sprite.getPosition();
    sf::Vector2f direction = PlayerPosition - myPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float speed = Stats.GetSpeed();
    sf::Vector2f movement(0.f, 0.f);

    phaseTimer += deltaTime;
    const float maxMeleeTime = 4.0f;

    if (isMeleePhase) {
        sf::Vector2f unitDir = direction / distance;
        sf::Vector2f perpendicularDir(-unitDir.y, unitDir.x);
        float strafeSide = (std::sin(phaseTimer * 3.0f) > 0) ? 1.0f : -1.0f;

        movement = (unitDir + perpendicularDir * strafeSide * 0.7f) * speed;

        if (distance < 70.0f && distance > 35.0f) {
            HandleMeleeAttack(1, direction);
        }

        if (phaseTimer > maxMeleeTime) {
            isMeleePhase = false;
            phaseTimer = 0.0f;

            if (nextTargetRight) {
                TargetX = 1100.0f;
            } else {
                TargetX = 100.0f;
            }
            nextTargetRight = !nextTargetRight;
        }
    }
    else {
        if (myPos.y < 120.0f) verticalDirection = 1;
        if (myPos.y > 580.0f) verticalDirection = -1;
        movement.y = verticalDirection * speed * 0.9f;

        float horizontalDiff = myPos.x - TargetX;
        if (std::abs(horizontalDiff) > 50.0f) {
            float dirX = (TargetX > myPos.x) ? 1.0f : -1.0f;
            movement.x = dirX * speed * 1.8f;
        }
        else {
            //shimy = miscare de vibratie/oscilatie
            //ca sa se duca "stanga/dreapta" cat timp e in ranged phase
            float shimmy = std::cos(phaseTimer * 5.0f) * 40.0f;
            float targetWithShimmy = TargetX + shimmy;
            float dirX = (targetWithShimmy > myPos.x) ? 1.0f : -1.0f;
            movement.x = dirX * speed * 0.6f;

            if (distance > 60.0f && !isRangedAttacking) {
                HandleRangedAttack(1, direction, 0.1f);
            }
        }

        if (phaseTimer > 8.0f && !isRangedAttacking) {
            isMeleePhase = true;
            phaseTimer = 0.0f;
        }
    }

    HandleMeleeAttack(0, direction);
    IntendedMovement = movement * deltaTime * deltaTimeMultiplier;
}

Enemy_AgileBoss::Enemy_AgileBoss(const Enemy_AgileBoss &other)
    : Enemy(other), TargetX(other.TargetX), isMeleePhase(other.isMeleePhase), isRangedPhase(other.isRangedPhase) {}

Enemy_AgileBoss & Enemy_AgileBoss::operator=(Enemy_AgileBoss other) {
    swap(*this, other);
    return *this;
}

void swap(Enemy_AgileBoss &first, Enemy_AgileBoss &second) noexcept {
    using std::swap;
    swap(static_cast<Enemy&>(first), static_cast<Enemy&>(second));
    swap(first.TargetX, second.TargetX);
    swap(first.isMeleePhase, second.isMeleePhase);
    swap(first.isRangedPhase, second.isRangedPhase);
}