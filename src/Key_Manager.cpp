

#include "Key_Manager.h"

Key_Manager::Key_Manager(bool ActionButton_, bool PauseButton_): ActionButton(ActionButton_), PauseButton(PauseButton_) {}

bool Key_Manager::CheckButton(const std::string &KeyName) const {
    if (KeyName == "LeftMouseButton" || KeyName == "RightMouseButton") {
        return ActionButton;
    }
    if (KeyName == "Escape") {
        return PauseButton;
    }
    std::cerr << "Invalid Button";
    return false;
}

void Key_Manager::ToggleActivation(const std::string &KeyName) {
    if (KeyName == "LeftMouseButton" || KeyName == "RightMouseButton") {
        ActionButton = !ActionButton;
    }
    else if (KeyName == "Escape") {
        PauseButton = !PauseButton;
    }
    else std::cerr << "Invalid Button";
}

std::ostream & operator<<(std::ostream &os, const Key_Manager &obj) {
    return os
           << "ActionButton: " << obj.ActionButton
           << " PauseButton: " << obj.PauseButton;
}
