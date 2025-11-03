
#include "Character_Stats.h"

Character_Stats::Character_Stats(float MaxHealth_, float Speed_, int Mana_): MaxHealth(MaxHealth_), Speed(Speed_), Mana(Mana_) {
    Health = MaxHealth;
}

float Character_Stats::GetSpeed() const {
    return Speed;
}

bool Character_Stats::ReduceHealth(float DamagePoints) {
    if (Health - DamagePoints <= 0) {
        Health = 0;
        return false; //Alive state
    }
    Health -= DamagePoints;
    return true;
}

float Character_Stats::GetHealth() const {
    return Health;
}

std::ostream & operator<<(std::ostream &out, const Character_Stats &object) {
    out<<object.MaxHealth<<" "<<object.Health<<" "<<object.Speed<<" "<<object.Mana;
    return out;
}
