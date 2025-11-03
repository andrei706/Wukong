
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

    Attack_Hitbox Entity{
        DamageCalculation(),
        {30, 100},
        Sprite.getPosition() + Offset,
        Degrees
    };

    Attacks.push_back(Entity);

    return Cooldown;
}

const std::string & Tool::GetName() {
    return Name;
}

const std::vector<Attack_Hitbox> & Tool::GetAttackHitboxes() const {
    return Attacks;
}

void Tool::ClearAttacks() {
    Attacks.clear();
}

void Tool::ShowHitboxes(sf::RenderWindow &window) const {
    for (const auto & i : Attacks) {
        i.ShowSprite(window);
    }
}

std::ostream & operator<<(std::ostream &out, const Tool &object) {
    out<<object.Name<<" "<<object.Damage<<" "<<object.Range<<" "<<object.Cooldown<<" "<<object.Critical_Chance<<"\n";
    for (const auto & i : object.Attacks) {
        out<<i;
    }
    return out;
}
