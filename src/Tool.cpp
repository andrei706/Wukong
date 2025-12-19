
#include "Tool.h"

Tool::Tool(const std::string &name_, float Damage_, float Cooldown_, float Range_, int Critical_Chance_)
    : Name(name_), Damage(Damage_), Cooldown(Cooldown_), Range(Range_), Critical_Chance(Critical_Chance_) {
    if (Damage < 0) throw InvalidDataException(Name + " Tool Damage", Damage);
    if (Cooldown < 0) throw InvalidDataException(Name + " Tool Cooldown", Cooldown);
    if (Range < 0) throw InvalidDataException(Name + " Tool Range", Range);
    if (Critical_Chance < 0) throw InvalidDataException(Name + " Tool Critical Chance", Critical_Chance);
}

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
    for (auto it = Attacks.begin(); it != Attacks.end(); ) {
        auto& attack = *it;
        attack->Update(deltaTime);

        if (!attack->IsActive()) {
            it = Attacks.erase(it);
        } else {
            ++it;
        }
    }
}

void Tool::CreateAttackHitbox(sf::Vector2f Position, sf::Vector2f Offset, sf::Angle Degrees) {
    auto new_hitbox = std::make_shared<Attack_Hitbox>(
        DamageCalculation(),
        sf::Vector2f{30.0f, 100.0f},
        Position + Offset,
        Degrees,
        Cooldown
    );

    Attacks.push_back(new_hitbox);
}

void Tool::DisplayInfo(std::ostream &out) const {
    out<< std::endl<< "-------------" <<std::endl << "Weapon Name: " << Name << std::endl
            << "Damage: " << Damage << std::endl
            << "Cooldown: " << Cooldown << std::endl
            << "Range: " << Range << std::endl;
}

const std::string & Tool::GetName() {
    return Name;
}

const std::vector<std::shared_ptr<Attack_Hitbox>> & Tool::GetAttackHitboxes() const {
    return Attacks;
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
