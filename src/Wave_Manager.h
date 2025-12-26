
#ifndef OOP_WAVE_MANAGER_H
#define OOP_WAVE_MANAGER_H

#include <utility>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <ostream>
#include <nlohmann/json.hpp>

#include "Game_Exceptions.h"

class Wave {
    int Index, MaxEnemiesOnDisplay;
    std::vector<std::pair<std::string, int>> Enemies;
public:
    Wave(int Number_, int MaxEnemiesOnDisplay_, std::vector<std::pair<std::string, int>> Enemies_);

    [[nodiscard]] int GetEnemiesOnDisplay() const;

    [[nodiscard]] int GetIndex() const;

    friend std::ostream & operator<<(std::ostream &os, const Wave &obj);

    std::string GetNextEnemy();

    bool IsFinished() const;
};

class Wave_Manager {
    std::vector<Wave> allWaves;
    int CurrentWave = 0;

public:
    explicit Wave_Manager(int StartWave = 1);

    void LoadWaves(const std::string& path);

    void Reset(const std::string& path);

    Wave* GetCurrentWave();

    friend std::ostream & operator<<(std::ostream &os, const Wave_Manager &obj);

    void MoveToNextWave();
};


#endif