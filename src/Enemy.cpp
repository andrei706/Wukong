

#include "Enemy.h"

Enemy::Enemy(const std::string &name_): Name(name_) {
    Sprite.setFillColor(sf::Color::Red);
    Sprite.setPosition(Position);
    Sprite.setSize({50, 50});
    Sprite.setOrigin({25, 25});
    ActionClock.start();
    DamagedClock.start();
}

Enemy::Enemy(const Enemy &other)
    : Name(other.Name),
      Stats(other.Stats),
      Weapon(other.Weapon->clone()),
      Sprite(other.Sprite) {}

Enemy::~Enemy() {
    std::cout << Name << " Destroyed\n";
}

std::shared_ptr<Enemy> Enemy::clone() const {
    return std::make_shared<Enemy>(*this);
}

void swap(Enemy &first, Enemy &second) noexcept {
    std::swap(first.Name, second.Name);
    std::swap(first.Stats, second.Stats);
    std::swap(first.Experience, second.Experience);
    std::swap(first.Sprite, second.Sprite);
    std::swap(first.Position, second.Position);
    std::swap(first.Weapon, second.Weapon);
    std::swap(first.Damaged, second.Damaged);
    std::swap(first.inAttack, second.inAttack);
    std::swap(first.getAttackReady, second.getAttackReady);
    std::swap(first.DamagedTimer, second.DamagedTimer);
    std::swap(first.ActionClock, second.ActionClock);
    std::swap(first.AttackWarningClock, second.AttackWarningClock);
    std::swap(first.CooldownClock, second.CooldownClock);
    std::swap(first.DamagedClock, second.DamagedClock);
}

Enemy & Enemy::operator=(Enemy other) {
    swap(*this, other);
    return *this;
}

sf::FloatRect Enemy::GetEnemyHitbox() {
    return Sprite.getGlobalBounds();
}

void Enemy::ChangeDamagedStatus(bool Value, float Seconds) {
    if (Value && !Damaged) {
        if (DamagedClock.getElapsedTime().asSeconds() >= Seconds) {
            Damaged = true;
            DamagedClock.restart();
        }
    }
    if (!Value) {
        Damaged = false;
    }
    DamagedTimer = Seconds;
}

bool Enemy::GetDamagedStatus() const {
    return Damaged;
}

float Enemy::GetDamage() const {
    return Weapon->DamageCalculation();
}

int Enemy::GetExperience() const {
    return Experience;
}

bool Enemy::TakeDamage(float Damage_Points) {
    return Stats.ReduceHealth(Damage_Points);
}

void Enemy::AssignWeapon(const std::shared_ptr<Tool>& toolPtr) {
    Weapon = nullptr;
    Weapon = toolPtr->clone();
    // std::cout << "   Tipul din ToolList: " << typeid(*Weapon).name() << "\n";
    // std::cout<<*Weapon;
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
    AttackWarning.ShowSprite(window);
}

void Enemy::RenderHitboxes(sf::RenderWindow &window) const {
    Weapon->ShowHitboxes(window);
}

void Enemy::Update(const sf::Vector2f& PlayerPosition, float deltaTime, float deltaTimeMultiplier) {
    AttackWarning.Update();
    HandleActions(PlayerPosition, deltaTime, deltaTimeMultiplier);
    Weapon->Update(deltaTime);

    if (DamagedTimer < DamagedClock.getElapsedTime().asSeconds()) {
        Damaged = false;
    }
}

void Enemy::HandleActions(const sf::Vector2f &PlayerPosition, float deltaTime, float deltaTimeMultiplier) {
    sf::Vector2f direction = PlayerPosition - Sprite.getPosition();

    // float radians = std::atan2(direction.y, direction.x);
    // float angleDegrees = radians * 180.f / 3.14f;

    if (inAttack == false && getAttackReady == false) {
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
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
            HandleMeleeAttack(1, direction);
        }
    }
    HandleMeleeAttack(0, direction);

}

void Enemy::HandleMeleeAttack(bool canAttack, sf::Vector2f direction) {
    sf::Vector2f TempPosition = Sprite.getPosition();
    AttackWarning.SetPosition({TempPosition.x, TempPosition.y - 60});

    if (inAttack == true) {
        if (CooldownClock.getElapsedTime().asSeconds() >= Weapon->GetCooldown()) {
            std::cout <<"Attack is done! Cooldown finished.\n";
            inAttack = false;
        }
        return;
    }

    if (getAttackReady == false && canAttack) {
        AttackWarning.SetVisibility(true, 0.5f);
        AttackWarningClock.restart();
        getAttackReady = true;
        return;
    }

    if (getAttackReady == true) {
        if (AttackWarningClock.getElapsedTime().asSeconds() >= 0.5f) {
            std::cout <<"Started attack! \n";

            float radians = std::atan2(direction.y, direction.x);
            float angleDegrees = radians * 180.0f / 3.14f;

            Weapon->Attack(Sprite, sf::degrees(angleDegrees));

            CooldownClock.restart();
            inAttack = true;
            getAttackReady = false;
        }
    }
}

std::ostream & operator<<(std::ostream &out, const Enemy &object) {
    out<<object.Name<<"\n"<<object.Stats<<"\n"<<object.Weapon;
    return out;
}


