#ifndef OOP_TOOL_H
#define OOP_TOOL_H

#include "Attack_Hitbox.h"
#include "Game_Exceptions.h"
#include "Random_Value_Generator.h"

#include <string>
#include <vector>
#include <cmath>

class Tool {



protected:
    std::string Name;
    float Damage, Cooldown, Range;
    unsigned int Critical_Chance;

    float Bonus_Damage = 0, Damage_Multiplier = 1;

    int Burst_Count;
    float Burst_Delay;

    int m_attacksQueued = 0;
    float m_burstTimer = 0.0f;
    sf::Angle m_lastAngle;
    const sf::RectangleShape* m_sourceSprite = nullptr;

    std::vector<std::shared_ptr<Attack_Hitbox>> Attacks;

    [[nodiscard]] float DamageCalculation() const;

public:
    virtual ~Tool() = default;
    Tool(const std::string& name_, float Damage_, float Cooldown_, float Range_ = 10000.0f, int Critical_Chance_ = 0, int Burst_Count_ = 1, float Burst_Delay_ = 0.5f);
    friend std::ostream & operator<<(std::ostream & out, const Tool & object);
    const std::string& GetName();
    [[nodiscard]] const std::vector<std::shared_ptr<Attack_Hitbox>>& GetAttackHitboxes() const;
    void ShowHitboxes(sf::RenderWindow& window) const;
    float GetCooldown() const;
    [[nodiscard]] virtual std::shared_ptr<Tool> clone() const;
    void Update(float deltaTime);
    void ClearAttackHitboxes();

    virtual void CreateAttackHitbox(sf::Vector2f Position, sf::Vector2f Offset, sf::Angle Degrees);
    virtual void DisplayInfo(std::ostream &out) const;
    float Attack(const sf::RectangleShape &Sprite, sf::Angle Degrees);
    void AddStat(const std::string &StatName, float Value);
    void ResetStats();
};


#endif //OOP_TOOL_H