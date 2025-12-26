#include "Player_Class.h"

void Player_Class::UpdateInvincibility() {
    if (Invincibility) {
        if (!isDodging) {
            if ((int)(ClockInvincibilityTime.getElapsedTime().asMilliseconds() / 100) % 2 == 0) {
                Sprite.setFillColor(sf::Color(255, 255, 255, 100));
            } else {
                Sprite.setFillColor(sf::Color::White);
            }
        }
        if (ClockInvincibilityTime.getElapsedTime() >= InvincibilityTime) {
            Invincibility = false;
            Sprite.setFillColor(sf::Color::White);
        }
    }
}

void Player_Class::MakeInvincible(float Seconds) {
    Invincibility = true;
    InvincibilityTime = sf::seconds(Seconds); // Set how long this specific invincibility lasts
    ClockInvincibilityTime.restart();
}

Player_Class::Player_Class(int Experience_, float InvincibilityTime_): Experience(Experience_), InvincibilityTime(sf::seconds(InvincibilityTime_)) {

    if (!PlayerTexture.loadFromFile("data/textures/player/Monkey_Sprite_Sheet.png")) {
        throw AssetMissingException("data/textures/player/Monkey_Sprite_Sheet.png");
    }

    Sprite.setSize(sf::Vector2f(FrameSize));
    Sprite.setOrigin({FrameSize.x / 2.0f, FrameSize.y / 2.0f});
    Sprite.setTexture(&PlayerTexture);
    Sprite.setTextureRect(sf::IntRect({0, 0}, FrameSize));
    Sprite.setPosition(Position);

    RangedCooldown.start();
    ClockDodgeCooldown.restart();
    AttackCooldown.start();

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

float Player_Class::GetGauge() const {
    return Gauge;
}

void Player_Class::AddExperience(int Value) {
    Experience += Value;
}

void Player_Class::TakeDamage(float Value) {
    if (!Invincibility) {
        std::cout<<Value;
        Stats.ReduceHealth(Value);
        MakeInvincible(0.5f);
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

void Player_Class::Update(sf::RenderWindow &window, float deltaTime, float deltaTimeMultiplier, Key_Manager &keyManager) {
    UpdateInvincibility();
    Pole.Update(deltaTime);
    Blast.Update(deltaTime);
    HandleDodge(keyManager);
    if (InAttackTime < AttackCooldown.getElapsedTime()) {
        float cooldown = HandleAttack(keyManager);
        if (cooldown > 0) {
            InAttackTime = sf::seconds(cooldown);
            AttackCooldown.restart();
        }
    }
    HandleMovement(window, deltaTime, deltaTimeMultiplier);
    UpdateAnimation(deltaTime);
}

void Player_Class::Restart() {
    Experience = 0;
    Gauge = 50;
    Stats.RestoreHealth(500.0f);

    Invincibility = false;
    inAttack = false;
    inRangedAttack = false;
    isDodging = false;

    ActiveHitboxes.clear();

    SetPosition({140.f, 360.f});
    Sprite.setFillColor(sf::Color::White);
    Sprite.setTextureRect(sf::IntRect({0, 0}, FrameSize));

    AttackCooldown.restart();
    RangedCooldown.restart();
    ClockDodgeCooldown.restart();
    ClockInvincibilityTime.restart();
}

float Player_Class::HandleAttack(Key_Manager& KeyManager) {
    inAttack = false;
    SpeedMultiplier = 1.f;

    if (isDodging)
        return 0.0f;

    if (KeyManager.CheckInput("LeftMouseButton")) {
        if (Gauge < 96) Gauge += 5;
        float cooldown_time;
        inAttack = true;
        SpeedMultiplier = 0.f;
        cooldown_time = Pole.Attack(Sprite, Rotation);
        const auto& pole_hits = Pole.GetAttackHitboxes();
        ActiveHitboxes.insert(ActiveHitboxes.end(), pole_hits.begin(), pole_hits.end());
        return cooldown_time;
    }

    if (KeyManager.CheckInput("RightMouseButton")) {
        // Verificarea cooldownului de la distanta + daca are gauge
        if (RangedCooldown.getElapsedTime() > sf::seconds(0.2f) && Gauge >= 4) {
            Gauge -= 5;
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
    else if (!(RangedCooldown.getElapsedTime() > sf::seconds(0.2f) && Gauge >= 4))
            inRangedAttack = false;
    return 0.0f;
}

void Player_Class::HandleMovement(sf::RenderWindow &window, float deltaTime, float deltaTimeMultiplier) {

    if (inAttack && SpeedMultiplier == 0.f)
        return;

    if (isDodging) {
        if (ClockDodgeDuration.getElapsedTime() >= DodgeDuration) {
            isDodging = false;
        } else {
            sf::Vector2f dashStep = DodgeDirection * DodgeSpeed * deltaTime * deltaTimeMultiplier;
            Sprite.move(dashStep);
            return;
        }
    }

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

    FacingRight = (MouseWorldPos.x >= PlayerPosition.x);

    float radians = std::atan2(diff.y, diff.x);
    float angleDegrees = radians * 180.f / 3.14f;
    //Sprite.setRotation(sf::degrees(angleDegrees));
    Rotation = sf::degrees(angleDegrees);
}

void Player_Class::UpdateAnimation(float dt) {
    if (isDodging)
        VisualID = 5;
    else if (inAttack) {
        VisualID = 3; // Melee stance
    }
    else if (inRangedAttack) {
        VisualID = 4; // Ranged stance
    }
    else if (SpeedMultiplier > 0.1f && (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
                                        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
                                        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
                                        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))) {
        AnimationTimer += dt;
        if (AnimationTimer >= 0.15f) {
            WalkToggle = (WalkToggle == 0) ? 1 : 0;
            AnimationTimer = 0.f;
        }
        VisualID = (WalkToggle == 0) ? 1 : 2;
    }
    else {
        VisualID = 0; // Idle
    }

    Sprite.setTextureRect(sf::IntRect({VisualID * FrameSize.x, 0}, FrameSize));

    if (FacingRight) Sprite.setScale({1.f, 1.f});
    else Sprite.setScale({-1.f, 1.f});
}

void Player_Class::StartDodge(sf::Vector2f inputDirection) {
    isDodging = true;
    ClockDodgeDuration.restart();
    ClockDodgeCooldown.restart();

    if (inputDirection.x != 0 || inputDirection.y != 0) {
        float length = std::sqrt(inputDirection.x * inputDirection.x + inputDirection.y * inputDirection.y);
        DodgeDirection = inputDirection / length;
    }
    else {
        float radians = Rotation.asRadians();
        // float radians = Rotation * 3.14159f / 180.f;
        DodgeDirection = sf::Vector2f(std::cos(radians), std::sin(radians));
    }
    MakeInvincible(DodgeDuration.asSeconds());
}

void Player_Class::HandleDodge(Key_Manager &keyManager) {
    if (keyManager.CheckInput("Space")) {
        if (ClockDodgeCooldown.getElapsedTime() >= DodgeCooldown && !isDodging) {
            MakeInvincible(0.5f);
            sf::Vector2f inputDir(0.f, 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) inputDir.y -= 1.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) inputDir.y += 1.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) inputDir.x -= 1.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) inputDir.x += 1.f;
            StartDodge(inputDir);

            if (inAttack)
                SpeedMultiplier = 0.5f;
        }
    }
}

std::ostream & operator<<(std::ostream &out, const Player_Class &object) {
    out<<object.Experience<<"\n"<<object.Gauge<<"\n"<<object.inAttack<<"\n"<<object.Invincibility<<object.Pole;
    return out;
}
