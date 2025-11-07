
#ifndef OOP_KEY_MANAGER_H
#define OOP_KEY_MANAGER_H

//Rolul este de a verifica daca un input este apasat in continuu

#include <ostream>
#include <vector>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Key_Manager {
    bool ActionButton = false, PauseButton = false;
    [[nodiscard]] bool CheckButton(const std::string &KeyName) const;
    void ToggleActivation(const std::string &KeyName);
    bool CheckKeyPressed(const std::string &KeyName) const;

public:
    explicit Key_Manager(bool ActionButton_ = false, bool  PauseButton_ = false);
    friend std::ostream & operator<<(std::ostream &os, const Key_Manager &obj);
    bool CheckInput(const std::string &input);
};


#endif //OOP_KEY_MANAGER_H