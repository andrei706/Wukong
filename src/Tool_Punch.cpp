
#include "Tool_Punch.h"

Tool_Punch::Tool_Punch(const std::string &name_, float Damage_, float Cooldown_, float Range_, int Critical_Chance_,
    float PunchRadius_): Tool(name_, Damage_, Cooldown_, Range_, Critical_Chance_), PunchRadius(PunchRadius_) {}

std::shared_ptr<Tool> Tool_Punch::clone() const {
    return std::make_shared<Tool_Punch>(*this);
}

void Tool_Punch::CreateAttackHitbox(sf::Vector2f Position, sf::Vector2f Offset, sf::Angle Degrees) {
    Degrees = sf::degrees(0);

    auto new_hitbox = std::make_shared<Attack_Hitbox>(
        DamageCalculation(),
        sf::Vector2f{PunchRadius, PunchRadius},
        Position + Offset,
        Degrees,
        0.3,
        "data/textures/attack_assets/punch_sprite.png"
    );

    Attacks.push_back(new_hitbox);
}

void Tool_Punch::DisplayInfo(std::ostream &out) const {
    Tool::DisplayInfo(out);
    out << "Punch Radius: " << PunchRadius << std::endl;
}
