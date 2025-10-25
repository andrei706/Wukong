#include <iostream>
#include <array>
#include <vector>
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

class Player_Class {
    int Experience, Gauge = 0;
    bool Invincibility = 0;
    sf::Time InvincibilityTime = sf::seconds(0.4f);
    Character_Stats Stats{30, 6, 0};
    Tool
        PoleShortRange{"Pool Short Range", 1, 0.4f, 2},
        PoleLongRange{"Pole Long Range", 4, 2, 10};

    sf::RectangleShape Sprite;

private:
    //trebuie regandita, probabil fac frame based verification event
    // void MakeInvincible(sf::Time Interval) {
    //     Invincibility = true;
    //     sf::Clock clock;
    //     sf::Time elapsed1 = clock.getElapsedTime();
    //     sf::Time elapsed2 = clock.getElapsedTime();
    //     while (elapsed2 - elapsed1 < Interval) {
    //         elapsed2 = clock.getElapsedTime();
    //     }
    //     Invincibility = false;
    // }

public:
    Player_Class(int Experience_) : Experience(Experience_) {
        Sprite.setSize(sf::Vector2f(50.f, 50.f));
        Sprite.setFillColor(sf::Color::Blue);
        Sprite.setPosition({100.f, 100.f});
        Gauge = 0;
        Invincibility = 0;
        Experience = 0;
    };

    void Show_Sprite(sf::RenderWindow& window) {
        window.draw(Sprite);
    }

    void Add_Experience(int Value) {
        Experience += Value;
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

class Game {
    sf::RenderWindow& window;
    Player_Class &player;

public:
    Game(sf::RenderWindow& window_, Player_Class& player_) : window(window_), player(player_) {}

    void WindowRendering() {
        window.setFramerateLimit(60);
        while (window.isOpen()) {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();

            }
            window.clear(sf::Color::Black);

            //here we draw
            //window.draw(player->Sprite);
            player.Show_Sprite(window);

            window.display();
        }
    }
};

int main() {
    srand(time(0));

    sf::RenderWindow window;
    window.create(sf::VideoMode({800, 600}), "Wukong");

    Player_Class Player{1};
    Game myGame{window, Player};
    myGame.WindowRendering();

    return 0;
}
