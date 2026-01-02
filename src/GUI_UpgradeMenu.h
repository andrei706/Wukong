#ifndef OOP_GUI_UPGRADEMENU_H
#define OOP_GUI_UPGRADEMENU_H

#include <vector>
#include <utility>
#include <memory>
#include <string>
#include "GUI_Button.h"
#include "GUI_TextLabel.h"
#include "Player_Class.h"
#include "Key_Manager.h"

class GUI_UpgradeMenu {
    bool isActive = true;
    sf::RectangleShape menuBackground;

    std::unique_ptr<GUI_TextLabel> menuTitle;
    std::unique_ptr<GUI_TextLabel> expLabel;

    std::vector<GUI_TextLabel> statLabels;
    std::vector<GUI_TextLabel> valueLabels;
    std::vector<GUI_TextLabel> costLabels;
    std::vector<std::unique_ptr<GUI_Button>> plusButtons;
    std::vector<GUI_TextLabel> descriptionLabels;

    std::shared_ptr<GUI_Button> applyButton;
    std::shared_ptr<GUI_Button> revertButton;

    std::vector<std::pair<std::string, float>> pendingUpgrades;
    std::vector<std::string> statNames = {"MeleeWeapon", "RangedWeapon", "MaxHealth", "Defense", "SpiritBall", "RestoreHealth"};
    std::vector<std::string> statDescriptions = {
        "Increases basic pole damage.",
        "Enhances blast projectile power.",
        "Gives you more life points.",
        "Reduces incoming enemy damage.",
        "Increases max spirit energy.",
        "Restores a fraction of your current health."
    };
    std::vector<float> baseValues;

    static int CalculateCost(int currentPoints, const std::string& name = "");

public:
    GUI_UpgradeMenu();

    void Toggle();
    void HandleInput(sf::Vector2f mousePos, Key_Manager& km, Player_Class& player);
    void RevertChanges();
    void UpdateLabels(const Player_Class& player);
    void Render(sf::RenderWindow& window) const;
    void Reset();
    bool IsActive() const;
};

#endif