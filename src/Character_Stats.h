
#ifndef OOP_CHARACTER_STATS_H
#define OOP_CHARACTER_STATS_H

#include <iostream>


class Character_Stats {
    float MaxHealth, Health, Speed;
    int Mana;

public:

    explicit Character_Stats(float MaxHealth_ = 10, float Speed_ = 6, int Mana_ = 5);
    friend std::ostream & operator<<(std::ostream & out, const Character_Stats & object);
    [[nodiscard]] float GetSpeed() const;
    [[nodiscard]] float GetHealth() const;
    bool ReduceHealth(float DamagePoints);

    void RestoreHealth(float HealthPoints);
};


#endif //OOP_CHARACTER_STATS_H