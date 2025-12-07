
#ifndef OOP_TOOL_RANGED_H
#define OOP_TOOL_RANGED_H

#include "Tool.h"
#include "Attack_Projectile.h"

class Tool_Ranged : public Tool{
    float ProjectileSize, ProjectileSpeed, Lifetime;
public:
    Tool_Ranged(const std::string& name_, float Damage_, float Cooldown_, float Range_ = 1.0f, int Critical_Chance_ = 0,
    float ProjectileSize_ = 30.0f, float ProjectileSpeed_ = 15.0f, float Lifetime_ = 4.0f);

    [[nodiscard]] std::shared_ptr<Tool> clone() const override;;

    void DisplayInfo(std::ostream &out) const override{
        Tool::DisplayInfo(out);
        out << "Projectile size: " << ProjectileSize << std::endl
        << "Projectile speed: " << ProjectileSpeed << std::endl
        << "Lifetime: " << Lifetime << std::endl;
    }

    void CreateAttackHitbox(sf::Vector2f Position, sf::Vector2f Offset, sf::Angle Degrees) override;
};


#endif