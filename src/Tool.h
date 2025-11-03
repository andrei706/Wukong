

#ifndef OOP_TOOL_H
#define OOP_TOOL_H
#include "Attack_Hitbox.h"

#include <string>
#include <vector>
#include <cmath>

class Tool {
    std::string Name;
    float Damage, Cooldown, Range;
    int Critical_Chance;

    std::vector<Attack_Hitbox> Attacks;

public:
    Tool(const std::string& name_, float Damage_, float Cooldown_, float Range_ = 10000.0f, int Critical_Chance_ = 0);

    friend std::ostream & operator<<(std::ostream & out, const Tool & object);

    [[nodiscard]] float DamageCalculation() const;

    float Attack(const sf::RectangleShape &Sprite, sf::Angle Degrees);

    const std::string& GetName();

    [[nodiscard]] const std::vector <Attack_Hitbox>& GetAttackHitboxes() const;

    void ClearAttacks();

    void ShowHitboxes(sf::RenderWindow& window) const;
};


#endif //OOP_TOOL_H