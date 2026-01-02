
#include "GUI_UpgradeMenu.h"

int GUI_UpgradeMenu::CalculateCost(int currentPoints, const std::string &name) {
    if (name == "RestoreHealth") {
        return 15;
    }
    return 10 + (currentPoints * 5);
}

GUI_UpgradeMenu::GUI_UpgradeMenu() {

    applyButton = std::make_shared<GUI_Button>("Apply", "data/textures/buttons/Apply_Button.png", sf::Vector2f(430, 490));
    revertButton = std::make_shared<GUI_Button>("Revert", "data/textures/buttons/Revert_Button.png", sf::Vector2f(680, 490));

    menuBackground.setPosition({315.f, 50.f});
    menuBackground.setSize(sf::Vector2f(650.f, 510.f));
    menuBackground.setFillColor(sf::Color(218, 160, 109, 245));
    menuBackground.setOutlineThickness(3.f);
    menuBackground.setOutlineColor(sf::Color(255, 215, 0));

    menuTitle = std::make_unique<GUI_TextLabel>("UPGRADES", "data/fonts/Tiny5-Regular.ttf", 35, sf::Color::Black);
    menuTitle->SetPosition({510, 65});
    menuTitle->SetText("UPGRADES");

    expLabel = std::make_unique<GUI_TextLabel>("EXP", "data/fonts/Tiny5-Regular.ttf", 20, sf::Color::Cyan);
    expLabel->SetPosition({330, 65});

    for (int i = 0; i < 6; ++i) {
        float yPos = 130.f + (i * 55.f);
        pendingUpgrades.emplace_back(statNames[i], 0.0f);
        baseValues.push_back(0.0f);

        statLabels.emplace_back(statNames[i], "data/fonts/Tiny5-Regular.ttf", 22, sf::Color::Black);
        statLabels.back().SetPosition({330, yPos});

        descriptionLabels.emplace_back(statNames[i] + "_Desc", "data/fonts/Tiny5-Regular.ttf", 18, sf::Color(60, 60, 60));
        descriptionLabels.back().SetText(statDescriptions[i]);
        descriptionLabels.back().SetPosition({330, yPos + 10});

        valueLabels.emplace_back(statNames[i] + "_Val", "data/fonts/Tiny5-Regular.ttf", 22, sf::Color::Yellow);
        valueLabels.back().SetPosition({790, yPos});

        plusButtons.push_back(std::make_unique<GUI_Button>(
            "Plus_" + statNames[i],
            "data/textures/buttons/Plus_Button.png",
            sf::Vector2f{890.f, yPos - 10},
            sf::Vector2f{45.f, 45.f}
        ));

        costLabels.emplace_back(statNames[i] + "_Cost", "data/fonts/Tiny5-Regular.ttf", 16, sf::Color(200, 0, 0));
        costLabels.back().SetPosition({940, yPos});
    }
}

void GUI_UpgradeMenu::Toggle() { isActive = !isActive; }

void GUI_UpgradeMenu::HandleInput(sf::Vector2f mousePos, Key_Manager &km, Player_Class &player) {
    if (!isActive) return;

    int totalPendingCost = 0;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < (int)pendingUpgrades[i].second; ++j) {
            totalPendingCost += CalculateCost((int)baseValues[i] + j, statNames[i]);
        }
    }

    for (int i = 0; i < 6; ++i) {
        if (plusButtons[i]->isClicked(mousePos, km)) {
            int nextPointCost = CalculateCost((int)baseValues[i] + (int)pendingUpgrades[i].second, statNames[i]);
            if (player.GetExperience() >= (totalPendingCost + nextPointCost)) {
                pendingUpgrades[i].second += 1.0f;
            }
        }
    }

    if (applyButton->isClicked(mousePos, km)) {
        player.AddExperience(-totalPendingCost);

        player.ApplyUpgrades(pendingUpgrades);
        for (int i = 0; i < 6; ++i) {
            baseValues[i] += pendingUpgrades[i].second;
        }
        RevertChanges();
    }

    if (revertButton->isClicked(mousePos, km)) {
        RevertChanges();
    }
}

void GUI_UpgradeMenu::RevertChanges() { for (auto& pair : pendingUpgrades) pair.second = 0.0f; }

void GUI_UpgradeMenu::UpdateLabels(const Player_Class &player) {
    if (!isActive) return;

    int totalPendingCost = 0;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < (int)pendingUpgrades[i].second; ++j) {
            totalPendingCost += CalculateCost((int)baseValues[i] + j);
        }
    }

    expLabel->SetText("EXP: " + std::to_string(player.GetExperience() - totalPendingCost));

    for (int i = 0; i < 6; ++i) {
        float currentDisplay = baseValues[i] + pendingUpgrades[i].second;
        float pending = pendingUpgrades[i].second;
        valueLabels[i].SetText(std::to_string(static_cast<int>(currentDisplay)) +
                               " (+" + std::to_string(static_cast<int>(pending)) + ")");

        int nextCost = CalculateCost((int)currentDisplay, statNames[i]);
        costLabels[i].SetText(std::to_string(nextCost));
    }
}

void GUI_UpgradeMenu::Render(sf::RenderWindow &window) const {
    if (!isActive) return;
    window.draw(menuBackground);
    menuTitle->ShowSprite(window);
    expLabel->ShowSprite(window);

    for (auto& l : statLabels) l.ShowSprite(window);
    for (auto& d : descriptionLabels) d.ShowSprite(window);
    for (auto& l : valueLabels) l.ShowSprite(window);
    for (auto& c : costLabels) c.ShowSprite(window);
    for (auto& b : plusButtons) b->ShowSprite(window);
    applyButton->ShowSprite(window);
    revertButton->ShowSprite(window);
}

void GUI_UpgradeMenu::Reset() {
    for (auto &i : baseValues ) {
        i = 0.0f;
    }
    RevertChanges();
}

bool GUI_UpgradeMenu::IsActive() const { return isActive; }
