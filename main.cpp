#include <iostream>
#include <array>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Character_Stats {
    float MaxHealth, Health, Speed;
    int Mana;

public:

    explicit Character_Stats(float MaxHealth_ = 10, float Speed_ = 6, int Mana_ = 5)
        : MaxHealth(MaxHealth_), Speed(Speed_), Mana(Mana_) {
        Health = MaxHealth;
    }
    friend std::ostream & operator<<(std::ostream & out, const Character_Stats & object) {
        out<<object.MaxHealth<<" "<<object.Speed<<" "<<object.Mana;
        return out;
    }

    int ReduceHealth(float DamagePoints) {
        if (Health - DamagePoints <= 0) {
            Health = 0;
            return 0; //Alive state
        }
        Health -= DamagePoints;
        return 1;
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

    [[nodiscard]] float DamageCalculation() const {
        int Chance = rand() % 100;
        if (Chance < Critical_Chance) {
            return (float)Damage + 0.5 * Damage;
        }
        return Damage;
    }
};

class Enemy {
    std::string Name;
    Character_Stats Stats{10, 5, 5 };
    Tool Weapon{"Sword", 3, 0.6f, 10};
public:

    explicit Enemy(const std::string& name_) : Name(name_) {}
    Enemy(const Enemy& other)
        : Name(other.Name), Stats(other.Stats), Weapon(other.Weapon) {}
    ~Enemy() {
        std::cout << Name << " Destroyed\n";
    }
    friend std::ostream & operator<<(std::ostream & out, const Enemy & object) {
        out<<object.Name<<"\n"<<object.Stats<<"\n"<<object.Weapon;
        return out;
    }

    void TakeDamage (float Damage_Points) {
        Stats.ReduceHealth(Damage_Points);
    }
    void AssignWeapon (const Tool& other) {
        Weapon = other;
    }
    void AssignStats (const Character_Stats& other) {
        Stats = other;
    }
    Enemy& operator=(const Enemy& other) {
        Name = other.Name;
        Stats = other.Stats;
        Weapon = other.Weapon;
        return *this;
    }
};

class Player {
    int Exp;
    Character_Stats Stats{10, 6, 5};
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

    sf::Window window;
    window.create(sf::VideoMode({800, 600}), "Wukong");
    window.setFramerateLimit(60);

    //Main loop
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

        }
    }

    return 0;
}
