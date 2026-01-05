

#include "Enemy.h"

int Enemy::id = 0;

Enemy::Enemy(const std::string &name_): Name(name_) {
    std::string texturePath = "data/textures/enemies/" + Name + "/anim.png";

    if (!EnemyTexture.loadFromFile(texturePath)) {
        throw AssetMissingException(texturePath);
    }

    Sprite.setTexture(&EnemyTexture);

    Sprite.setPosition(Position);
    Sprite.setSize(sf::Vector2f(FrameSize));
    Sprite.setOrigin({FrameSize.x / 2.0f, FrameSize.y / 2.0f});

    Sprite.setTextureRect(sf::IntRect({0, 0}, FrameSize));

    ActionClock.start();
    DamagedClock.start();
    LocalId = id;
}

Enemy::Enemy(const Enemy &other)
    : Name(other.Name),
      EnemyTexture(other.EnemyTexture),
      AttackWarning(other.AttackWarning),
      LocalId(other.LocalId),
      MeleeWeapon(other.MeleeWeapon ? other.MeleeWeapon->clone() : nullptr),
      RangedWeapon(other.RangedWeapon ? other.RangedWeapon->clone() : nullptr),
      Damaged(other.Damaged),
      inAttack(other.inAttack),
      Stats(other.Stats),
      getAttackReady(other.getAttackReady),
      DamagedTimer(other.DamagedTimer),
      Sprite(other.Sprite),
      Position(other.Position),
      ActionClock(other.ActionClock),
      AttackWarningClock(other.AttackWarningClock),
      CooldownClock(other.CooldownClock),
      DamagedClock(other.DamagedClock) {
    Sprite.setTexture(&EnemyTexture);
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
    swap(first.EnemyTexture, second.EnemyTexture);
    swap(first.Stats, second.Stats);
    swap(first.MeleeWeapon, second.MeleeWeapon);
    swap(first.RangedWeapon, second.RangedWeapon);
    swap(first.Sprite, second.Sprite);
    swap(first.Position, second.Position);
    swap(first.Damaged, second.Damaged);
    swap(first.inAttack, second.inAttack);
    swap(first.getAttackReady, second.getAttackReady);
    swap(first.DamagedTimer, second.DamagedTimer);
    swap(first.ActionClock, second.ActionClock);
    swap(first.AttackWarningClock, second.AttackWarningClock);
    swap(first.CooldownClock, second.CooldownClock);
    swap(first.DamagedClock, second.DamagedClock);
    swap(first.AttackWarning, second.AttackWarning);

    first.Sprite.setTexture(&first.EnemyTexture);
    second.Sprite.setTexture(&second.EnemyTexture);
}

Enemy & Enemy::operator=(Enemy other) {
    swap(*this, other);
    return *this;
}

sf::FloatRect Enemy::GetEnemyHitbox() {
    return Sprite.getGlobalBounds();
}

const std::vector<std::shared_ptr<Attack>> & Enemy::GetHitboxes() {
    ActiveHitboxes.clear();

    if (MeleeWeapon != nullptr) {
        const auto& meleeHits = MeleeWeapon->GetAttackHitboxes();
        ActiveHitboxes.insert(ActiveHitboxes.end(), meleeHits.begin(), meleeHits.end());
    }

    if (RangedWeapon != nullptr) {
        const auto& rangedHits = RangedWeapon->GetAttackHitboxes();
        ActiveHitboxes.insert(ActiveHitboxes.end(), rangedHits.begin(), rangedHits.end());
    }

    return ActiveHitboxes;
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

const std::string & Enemy::GetName() const {
    return Name;
}

int Enemy::GetLocalId() const {
    return LocalId;
}

int Enemy::GetExperience() const {
    return Stats.GetMana();
}

sf::Vector2f Enemy::GetPosition() const {
    return Sprite.getPosition();
}

bool Enemy::TakeDamage(float Damage_Points) {
    return Stats.ReduceHealth(Damage_Points);
}

void Enemy::AssignWeapon(const std::shared_ptr<Tool>& toolPtr, bool isMelee) {
    if (isMelee) {
        MeleeWeapon = nullptr;
        MeleeWeapon = toolPtr->clone();
    }
    else {
        RangedWeapon = nullptr;
        RangedWeapon = toolPtr->clone();
    }
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
    MeleeWeapon->ShowHitboxes(window);
    RangedWeapon->ShowHitboxes(window);
}

void Enemy::MoveSafely(const std::vector<std::shared_ptr<Enemy>>& otherEnemies) {
    sf::Vector2f pushForce(0.f, 0.f);
    sf::Vector2f currentPos = Sprite.getPosition();
    float minSafeDistance = 40.f; // Raza de coliziune personală

    for (const auto& other : otherEnemies) {
        if (other.get() == this) continue;

        sf::Vector2f otherPos = other->GetPosition();
        sf::Vector2f diff = currentPos - otherPos;
        float distSq = diff.x * diff.x + diff.y * diff.y;

        if (distSq < minSafeDistance * minSafeDistance && distSq > 0.01f) {
            float dist = std::sqrt(distSq);
            sf::Vector2f repulsion = (diff / dist) * (minSafeDistance - dist);
            pushForce += repulsion;
        }
    }
    sf::Vector2f finalMovement = IntendedMovement + (pushForce * 0.5f);
    sf::Vector2f nextPos = currentPos + finalMovement;

    // Verificam daca inamicul trece de limitele hartii
    const float minX = 50.f;
    const float maxX = 1230.f;
    const float minY = 50.f;
    const float maxY = 670.f;

    if (nextPos.x < minX || nextPos.x > maxX) {
        finalMovement.x = 0.f;
    }

    if (nextPos.y < minY || nextPos.y > maxY) {
        finalMovement.y = 0.f;
    }

    Sprite.move(finalMovement);
}

void Enemy::UpdateAnimation(const sf::Vector2f &PlayerPosition, float deltaTime) {
    sf::Vector2f currentPos = Sprite.getPosition();

    if (inAttack || getAttackReady || isChargingRanged) {
        facingRight = (PlayerPosition.x >= currentPos.x);
    }

    else if (std::abs(IntendedMovement.x) > 0.1f) {
        facingRight = (IntendedMovement.x > 0);
    }

    Sprite.setScale(facingRight ? sf::Vector2f(1.f, 1.f) : sf::Vector2f(-1.f, 1.f));


    if (isChargingRanged) {
        currentVisualID = 5;
    }
    else if (inAttack) {
        currentVisualID = 4;
    }
    else if (getAttackReady) {
        currentVisualID = 3;
    }
    else if (std::abs(IntendedMovement.x) > 0.1f || std::abs(IntendedMovement.y) > 0.1f) {
        animationTimer += deltaTime;
        currentVisualID = (static_cast<int>(animationTimer * 5.0f) % 2 == 0) ? 1 : 2;
    }
    else {
        currentVisualID = 0; // Idle
    }

    Sprite.setTextureRect(sf::IntRect({currentVisualID * FrameSize.x, 0}, FrameSize));
}

void Enemy::Update(const sf::Vector2f& PlayerPosition, float deltaTime, float deltaTimeMultiplier, const std::vector<std::shared_ptr<Enemy>>& otherEnemies) {
    AttackWarning.Update();
    HandleActions(PlayerPosition, deltaTime, deltaTimeMultiplier);
    MeleeWeapon->Update(deltaTime);
    RangedWeapon->Update(deltaTime);

    if ((!inAttack || isRangedAttacking) && !getAttackReady)
        MoveSafely(otherEnemies);

    UpdateAnimation(PlayerPosition, deltaTime);

    if (DamagedTimer < DamagedClock.getElapsedTime().asSeconds()) {
        Damaged = false;
    }
    if (CooldownClock.getElapsedTime().asSeconds() > nextAttackDelay && isRangedAttacking) {
        isRangedAttacking = false;
        CooldownClock.restart();
    }
}

void Enemy::DisplayInfo(std::ostream &out) const {
    out<<std::endl << "Enemy Name: " << Name << std::endl
            << "Melee Weapon: " << *MeleeWeapon << std::endl
            << "Ranged Weapon: "<< *RangedWeapon <<std::endl
            << "Stats: " << Stats << std::endl
            << "ID: " << LocalId << std::endl;
}

void Enemy::AssignID(const std::shared_ptr<Enemy> &enemy) {
    enemy->LocalId = Enemy::id;
    Enemy::id++;
    if (Enemy::id == 2001) {
        Enemy::id = 0;
    }
}

void Enemy::HandleActions(const sf::Vector2f &PlayerPosition, float deltaTime, float deltaTimeMultiplier) {
    sf::Vector2f direction = PlayerPosition - Sprite.getPosition();

    // float radians = std::atan2(direction.y, direction.x);
    // float angleDegrees = radians * 180.f / 3.14f;

    if (inAttack == false && getAttackReady == false) {
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (distance > 50.0f) {

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
            HandleRangedAttack(1, direction);
        }
    }
    HandleMeleeAttack(0, direction);

}

void Enemy::HandleMeleeAttack(bool canAttack, sf::Vector2f direction) {
    if (MeleeWeapon == NULL) throw InvalidActionException("Cannot attack without a weapon");
    sf::Vector2f TempPosition = Sprite.getPosition();
    AttackWarning.SetPosition({TempPosition.x, TempPosition.y - 60});

    if (inAttack == true) {
        if (CooldownClock.getElapsedTime().asSeconds() >= MeleeWeapon->GetCooldown()) {
            //std::cout <<"Attack is done! Cooldown finished.\n";
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
            //std::cout <<"Started attack! \n";

            float radians = std::atan2(direction.y, direction.x);
            float angleDegrees = radians * 180.0f / 3.14f;

            MeleeWeapon->CreateAttack(Sprite, sf::degrees(angleDegrees));

            CooldownClock.restart();
            inAttack = true;
            getAttackReady = false;
        }
    }
}

void Enemy::HandleRangedAttack(bool canAttack, sf::Vector2f direction, float waitTime) {
    if (RangedWeapon == NULL) throw InvalidActionException("Cannot attack without a weapon");

    if (canAttack) {
        if (CooldownClock.getElapsedTime().asSeconds() < RangedWeapon->GetCooldown()) {
            isChargingRanged = true;
        }
        else if (CooldownClock.getElapsedTime().asSeconds() > RangedWeapon->GetCooldown() && !isRangedAttacking) {
            float radians = std::atan2(direction.y, direction.x);
            float angleDegrees = radians * 180.0f / 3.14159f;
            float weaponCooldown = RangedWeapon->CreateAttack(Sprite, sf::degrees(angleDegrees));
            nextAttackDelay = weaponCooldown + waitTime;
            isChargingRanged = false;
            isRangedAttacking = true;
        }
        else if (CooldownClock.getElapsedTime().asSeconds() > RangedWeapon->GetCooldown() + waitTime) {
            isRangedAttacking = false;
            CooldownClock.restart();
        }
    }
}

std::ostream & operator<<(std::ostream &out, const Enemy &object) {
    object.DisplayInfo(out);
    return out;
}


