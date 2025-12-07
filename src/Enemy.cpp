

#include "Enemy.h"

int Enemy::id = 0;

Enemy::Enemy(const std::string &name_): Name(name_) {
    Sprite.setFillColor(sf::Color::Red);
    Sprite.setPosition(Position);
    Sprite.setSize({50, 50});
    Sprite.setOrigin({25, 25});
    ActionClock.start();
    DamagedClock.start();

    LocalId = id;
    //IncreaseID();
}

Enemy::Enemy(const Enemy &other)
    : Name(other.Name),
      AttackWarning(other.AttackWarning),
      LocalId(other.LocalId),
      Weapon(other.Weapon ? other.Weapon->clone() : nullptr),
      Damaged(other.Damaged),
      inAttack(other.inAttack),
      Stats(other.Stats),
      getAttackReady(other.getAttackReady),
      Experience(other.Experience),
      DamagedTimer(other.DamagedTimer),
      Sprite(other.Sprite),
      Position(other.Position),
      ActionClock(other.ActionClock),
      AttackWarningClock(other.AttackWarningClock),
      CooldownClock(other.CooldownClock),
      DamagedClock(other.DamagedClock)
{
}


Enemy::~Enemy() {
    std::cout << Name << " Destroyed\n";
}

std::shared_ptr<Enemy> Enemy::clone() const {
    return std::make_shared<Enemy>(*this);
}

void swap(Enemy &first, Enemy &second) noexcept {
    using std::swap;
    swap(first.Name, second.Name);
    swap(first.Stats, second.Stats);
    swap(first.Weapon, second.Weapon);
    swap(first.Sprite, second.Sprite);
    swap(first.Position, second.Position);
    swap(first.Experience, second.Experience);

    swap(first.Damaged, second.Damaged);
    swap(first.inAttack, second.inAttack);
    swap(first.getAttackReady, second.getAttackReady);
    swap(first.DamagedTimer, second.DamagedTimer);

    swap(first.ActionClock, second.ActionClock);
    swap(first.AttackWarningClock, second.AttackWarningClock);
    swap(first.CooldownClock, second.CooldownClock);
    swap(first.DamagedClock, second.DamagedClock);

    swap(first.AttackWarning, second.AttackWarning);
}

Enemy & Enemy::operator=(Enemy other) {
    swap(*this, other);
    return *this;
}

sf::FloatRect Enemy::GetEnemyHitbox() {
    return Sprite.getGlobalBounds();
}

const std::vector<std::shared_ptr<Attack_Hitbox>> & Enemy::GetHitboxes() {
    return Weapon->GetAttackHitboxes();
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

// int Enemy::GetLocalId() const {
//     return LocalId;
// }

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
            HandleMeleeAttack(1, direction, Weapon);
        }
    }
    HandleMeleeAttack(0, direction, Weapon);

}

void Enemy::HandleMeleeAttack(bool canAttack, sf::Vector2f direction, std::shared_ptr<Tool> UsedWeapon) {
    sf::Vector2f TempPosition = Sprite.getPosition();
    AttackWarning.SetPosition({TempPosition.x, TempPosition.y - 60});

    if (inAttack == true) {
        if (CooldownClock.getElapsedTime().asSeconds() >= UsedWeapon->GetCooldown()) {
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

            UsedWeapon->Attack(Sprite, sf::degrees(angleDegrees));

            CooldownClock.restart();
            inAttack = true;
            getAttackReady = false;
        }
    }
}

std::ostream & operator<<(std::ostream &out, const Enemy &object) {
    object.DisplayInfo(out);
    return out;
}


