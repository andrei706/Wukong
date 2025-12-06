
#include "Tool_Ranged.h"

Tool_Ranged::Tool_Ranged(const std::string &name_, float Damage_, float Cooldown_, float Range_, float Critical_Chance_,
    float ProjectileSize_, float ProjectileSpeed_, float Lifetime_)
    : Tool(name_, Damage_, Range_, Critical_Chance_),
    ProjectileSize(ProjectileSize_),
    ProjectileSpeed(ProjectileSpeed_),
    Lifetime(Lifetime_){}

void Tool_Ranged::CreateAttackHitbox(sf::Vector2f Position, sf::Vector2f Offset, sf::Angle Degrees) {
    Offset.x = ProjectileSpeed * Offset.x / Range;
    Offset.y = ProjectileSpeed * Offset.y / Range;

    auto new_projectile = std::make_shared<Attack_Projectile>(
        DamageCalculation(),
        sf::Vector2f{ProjectileSize, ProjectileSize},
        Position,
        Degrees,
        Offset,
        Lifetime
    );

    Attacks.push_back(new_projectile);
}
