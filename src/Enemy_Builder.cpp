//
// Created by Andrei on 12/30/2025.
//

#include "Enemy_Builder.h"

Enemy_Builder & Enemy_Builder::startNew(const nlohmann::json &data) {
    std::string type = data.at("Type").get<std::string>();
    std::string name = data.at("Name").get<std::string>();

    if (type == "Walker") enemy = std::make_shared<Enemy_Walker>(name);
    else if (type == "Ranged") enemy = std::make_shared<Enemy_Ranger>(name);
    else if (type == "Rotator") {
        enemy = std::make_shared<Enemy_Rotator>(
            name,
            data.at("RotationSpeed").get<float>(),
            data.at("IsRotatingRight").get<bool>()
        );
    }
    else if (type == "Ambidextrous") enemy = std::make_shared<Enemy_Ambidextrous>(name);
    else if (type == "AgileBoss") enemy = std::make_shared<Enemy_AgileBoss>(name);

    return *this;
}

Enemy_Builder & Enemy_Builder::SetStats(float hp, float speed, int mana) {
    if (enemy) {
        Character_Stats stats{hp, speed, mana};
        enemy->AssignStats(stats);
    }
    return *this;
}

Enemy_Builder & Enemy_Builder::SetStats(const nlohmann::json &data) {
    if (enemy) {
        Character_Stats stats{
            data.at("MaxHealth").get<float>(),
            data.at("Speed").get<float>(),
            data.at("Mana").get<int>()
        };
        enemy->AssignStats(stats);
    }
    return *this;
}

Enemy_Builder & Enemy_Builder::SetMelee(const std::shared_ptr<Tool> &weapon) {
    if (enemy && weapon) enemy->AssignWeapon(weapon, true);
    return *this;
}

Enemy_Builder & Enemy_Builder::SetRanged(const std::shared_ptr<Tool> &weapon) {
    if (enemy && weapon) enemy->AssignWeapon(weapon, false);
    return *this;
}

std::shared_ptr<Enemy> Enemy_Builder::Build() {
    return std::move(enemy);
}
