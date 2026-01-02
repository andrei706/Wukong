

#include "Key_Manager.h"

Key_Manager::Key_Manager(bool ActionButton_, bool PauseButton_): ActionButton(ActionButton_), PauseButton(PauseButton_) {}

bool Key_Manager::CheckButton(const std::string &KeyName) const {
    if (KeyName == "LeftMouseButton" || KeyName == "RightMouseButton" || KeyName == "Space" || KeyName == "F") {
        return ActionButton;
    }
    if (KeyName == "Escape") {
        return PauseButton;
    }
    std::cerr << "Invalid Button";
    return false;
}

void Key_Manager::ToggleActivation(const std::string &KeyName) {
    if (KeyName == "LeftMouseButton" || KeyName == "RightMouseButton" || KeyName == "Space" || KeyName == "F") {
        ActionButton = !ActionButton;
    }
    else if (KeyName == "Escape") {
        PauseButton = !PauseButton;
    }
    else std::cerr << "Invalid Button";
}

bool Key_Manager::CheckKeyPressed(const std::string &KeyName) const {
    if (KeyName == "LeftMouseButton" && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        return true;
    if (KeyName == "RightMouseButton" && sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        return true;
    if (KeyName == "F" &&sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F))
        return true;
    if (KeyName == "Escape" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
        return true;
    if (KeyName == "Space" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        return true;
    return false;
}

bool Key_Manager::CheckInput(const std::string &input) {
    if (CheckKeyPressed(input)) {
        if (!CheckButton(input)) {
            ToggleActivation(input);
            return true;
        }
    }
    else
        if (CheckButton(input))
            ToggleActivation(input);
    return false;
}

std::ostream & operator<<(std::ostream &os, const Key_Manager &obj) {
    return os
           << "ActionButton: " << obj.ActionButton
           << " PauseButton: " << obj.PauseButton;
}
