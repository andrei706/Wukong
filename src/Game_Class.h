#ifndef OOP_GAME_CLASS_H
#define OOP_GAME_CLASS_H
#include "Player_Class.h"
#include "Game_Exceptions.h"
#include "Enemy.h"
#include "Enemy_Walker.h"
#include "Enemy_Ranger.h"
#include "Enemy_Rotator.h"
#include "Enemy_Ambidextrous.h"
#include "GUI_TextLabel.h"
#include "GUI_Button.h"
#include "Key_Manager.h"
#include "Wave_Manager.h"
#include "Spawn_Warning.h"


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
    float dt = 0.f;
    float dtMultiplier = 62.5f;
    bool isPaused = false;
    Player_Class &player;
    Key_Manager KeyManager;
    Wave_Manager WaveManager;
    sf::Vector2f windowSize = window.getDefaultView().getSize();
    sf::View view;

    sf::Clock GameClock;

    std::vector<Spawn_Warning> ActiveSpawnWarnings;
    std::vector<std::shared_ptr<Enemy>> EnemyList, SpawnedEnemies;
    std::vector<std::shared_ptr<Tool>> ToolList;
    std::vector<std::shared_ptr<Attack_Hitbox>> PlayerAttackHitbox;
    std::vector<GUI_TextLabel> TextLabelList;
    std::vector<GUI_Button> PauseButtonList;

    bool PlayerLost = false;

    static float GetRandomValue(float min, float max);

private:
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

public:
    Game_Class(sf::RenderWindow& window_, Player_Class& player_);
    friend std::ostream & operator<<(std::ostream & out, const Game_Class & object);
    void Setup();
};


#endif //OOP_GAME_CLASS_H