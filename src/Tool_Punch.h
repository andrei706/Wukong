

#ifndef OOP_TOOL_PUNCH_H
#define OOP_TOOL_PUNCH_H

#include "Tool.h"

class Tool_Punch : public Tool {
    float PunchRadius = 30.0f;
public:
    Tool_Punch(const std::string &name_, float Damage_, float Cooldown_, float Range_,
                   int Critical_Chance_, float PunchRadius_ = 30.0f,
                   int burstCount = 1, float burstDelay = 0.1f);

    [[nodiscard]] std::shared_ptr<Tool> clone() const override;

    void CreateAttackHitbox(sf::Vector2f Position, sf::Vector2f Offset, sf::Angle Degrees) override;

    void DisplayInfo(std::ostream &out) const override;
};


#endif //OOP_TOOL_PUNCH_H