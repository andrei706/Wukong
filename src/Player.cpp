#include "Player.h"

void Player::UpdateInvincibility() {
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

void Player::MakeInvincible(float Seconds) {
    Invincibility = true;
    InvincibilityTime = sf::seconds(Seconds); // Set how long this specific invincibility lasts
    ClockInvincibilityTime.restart();
}

Player::Player(int Experience_, float InvincibilityTime_): Experience(Experience_), InvincibilityTime(sf::seconds(InvincibilityTime_)) {

    if (!PlayerTexture.loadFromFile("data/textures/player/Monkey_Sprite_Sheet.png")) {
        throw AssetMissingException("data/textures/player/Monkey_Sprite_Sheet.png");
    }

    Sprite.setSize(sf::Vector2f(FrameSize));
    Sprite.setOrigin({FrameSize.x / 2.0f, FrameSize.y / 2.0f});
    Sprite.setTexture(&PlayerTexture);
    Sprite.setTextureRect(sf::IntRect({0, 0}, FrameSize));
    Sprite.setPosition(Position);

    //sf::IntRect rechargingRect({0, 0}, {50, 50});

    if (!DashCooldownTexture.loadFromFile("data/textures/player/dash_icon.png")) {
        throw AssetMissingException("data/textures/player/dash_icon.png");
    }
    DashCooldownSprite.setTexture(&DashCooldownTexture);
    DashCooldownSprite.setPosition({20.f, 80.f});
    DashCooldownSprite.setOrigin({12.5f, 12.5f});
    DashCooldownSprite.setTextureRect(sf::IntRect({0, 0}, {50, 50}));
    DashCooldownSprite.setSize({25.f, 25.f});

    RangedCooldown.start();
    ClockDodgeCooldown.restart();
    BallRangedCooldown.start();
    AttackCooldown.start();

    Invincibility = false;

}

void Player::ShowSprite(sf::RenderWindow &window) const {
    window.draw(Sprite);
    window.draw(DashCooldownSprite);
    Pole.ShowHitboxes(window);
}

void Player::SetPosition(sf::Vector2f position) {
    Sprite.setPosition(position);
    Position = position;
}

sf::RectangleShape & Player::GetSprite() {
    return Sprite;
}

sf::Vector2f Player::GetPosition() const {
    return Sprite.getPosition();
}

float Player::GetHealth() const {
    return Stats.GetHealth();
}

int Player::GetGauge() const {
    return Gauge;
}

void Player::AddExperience(int Value) {
    Experience += Value;
}

int Player::GetExperience() const {
    return Experience;
}

bool Player::TakeDamage(float Value) {
    if (!Invincibility) {
        MakeInvincible(0.5f);
        Stats.ReduceHealth(Value);
        return 1;
    }
    return 0;
}

void Player::RestoreHealth(float Value) {
    Stats.RestoreHealth(Value);
}

const std::vector<std::shared_ptr<Attack>>& Player::GetHitboxes(){
    ActiveHitboxes.clear();
    const auto& pole_hits = Pole.GetAttackHitboxes();
    ActiveHitboxes.insert(ActiveHitboxes.end(), pole_hits.begin(), pole_hits.end());
    const auto& blast_hits = Blast.GetAttackHitboxes();
    ActiveHitboxes.insert(ActiveHitboxes.end(), blast_hits.begin(), blast_hits.end());
    const auto& spirit_hits = SpiritBall.GetAttackHitboxes();
    ActiveHitboxes.insert(ActiveHitboxes.end(), spirit_hits.begin(), spirit_hits.end());

    return ActiveHitboxes;
}

void Player::ApplyUpgrades(const std::vector<std::pair<std::string, float>> &upgrades) {
    for (const auto& upgrade : upgrades) {
        const std::string& statName = upgrade.first;
        float value = upgrade.second;

        if (value <= 0) continue;

        if (statName == "MaxHealth") {
            Stats.AddStat("MaxHealth", 20);
        }
        else if (statName == "Defense") {
            Stats.AddStat("Defense", value);
        }
        else if (statName == "MeleeWeapon") {
            UpgradeWeapon("Melee", value, 0.1f);
        }
        else if (statName == "RangedWeapon") {
            UpgradeWeapon("Ranged", value, 0.1f);
        }
        else if (statName == "SpiritBall") {
            UpgradeWeapon("SpiritBall", value, 0.1f);
        }
        else if (statName == "RestoreHealth") {
            RestoreHealth(20 * value);
        }
    }
}

void Player::Update(sf::RenderWindow &window, float deltaTime, float deltaTimeMultiplier, Key_Manager &keyManager) {
    UpdateInvincibility();
    Pole.Update(deltaTime);
    Blast.Update(deltaTime);
    SpiritBall.Update(deltaTime);
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
    UpdateDashSprite();
}

void Player::Restart() {
    Experience = 25;
    Gauge = 50;
    Stats.RestoreHealth(500.0f);
    Stats.ResetStats();
    Blast.ResetStats();
    Blast.ClearAttackHitboxes();
    SpiritBall.ResetStats();
    SpiritBall.ClearAttackHitboxes();
    Pole.ResetStats();
    Pole.ClearAttackHitboxes();

    Invincibility = false;
    inAttack = false;
    inRangedAttack = false;
    inRangedBallAttack = false;
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

void Player::PauseClocks(bool Pause) {
    if (Pause) {
        ClockDodgeDuration.stop();
        ClockInvincibilityTime.stop();
        AttackCooldown.stop();
        RangedCooldown.stop();
        ClockDodgeCooldown.stop();
        BallRangedCooldown.stop();
    }
    else {
        ClockDodgeDuration.start();
        ClockInvincibilityTime.start();
        AttackCooldown.start();
        RangedCooldown.start();
        ClockDodgeCooldown.start();
        BallRangedCooldown.start();
    }
}

float Player::HandleAttack(Key_Manager& KeyManager) {
    inAttack = false;
    if (!inRangedBallAttack && !inRangedAttack)
        SpeedMultiplier = 1.f;

    if (isDodging)
        return 0.0f;

    if ((BallRangedCooldown.getElapsedTime() > sf::seconds(SpiritBall.GetCooldown())))
        inRangedBallAttack = false;
    if (RangedCooldown.getElapsedTime() > sf::seconds(Blast.GetCooldown()))
        inRangedAttack = false;

    if (KeyManager.CheckInput("F")) {
        if (BallRangedCooldown.getElapsedTime() > sf::seconds(SpiritBall.GetCooldown()) && Gauge >= 25) {
            Gauge -= 25;
            ActiveHitboxes.clear();
            inRangedBallAttack = true;
            if (SpeedMultiplier > 0.9f)
                SpeedMultiplier = 0.9f;
            SpiritBall.Attack(Sprite, Rotation);
            const auto& spirit_hits = SpiritBall.GetAttackHitboxes();
            ActiveHitboxes.insert(ActiveHitboxes.end(), spirit_hits.begin(), spirit_hits.end());
            BallRangedCooldown.restart();
            return 0.0f;
        }
    }
    if (KeyManager.CheckInput("LeftMouseButton")) {
        if (Gauge < 96) Gauge += 5;
        float cooldown_time;
        inAttack = true;
        SpeedMultiplier = 0.2f;
        cooldown_time = Pole.Attack(Sprite, Rotation);
        const auto& pole_hits = Pole.GetAttackHitboxes();
        ActiveHitboxes.insert(ActiveHitboxes.end(), pole_hits.begin(), pole_hits.end());
        return cooldown_time;
    }

    if (KeyManager.CheckInput("RightMouseButton")) {
        if (RangedCooldown.getElapsedTime() > sf::seconds(Blast.GetCooldown()) && Gauge >= 4) {
            Gauge -= 5;
            ActiveHitboxes.clear();
            inRangedAttack = true;
            SpeedMultiplier = 0.4f;
            Blast.Attack(Sprite, Rotation);
            const auto& blast_hits = Blast.GetAttackHitboxes();
            ActiveHitboxes.insert(ActiveHitboxes.end(), blast_hits.begin(), blast_hits.end());

            RangedCooldown.restart();
            return 0.f;
        }
    }
    return 0.0f;
}

void Player::UpgradeWeapon(const std::string &Name, float Bonus_Damage, float Damage_Multiplier) {
    if (Name == "Melee") {
        Pole.AddStat("Damage", Bonus_Damage);
        Pole.AddStat("Damage_Multiplier", Damage_Multiplier);
    }
    if (Name == "Ranged") {
        Blast.AddStat("Damage", Bonus_Damage);
        Blast.AddStat("Damage_Multiplier", Damage_Multiplier);
    }
    if (Name == "SpiritBall") {
        SpiritBall.AddStat("Damage", Bonus_Damage);
        SpiritBall.AddStat("Damage_Multiplier", Damage_Multiplier);
    }
}

void Player::UpdateDashSprite() {
    if (isDodging) {
        DashCooldownSprite.setFillColor(sf::Color::Transparent);
        return;
    }
    DashCooldownSprite.setPosition({Sprite.getPosition().x, Sprite.getPosition().y - 40});
    float elapsed = ClockDodgeCooldown.getElapsedTime().asSeconds();
    float cooldown = DodgeCooldown.asSeconds();
    float lingerTime = 0.4f;
    if (elapsed < cooldown) {
        float ratio = elapsed / cooldown;
        int alpha = static_cast<int>(ratio * 255);
        DashCooldownSprite.setTextureRect(sf::IntRect({0, 0}, {50, 50}));
        DashCooldownSprite.setScale({1.0f, 1.0f});
        DashCooldownSprite.setFillColor(sf::Color(255, 255, 255, alpha));
    }
    else if (elapsed < cooldown + lingerTime) {
        DashCooldownSprite.setTextureRect(sf::IntRect({50, 0}, {50, 50}));
        DashCooldownSprite.setScale({1.2f, 1.2f});
        DashCooldownSprite.setFillColor(sf::Color(255, 255, 255, 255));
    }
    else {
        DashCooldownSprite.setFillColor(sf::Color::Transparent);
        DashCooldownSprite.setScale({1.0f, 1.0f});
    }
}

void Player::HandleMovement(sf::RenderWindow &window, float deltaTime, float deltaTimeMultiplier) {

    const float minX = 50.f;
    const float maxX = 1230.f;
    const float minY = 50.f;
    const float maxY = 670.f;

    if (inAttack && SpeedMultiplier == 0.f)
        return;


    if (isDodging) {
        if (ClockDodgeDuration.getElapsedTime() >= DodgeDuration) {
            isDodging = false;
        } else {
            sf::Vector2f dashStep = DodgeDirection * DodgeSpeed * deltaTime * deltaTimeMultiplier;
            sf::Vector2f nextDodgePos = Sprite.getPosition() + dashStep;

            if (nextDodgePos.x < minX || nextDodgePos.x > maxX) dashStep.x = 0.f;
            if (nextDodgePos.y < minY || nextDodgePos.y > maxY) dashStep.y = 0.f;

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

    sf::Vector2f nextPos = Sprite.getPosition() + movement;

    if (nextPos.x < minX || nextPos.x > maxX) movement.x = 0.f;
    if (nextPos.y < minY || nextPos.y > maxY) movement.y = 0.f;

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

void Player::UpdateAnimation(float dt) {
    if (isDodging)
        VisualID = 5;
    else if (inAttack) {
        VisualID = 3; // Melee stance
    }
    else if (inRangedAttack || inRangedBallAttack) {
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

void Player::StartDodge(sf::Vector2f inputDirection) {
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

void Player::HandleDodge(Key_Manager &keyManager) {
    if (keyManager.CheckInput("Space")) {
        if (ClockDodgeCooldown.getElapsedTime() >= DodgeCooldown && !isDodging) {
            if (inAttack) {
                inAttack = false;
                //Pole.ClearAttackHitboxes();
            }
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

std::ostream & operator<<(std::ostream &out, const Player &object) {
    out<<object.Experience<<"\n"<<object.Gauge<<"\n"<<object.inAttack<<"\n"<<object.Invincibility<<object.Pole;
    return out;
}
