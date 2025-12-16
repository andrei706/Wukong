
#include "Game_Exceptions.h"

GameException::GameException(const std::string &message)
    : std::runtime_error("[Game Error]: " + message) {}

AssetMissingException::AssetMissingException(const std::string &filename)
    : GameException("Asset missing file: " + filename) {}

JsonMissingAttributeException::JsonMissingAttributeException(const std::string &filename,
    const std::string &AttributeName): GameException("Missing attribute from " + filename + ".json: " + AttributeName) {}

InvalidActionException::InvalidActionException(const std::string &action)
    : GameException("Illegal gameplay action attempted: " + action) {}

InvalidDataException::InvalidDataException(const std::string &varName, float value)
    : GameException("Invalid data value for " + varName + ": " + std::to_string(value)) {}
