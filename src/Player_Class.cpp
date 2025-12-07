

#include "Player_Class.h"

bool Player_Class::MakeInvincibile() {
    if (Invincibility == 1) {
        if (ClockInvincibilityTime.getElapsedTime() >= InvincibilityTime) {
            Invincibility = false;
        }
        return true;
    }
    ClockInvincibilityTime.restart();
    Invincibility = true;
    return false;
}

Player_Class::Player_Class(int Experience_, float InvincibilityTime_): Experience(Experience_), InvincibilityTime(sf::seconds(InvincibilityTime_)) {

    Sprite.setSize(Size);
    Sprite.setOrigin({Size.x / 2, Size.y / 2});
    Sprite.setFillColor(sf::Color::Blue);
    Sprite.setPosition(Position);

    RangedCooldown.start();

    Gauge = 0;
    Invincibility = false;
}

void Player_Class::ShowSprite(sf::RenderWindow &window) const {
    window.draw(Sprite);
    Pole.ShowHitboxes(window);
}

void Player_Class::SetPosition(sf::Vector2f position) {
    Sprite.setPosition(position);
    Position = position;
}

sf::RectangleShape & Player_Class::GetSprite() {
    return Sprite;
}

sf::Vector2f Player_Class::GetPosition() const {
    return Sprite.getPosition();
}

float Player_Class::GetHealth() const {
    return Stats.GetHealth();
}

void Player_Class::AddExperience(int Value) {
    Experience += Value;
}

void Player_Class::TakeDamage(float Value) {
    if (!MakeInvincibile()) {
        Stats.ReduceHealth(Value);
    }
}

void Player_Class::RestoreHealth(float Value) {
    Stats.RestoreHealth(Value);
}

const std::vector<std::shared_ptr<Attack_Hitbox>>& Player_Class::GetHitboxes(){
    ActiveHitboxes.clear();
    const auto& pole_hits = Pole.GetAttackHitboxes();
    ActiveHitboxes.insert(ActiveHitboxes.end(), pole_hits.begin(), pole_hits.end());
    const auto& blast_hits = Blast.GetAttackHitboxes();
    ActiveHitboxes.insert(ActiveHitboxes.end(), blast_hits.begin(), blast_hits.end());
    return ActiveHitboxes;
}

void Player_Class::ClearAttackHitboxes() {
    Pole.ClearAttacks();
    ActiveHitboxes.clear();
    inAttack = false;
}

float Player_Class::HandleAttack(Key_Manager& KeyManager) {
    inAttack = false;
    SpeedMultiplier = 1.f;

    if (KeyManager.CheckInput("LeftMouseButton")) {
        float cooldown_time;
        inAttack = true;
        SpeedMultiplier = 0.f;
        cooldown_time = Pole.Attack(Sprite, Rotation);
        const auto& pole_hits = Pole.GetAttackHitboxes();
        ActiveHitboxes.insert(ActiveHitboxes.end(), pole_hits.begin(), pole_hits.end());
        return cooldown_time;
    }

    if (KeyManager.CheckInput("RightMouseButton")) {
        // Verificarea Cooldown-ului la Distanță (RangedCooldown)
        if (RangedCooldown.getElapsedTime() > sf::seconds(0.2f)) {

            ActiveHitboxes.clear();
            inRangedAttack = true;
            SpeedMultiplier = 0.3f;
            Blast.Attack(Sprite, Rotation);

            const auto& blast_hits = Blast.GetAttackHitboxes();
            ActiveHitboxes.insert(ActiveHitboxes.end(), blast_hits.begin(), blast_hits.end());

            RangedCooldown.restart();
            return 0.f;
        }
    }
    return 0.0f;
}

void Player_Class::HandleMovement(sf::RenderWindow &window, float deltaTime, float deltaTimeMultiplier) {
    Pole.Update(deltaTime);
    Blast.Update(deltaTime);

    if (inAttack && SpeedMultiplier == 0.f)
        return;

    //Movement
    sf::Vector2f movement(0.f, 0.f);
    float speed = Stats.GetSpeed() * SpeedMultiplier;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= speed * deltaTime * deltaTimeMultiplier;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += speed * deltaTime * deltaTimeMultiplier;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x -= speed * deltaTime * deltaTimeMultiplier;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += speed * deltaTime * deltaTimeMultiplier;

    Sprite.move(movement);

    sf::Vector2i MousePos = sf::Mouse::getPosition(window);
    sf::Vector2f MouseWorldPos = window.mapPixelToCoords(MousePos);
    sf::Vector2f PlayerPosition = Sprite.getPosition();
    sf::Vector2f diff = MouseWorldPos - PlayerPosition;

    float radians = std::atan2(diff.y, diff.x);
    float angleDegrees = radians * 180.f / 3.14f;
    //Sprite.setRotation(sf::degrees(angleDegrees));
    Rotation = sf::degrees(angleDegrees);
}

std::ostream & operator<<(std::ostream &out, const Player_Class &object) {
    out<<object.Experience<<"\n"<<object.Gauge<<"\n"<<object.inAttack<<"\n"<<object.Invincibility<<object.Pole;
    return out;
}
