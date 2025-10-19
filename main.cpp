#include <iostream>
#include <array>
#include <string>

class Character_Stats {
    float Health, Speed;
    int Mana;
public:
    Character_Stats(float Health_, float Speed_, int Mana_)
        : Health(Health_), Speed(Speed_), Mana(Mana_) {}
    friend std::ostream & operator<<(std::ostream & out, const Character_Stats & object) {
        out<<object.Health<<" "<<object.Speed<<" "<<object.Mana;
        return out;
    }

};

class Tool {
    std::string Name;
    float Damage, Cooldown;
    int Critical_Chance;
public:
    Tool(const std::string& name_, float Damage_, float Cooldown_, int Critical_Chance_)
        : Name(name_), Damage(Damage_), Cooldown(Cooldown_), Critical_Chance(Critical_Chance_) {}
    friend std::ostream & operator<<(std::ostream & out, const Tool & object) {
        out<<object.Name<<" "<<object.Damage<<" "<<object.Cooldown<<" "<<object.Critical_Chance;
        return out;
    }
    float DamageCalculation() const {
        int Chance = rand() % 100;
        if (Chance < Critical_Chance) {
            return (float)Damage + 0.5 * Damage;
        }
        return Damage;
    }
};

class Character {
    std::string Name;
    Character_Stats Stats{10, 5, 5 };
    Tool Weapon{"Sword", 3, 0.6f, 10};
public:
    explicit Character(const std::string& name_) : Name(name_) {}
    Character(const Character& other)
        : Name(other.Name), Stats(other.Stats), Weapon(other.Weapon) {}
    friend std::ostream & operator<<(std::ostream & out, const Character & object) {
        out<<object.Name;
        return out;
    }
    ~Character() {
        std::cout << Name << " Destroyed\n";
    }
    Character& operator=(const Character& other) {
        Name = other.Name;
        Stats = other.Stats;
        Weapon = other.Weapon;
        return *this;
    }
};

class Enviroment_Object {
    bool isDestructable;
    float Health;
public:
    Enviroment_Object(bool isDestructable_, float Health_)
        : isDestructable(isDestructable_), Health(Health_) {}
    friend std::ostream & operator<<(std::ostream & out, const Enviroment_Object & object) {
        out<<object.isDestructable<<" "<<object.Health;
        return out;
    }
};


int main() {
    srand(time(0));

    return 0;
}
