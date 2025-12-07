

#ifndef OOP_TOOL_H
#define OOP_TOOL_H
#include "Attack_Hitbox.h"

#include <string>
#include <vector>
#include <cmath>

class Tool {


protected:
    std::string Name;
    float Damage, Cooldown, Range;
    int Critical_Chance;

    std::vector<std::shared_ptr<Attack_Hitbox>> Attacks;

public:
    virtual ~Tool() = default;

    Tool(const std::string& name_, float Damage_, float Cooldown_, float Range_ = 10000.0f, int Critical_Chance_ = 0);

    friend std::ostream & operator<<(std::ostream & out, const Tool & object);

    [[nodiscard]] float DamageCalculation() const;

    const std::string& GetName();

    [[nodiscard]] const std::vector<std::shared_ptr<Attack_Hitbox>>& GetAttackHitboxes() const;

    void ClearAttacks();

    void ShowHitboxes(sf::RenderWindow& window) const;

    float GetCooldown() const;

    [[nodiscard]] virtual std::shared_ptr<Tool> clone() const;

    void Update(float deltaTime);

    virtual void CreateAttackHitbox(sf::Vector2f Position, sf::Vector2f Offset, sf::Angle Degrees);

    virtual void DisplayInfo(std::ostream &out) const;;

    float Attack(const sf::RectangleShape &Sprite, sf::Angle Degrees);
};


#endif //OOP_TOOL_H