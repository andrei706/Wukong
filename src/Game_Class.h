#ifndef OOP_GAME_CLASS_H
#define OOP_GAME_CLASS_H
#include "Player_Class.h"
#include "Enemy.h"
#include "GUI_TextLabel.h"
#include "GUI_Button.h"
#include "Key_Manager.h"

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Game_Class {
    sf::RenderWindow& window;
    float dt = 0.f;
    float dtMultiplier = 62.5f;
    bool isPaused = false;
    Player_Class &player;
    Key_Manager KeyManager;

    sf::Vector2f windowSize = window.getDefaultView().getSize();
    sf::View view;

    sf::Clock GameClock, ActionClock;
    float ActionCooldown = 0;

    std::vector<Enemy> EnemyList, SpawnedEnemies;
    std::vector<Tool> ToolList;
    std::vector<Attack_Hitbox> PlayerAttackHitbox;
    std::vector<GUI_TextLabel> TextLabelList;
    std::vector<GUI_Button> PauseButtonList;

    bool PlayerLost = false;

private:

    void RenderEntities() const;
    void ReadData();
    void UpdateHealthbar();
    void Replay();
    void PauseHandler();

    void AdjustView(sf::RenderWindow &window, unsigned int newWidth, unsigned int newHeight);

    void EventHandler();
    void WindowRendering();

public:
    Game_Class(sf::RenderWindow& window_, Player_Class& player_);
    friend std::ostream & operator<<(std::ostream & out, const Game_Class & object);
    void Setup();
};


#endif //OOP_GAME_CLASS_H