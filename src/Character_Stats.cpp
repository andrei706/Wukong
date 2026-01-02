
#include "Character_Stats.h"

Character_Stats::Character_Stats(float MaxHealth_, float Speed_, int Mana_, float Defense_, float DamageReduction_):
    MaxHealth(MaxHealth_), Speed(Speed_), Mana(Mana_), Defense(Defense_), DamageReduction(DamageReduction_) {
    Health = MaxHealth;
    BackupMaxHealth = MaxHealth;
    if (Speed < 0) throw InvalidDataException("Speed", Speed);
    if (Mana < 0) throw InvalidDataException("Mana", Mana);
    if (MaxHealth < 0) throw InvalidDataException("MaxHealth", MaxHealth);
}

float Character_Stats::GetSpeed() const {
    return Speed;
}

bool Character_Stats::ReduceHealth(float DamagePoints) {
    float CalculatedDamage = DamagePoints - DamagePoints * DamageReduction / 100 - Defense;
    if (Health - CalculatedDamage <= 0) {
        Health = 0;
        return false; //Alive state
    }
    Health -= CalculatedDamage;
    return true;
}

void Character_Stats::RestoreHealth(float HealthPoints) {
    if (Health + HealthPoints >= MaxHealth) {
        Health = MaxHealth;
        return;
    }
    Health += HealthPoints;
}

void Character_Stats::AddStat(const std::string &StatName, float Value) {
    if (StatName == "MaxHealth") {
        MaxHealth += Value;
        Health += Value;
        if (MaxHealth < 0) throw InvalidDataException("MaxHealth (In AddStat Function)", MaxHealth);
    }
    if (StatName == "Defense") {
        Defense += Value;
        if (Defense < 0)
            Defense = 0;
    }
    if (StatName == "DamageReduction") {
        DamageReduction += Value;
        if (DamageReduction < 0)
            DamageReduction = 0;
        if (DamageReduction > 100)
            DamageReduction = 100;
    }
}

void Character_Stats::ResetStats() {
    MaxHealth = BackupMaxHealth;
    if (Health > MaxHealth) {
        Health = MaxHealth;
    }
    DamageReduction = 0;
    Defense = 0;
}

float Character_Stats::GetHealth() const {
    return Health;
}

int Character_Stats::GetMana() const {
    return Mana;
}

std::ostream & operator<<(std::ostream &out, const Character_Stats &object) {
    out<<object.MaxHealth<<" "<<object.Health<<" "<<object.Speed<<" "<<object.Mana;
    return out;
}
