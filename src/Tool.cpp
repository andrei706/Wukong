
#include "Tool.h"

Tool::Tool(const std::string &name_, float Damage_, float Cooldown_, float Range_, int Critical_Chance_)
    : Name(name_), Damage(Damage_), Cooldown(Cooldown_), Range(Range_), Critical_Chance(Critical_Chance_) {}

float Tool::DamageCalculation() const {
    srand(time(nullptr));
    int Chance = rand() % 100;
    if (Chance < Critical_Chance) {
        return (float)Damage + 0.5 * Damage;
    }
    return Damage;
}

float Tool::Attack(const sf::RectangleShape &Sprite, sf::Angle Degrees) {
    float angleDegrees = (float)Degrees.asDegrees();
    float angleRadians = angleDegrees * (3.14 / 180.0);

    sf::Vector2f Offset{
        Range * std::cos(angleRadians),
        Range * std::sin(angleRadians)
    };

    CreateAttackHitbox(Sprite.getPosition(), Offset, Degrees);

    return Cooldown;
}

float Tool::GetCooldown() const {
    return Cooldown;
}

std::shared_ptr<Tool> Tool::clone() const {
    return std::make_shared<Tool>(*this);
}

void Tool::Update(float deltaTime) {
    // Update all attacks first
    for (auto& attack : Attacks) {
        attack->Update(deltaTime);
    }

    // Then remove inactive ones
    Attacks.erase(
        std::remove_if(Attacks.begin(), Attacks.end(),
                       [](const std::shared_ptr<Attack_Hitbox>& attack) {
                           return !attack->IsActive(); // Assuming you have an IsActive() method
                       }),
        Attacks.end()
    );
}

void Tool::CreateAttackHitbox(sf::Vector2f Position, sf::Vector2f Offset, sf::Angle Degrees) {
    auto new_hitbox = std::make_shared<Attack_Hitbox>(
        DamageCalculation(),
        sf::Vector2f{30.0f, 100.0f},
        Position + Offset,
        Degrees
    );

    Attacks.push_back(new_hitbox);
}

const std::string & Tool::GetName() {
    return Name;
}

const std::vector<std::shared_ptr<Attack_Hitbox>> & Tool::GetAttackHitboxes() const {
    return Attacks;
}

void Tool::ClearAttacks() {
    Attacks.clear();
}

void Tool::ShowHitboxes(sf::RenderWindow &window) const {
    for (const auto & i : Attacks) {
        i->ShowSprite(window);
    }
}

std::ostream & operator<<(std::ostream &out, const Tool &object) {
    object.DisplayInfo(out);
    // for (const auto & i : object.Attacks) {
    //     out<<i;
    // }
    return out;
}
