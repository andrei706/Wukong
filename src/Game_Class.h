#ifndef OOP_GAME_CLASS_H
#define OOP_GAME_CLASS_H
#include "Player_Class.h"
#include "Game_Exceptions.h"
#include "Random_Value_Generator.h"
#include "Enviroment_Object.h"
#include "Tool_Factory.h"
#include "Enemy_Builder.h"
#include "GUI_TextLabel.h"
#include "GUI_Button.h"
#include "GUI_UpgradeMenu.h"
#include "Key_Manager.h"
#include "Wave_Manager.h"
#include "Spawn_Warning.h"
#include "Template_Function.h"


#include <iostream>
#include <fstream>
#include <random>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Exception.hpp>
#include <memory>

class Game_Class {
    sf::RenderWindow& window;
    sf::RectangleShape PauseTint;
    float dt = 0.f;
    float dtMultiplier = 62.5f;
    bool isPaused = false;
    Player_Class &player;
    Key_Manager KeyManager;
    Wave_Manager WaveManager;
    sf::Vector2f windowSize = window.getDefaultView().getSize();
    sf::View view;
    sf::Font GameFont;
    sf::Clock GameClock;

    float totalRunTime = 0.f;
    bool gameBeaten = false;

    std::vector<Spawn_Warning> ActiveSpawnWarnings;
    std::vector<std::shared_ptr<Enemy>> EnemyList, SpawnedEnemies;
    std::vector<std::shared_ptr<Tool>> ToolList;
    std::vector<std::shared_ptr<Attack_Hitbox>> PlayerAttackHitbox;
    std::vector<GUI_TextLabel> TextLabelList;
    std::vector<std::pair<GUI_TextLabel, float>> DamageCounter;
    std::vector<GUI_Button> PauseButtonList;
    std::vector<std::shared_ptr<Enviroment_Object>> MapBlocks;
    GUI_UpgradeMenu UpgradeMenu;

    bool PlayerLost = false;

private:
    std::string CalculateRank() const;
    void RenderEntities() const;
    void ReadData();
    void UpdateHealthbar();
    void UpdateSpawnWarnings();
    void Replay();
    void PauseHandler();
    void SpawnEnemy(const std::string& Name, sf::Vector2f Position);
    void AdjustView(unsigned int newWidth, unsigned int newHeight);
    void EventHandler();
    void WindowRendering();
    void WaveHandler();
    void ToggleRotatorRotation() const;
    void AddDamageCounter(float damage, sf::Vector2f position, sf::Color color);
    void UpdateDamageCounters();
    void AddTextLabel(const std::string& name, const std::string& content,
                      sf::Vector2f position, int size,
                      sf::Color color, bool isActive = true, bool OutlineEnabled = false);
    void SetupMap();

public:
    Game_Class(sf::RenderWindow& window_, Player_Class& player_);
    friend std::ostream & operator<<(std::ostream & out, const Game_Class & object);
    void Setup();
};


#endif //OOP_GAME_CLASS_H