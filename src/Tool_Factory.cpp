

#include "Tool_Factory.h"
std::shared_ptr<Tool> Tool_Factory::createWeapon(const nlohmann::json &data) {
    if (!data.contains("Type")) return nullptr;

    std::string type = data.at("Type").get<std::string>();

    if (type == "Punch") {
        return createPunch(data);
    }
    if (type == "Ranged") {
        return createRanged(data);
    }

    return nullptr;
}

std::shared_ptr<Tool_Punch> Tool_Factory::createPunch(const nlohmann::json &data) {
    return std::make_shared<Tool_Punch>(
        data.at("WeaponName").get<std::string>(),
        data.at("Damage").get<float>(),
        data.at("Cooldown").get<float>(),
        data.at("Range").get<float>(),
        data.at("Critical_Chance").get<int>(),
        data.value("PunchRadius", 30.0f),
        data.value("Burst_Count", 1),
        data.value("Burst_Delay", 0.1f)
    );
}

std::shared_ptr<Tool_Ranged> Tool_Factory::createRanged(const nlohmann::json &data) {
    return std::make_shared<Tool_Ranged>(
        data.at("WeaponName").get<std::string>(),
        data.at("Damage").get<float>(),
        data.at("Cooldown").get<float>(),
        data.at("Range").get<float>(),
        data.at("Critical_Chance").get<int>(),
        data.value("Burst_Count", 1),
        data.value("Burst_Delay", 0.1f),
        data.value("ProjectileSize", 30.0f),
        data.value("ProjectileSpeed", 15.0f),
        data.value("Lifetime", 4.0f),
        data.value("DestroyProjectileOnHit", true)
    );
}
