

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

    Gauge = 0;
    Invincibility = false;
}

void Player_Class::ShowSprite(sf::RenderWindow &window) const {
    window.draw(Sprite);
    Pole.ShowHitboxes(window);
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

std::vector<Attack_Hitbox> Player_Class::GetHitboxes() const {
    return Pole.GetAttackHitboxes();
}

void Player_Class::ClearAttackHitboxes() {
    Pole.ClearAttacks();
    inAttack = false;
}

float Player_Class::HandleAttack(Key_Manager& KeyManager) {
    inAttack = false;
    Pole.ClearAttacks();
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (!KeyManager.CheckButton("LeftMouseButton")) {
            inAttack = true;
            KeyManager.ToggleActivation("LeftMouseButton");
            return Pole.Attack(Sprite, Rotation);
        }
    }
    else
        if (KeyManager.CheckButton("LeftMouseButton"))
            KeyManager.ToggleActivation("LeftMouseButton");
    return 0.0f;
}

void Player_Class::HandleMovement(sf::RenderWindow &window, float deltaTime, float deltaTimeMultiplier) {
    if (inAttack)
        return;

    //Movement
    sf::Vector2f movement(0.f, 0.f);
    float speed = Stats.GetSpeed();

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
    sf::Vector2f MouseWorldPos = sf::Vector2f(MousePos.x, MousePos.y);
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
