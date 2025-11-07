

#include "Enemy.h"

Enemy::Enemy(const std::string &name_): Name(name_) {
    Sprite.setFillColor(sf::Color::Red);
    Sprite.setPosition(Position);
    Sprite.setSize({50, 50});
    Sprite.setOrigin({25, 25});
    ActionClock.start();
}

Enemy::Enemy(const Enemy &other)
    : Name(other.Name), Stats(other.Stats), Weapon(other.Weapon), Sprite(other.Sprite) {}

Enemy::~Enemy() {
    std::cout << Name << " Destroyed\n";
}

Enemy & Enemy::operator=(const Enemy &other) {
    Name = other.Name;
    Stats = other.Stats;
    Weapon = other.Weapon;
    Sprite = other.Sprite;
    Experience = other.Experience;
    return *this;
}

sf::FloatRect Enemy::GetEnemyHitbox() {
    return Sprite.getGlobalBounds();
}

void Enemy::ChangeDamagedStatus() {
    Damaged = !Damaged;
}

bool Enemy::GetDamagedStatus() const {
    return Damaged;
}

float Enemy::GetDamage() const {
    return Weapon.DamageCalculation();
}

int Enemy::GetExperience() const {
    return Experience;
}

bool Enemy::TakeDamage(float Damage_Points) {
    return Stats.ReduceHealth(Damage_Points);
}

void Enemy::AssignWeapon(const Tool &other) {
    Weapon = other;
}

void Enemy::AssignStats(const Character_Stats &other) {
    Stats = other;
}

void Enemy::SetPosition(float x, float y) {
    Position.x = x;
    Position.y = y;
    Sprite.setPosition(Position);
}

void Enemy::ShowSprite(sf::RenderWindow &window) const {
    window.draw(Sprite);
}

// void Enemy::RenderHitboxes(sf::RenderWindow &window) const {
//     Weapon.ShowHitboxes(window);
// }

void Enemy::HandleMovement(const sf::Vector2f &PlayerPosition, float deltaTime, float deltaTimeMultiplier) {
    sf::Vector2f direction = PlayerPosition - Sprite.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    // float radians = std::atan2(direction.y, direction.x);
    // float angleDegrees = radians * 180.f / 3.14f;

    if (distance > 25.0f) {

        sf::Vector2f unitDirection = direction / distance;
        float speed = Stats.GetSpeed();

        if (Damaged) {
            speed /= 10.0f;
        }
        sf::Vector2f movement = unitDirection * speed * deltaTime * deltaTimeMultiplier;

        Sprite.move(movement);

        // Sprite.setRotation(sf::degrees(angleDegrees));
    }
    else {

        if (ActionClock.getElapsedTime() > sf::seconds(Weapon.GetCooldown())) {
            float radians = std::atan2(direction.y, direction.x);
            float angleDegrees = radians * 180.f / 3.14f;
            Weapon.Attack(Sprite, sf::degrees(angleDegrees));
            ActionClock.restart();
        }
    }

}

std::ostream & operator<<(std::ostream &out, const Enemy &object) {
    out<<object.Name<<"\n"<<object.Stats<<"\n"<<object.Weapon;
    return out;
}
