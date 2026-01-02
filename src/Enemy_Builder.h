
#ifndef OOP_ENEMY_BUILDER_H
#define OOP_ENEMY_BUILDER_H

#include "Enemy.h"
#include "Enemy_Walker.h"
#include "Enemy_Ranger.h"
#include "Enemy_Rotator.h"
#include "Enemy_Ambidextrous.h"
#include "Enemy_AgileBoss.h"

#include <nlohmann/json.hpp>

class Enemy_Builder {
    std::shared_ptr<Enemy> enemy;

public:
    Enemy_Builder() = default;

    Enemy_Builder& startNew(const nlohmann::json& data);
    Enemy_Builder& SetStats(float hp, float speed, int mana);
    Enemy_Builder& SetStats(const nlohmann::json& data);
    Enemy_Builder& SetMelee(const std::shared_ptr<Tool> &weapon);
    Enemy_Builder& SetRanged(const std::shared_ptr<Tool> &weapon);
    std::shared_ptr<Enemy> Build();
};


#endif //OOP_ENEMY_BUILDER_H