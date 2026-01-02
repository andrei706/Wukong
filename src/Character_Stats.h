
#ifndef OOP_CHARACTER_STATS_H
#define OOP_CHARACTER_STATS_H

#include <iostream>
#include "Game_Exceptions.h"

class Character_Stats {
    float MaxHealth, Health, Speed;
    int Mana;

    float Defense, DamageReduction;
    float BackupMaxHealth;
public:
    explicit Character_Stats(float MaxHealth_ = 10, float Speed_ = 6, int Mana_ = 5, float Defense_ = 0, float DamageReduction_ = 0);
    friend std::ostream & operator<<(std::ostream & out, const Character_Stats & object);
    [[nodiscard]] float GetSpeed() const;
    [[nodiscard]] float GetHealth() const;
    [[nodiscard]] int GetMana() const;
    float ReduceHealth(float DamagePoints);
    void RestoreHealth(float HealthPoints);
    void AddStat(const std::string &StatName, float Value);
    void ResetStats();
};


#endif //OOP_CHARACTER_STATS_H