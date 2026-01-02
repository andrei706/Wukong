
#ifndef OOP_TOOL_FACTORY_H
#define OOP_TOOL_FACTORY_H

#include "Tool.h"
#include "Tool_Punch.h"
#include "Tool_Ranged.h"

#include <memory>
#include <nlohmann/json.hpp>

class Tool_Factory {
public:
    static std::shared_ptr<Tool> createWeapon(const nlohmann::json &data);
    static std::shared_ptr<Tool_Punch> createPunch(const nlohmann::json &data);
    static std::shared_ptr<Tool_Ranged> createRanged(const nlohmann::json &data);
};


#endif //OOP_TOOL_FACTORY_H