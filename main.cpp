#include <iostream>
#include <fstream>
#include <array>
#include <cmath>
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
        out<<object.MaxHealth<<" "<<object.Health<<" "<<object.Speed<<" "<<object.Mana;
        return out;
    }

    [[nodiscard]] float GetSpeed() const {
        return Speed;
    }
    int ReduceHealth(float DamagePoints) {
        if (Health - DamagePoints <= 0) {
            Health = 0;
            return 0; //Alive state
        }
        Health -= DamagePoints;
        return 1;
    }
    float GetHealth() const {
        return Health;
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

    std::string NameGetter() {
        return Name;
    }

    [[nodiscard]] float DamageCalculation() const {
        srand(time(NULL));
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

    sf::RectangleShape Sprite;
    sf::Vector2f Position = {200.f, 100.f};
public:

    explicit Enemy(const std::string& name_) : Name(name_) {
        Sprite.setFillColor(sf::Color::Red);
        Sprite.setPosition(Position);
        Sprite.setSize({50, 50});
    }
    Enemy(const Enemy& other)
        : Name(other.Name), Stats(other.Stats), Weapon(other.Weapon), Sprite(other.Sprite) {}
    ~Enemy() {
        std::cout << Name << " Destroyed\n";
    }
    friend std::ostream & operator<<(std::ostream & out, const Enemy & object) {
        out<<object.Name<<"\n"<<object.Stats<<"\n"<<object.Weapon;
        return out;
    }

    sf::RectangleShape& GetSprite() {
        return Sprite;
    }
    float GetDamage() {
        return Weapon.DamageCalculation();
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
    void SetPosition(float x, float y) {
        Position.x = x;
        Position.y = y;
        Sprite.setPosition(Position);
    }
    Enemy& operator=(const Enemy& other) {
        Name = other.Name;
        Stats = other.Stats;
        Weapon = other.Weapon;
        Sprite = other.Sprite;
        return *this;
    }


    void ShowSprite(sf::RenderWindow& window) {
        window.draw(Sprite);
    }
};

class Player_Class {
    int Experience, Gauge = 0;
    bool Invincibility = 0;
    Character_Stats Stats{30, 15, 0};
    Tool
        PoleShortRange{"Pool Short Range", 1, 0.4f, 2},
        PoleLongRange{"Pole Long Range", 4, 2, 10};

    sf::RectangleShape Sprite;
    sf::Vector2f Position = {100.f, 100.f};
    sf::Vector2f Size = {50.f, 50.f};

    sf::Time InvincibilityTime = sf::seconds(1.0f);
    sf::Clock ClockInvincibilityTime;

private:
    bool MakeInvincibile() {
        if (Invincibility == 1) {
            if (ClockInvincibilityTime.getElapsedTime() >= InvincibilityTime) {
                Invincibility = 0;
            }
            return 1;
        }
        ClockInvincibilityTime.restart();
        Invincibility = 1;
        return 0;
    }

public:
    Player_Class(int Experience_) : Experience(Experience_) {

        Sprite.setSize(Size);
        Sprite.setOrigin({Size.x / 2, Size.y / 2});
        Sprite.setFillColor(sf::Color::Blue);
        Sprite.setPosition(Position);


        Gauge = 0;
        Invincibility = 0;
        Experience = 0;
    };

    void ShowSprite(sf::RenderWindow& window) {
        window.draw(Sprite);
    }

    sf::RectangleShape& GetSprite() {
        return Sprite;
    }

    float GetHealth() const {
        return Stats.GetHealth();
    }

    void AddExperience(int Value) {
        Experience += Value;
    }

    void TakeDamage(float Value) {
        if (!MakeInvincibile()) {
            Stats.ReduceHealth(Value);
        }
    }

    void HandleActions(sf::RenderWindow &window, float deltaTime = 0.16) {

        //Movement
        sf::Vector2f movement(0.f, 0.f);
        float speed = Stats.GetSpeed();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            movement.y -= speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            movement.y += speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            movement.x -= speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            movement.x += speed * deltaTime;

        Sprite.move(movement);

        sf::Vector2i MousePos = sf::Mouse::getPosition(window);
        sf::Vector2f MouseWorldPos = sf::Vector2f(MousePos.x, MousePos.y);
        sf::Vector2f PlayerPosition = Sprite.getPosition();
        sf::Vector2f diff = MouseWorldPos - PlayerPosition;

        float radians = std::atan2(diff.y, diff.x);
        float angleDegrees = radians * 180.f / 3.14;
        Sprite.setRotation(sf::degrees(angleDegrees));
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

class GUI_TextLabel {
    sf::Font TextFont;
    sf::Text TextValue;
    sf::Vector2f Position = {100.f, 100.f};

    std::string Name;
    bool Status = 1;
public:
    explicit GUI_TextLabel(sf::Text TextValue_,std::string Name_ = "TextLabel",  std::string FontPath = "data/fonts/arial.ttf", int TextSize = 10, sf::Color TextColor = sf::Color::Black)
    : TextValue(TextValue_), Name(Name_){
        if (!TextFont.openFromFile(FontPath)) {
            std::cout<<"Error: Font not found for Text Label, must give the path to font_name.ttf.";
        }
        TextValue.setFont(TextFont);
        TextValue.setCharacterSize(TextSize);
        TextValue.setFillColor(TextColor);
    }

    void SetText(std::string TextValue_) {
        TextValue.setString(TextValue_);
    }

    void SetPosition(sf::Vector2f Position_) {
        Position = Position_;
    }

    void SetColor(sf::Color TextColor) {
        TextValue.setFillColor(TextColor);
    }

    void SetSize(int Size_) {
        TextValue.setCharacterSize(Size_);
    }

    void SetName(std::string Name_) {
        Name = Name_;
    }

    std::string GetName() {
        return Name;
    }

    bool GetStatus() {
        return Status;
    }

    void ToggleActive() {
        Status = !Status;
    }

    void ShowSprite(sf::RenderWindow& window) {
        window.draw(TextValue);
    }
};

class Game_Class {
    sf::RenderWindow& window;
    Player_Class &player;

    std::vector<Enemy> EnemyList, SpawnedEnemies;
    std::vector<Tool> ToolList;
    std::vector<GUI_TextLabel> TextLabelList;

private:
    void RenderEntities() {
        //Render Player
        player.ShowSprite(window);
        //Render Enemies
        for (auto &i : SpawnedEnemies) {
            i.ShowSprite(window);
        }
        //Render GUI
        for (auto &i : TextLabelList) {
            if (i.GetStatus())
                i.ShowSprite(window);
        }
    }

    void ReadData() {
        //Eventual fisierele txt vor fi inlocuite cu json-uri
        std::ifstream File1("data/EnemyList.txt");
        std::ifstream File2("data/ToolList.txt");

        std::string Name, WeaponName;
        float Damage, Cooldown;
        int Critical_Chance;
        while (File2 >> WeaponName >> Damage >> Cooldown >> Critical_Chance) {
            Tool WeaponAux{WeaponName, Damage, Cooldown, Critical_Chance};
            ToolList.push_back(WeaponAux);
        }
        File2.close();

        float MaxHealth, Speed;
        int Mana;
        while (File1 >> Name >> MaxHealth >> Speed >> Mana >> WeaponName) {
            Enemy EnemyAux{Name};
            Character_Stats StatsAux{MaxHealth, Speed, Mana};
            EnemyAux.AssignStats(StatsAux);
            for (auto &Weapon : ToolList)
                if (Weapon.NameGetter() == WeaponName) {
                    EnemyAux.AssignWeapon(Weapon);
                    break;
                }
            EnemyList.push_back(EnemyAux);
        }
        File1.close();

    }

    void UpdateHealthbar() {
        for (auto &i : TextLabelList) {
            if (i.GetName() == "Health") {
                i.SetText("Health: " + std::to_string(player.GetHealth()));
            }
        }
    }

    void EventHandler() {
        //Collision Verification
        //Between Player and Enemy
        sf::FloatRect PlayerBounds = player.GetSprite().getGlobalBounds();
        for (auto &i : SpawnedEnemies) {
            sf::FloatRect EnemyBounds = i.GetSprite().getGlobalBounds();

            if (EnemyBounds.findIntersection(PlayerBounds)) {
                player.TakeDamage(i.GetDamage());
            }
        }

        //Player Events
        player.HandleActions(window);

        UpdateHealthbar();
    }

    void WindowRendering() {
        window.setVerticalSyncEnabled(false);
        window.setFramerateLimit(60);
        while (window.isOpen()) {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();

            }

            EventHandler();

            //Rendering
            window.clear(sf::Color::White);
            RenderEntities();
            window.display();
        }
    }

public:
    Game_Class(sf::RenderWindow& window_, Player_Class& player_) : window(window_), player(player_) {}

    void Setup() {
        ReadData();
        SpawnedEnemies.insert(SpawnedEnemies.begin(), {EnemyList[0], EnemyList[1]});
        SpawnedEnemies[0].SetPosition(500, 200);
        SpawnedEnemies[1].SetPosition(500, 300);

        sf::Font font("data/fonts/arial.ttf");
        sf::Text text(font);
        text.setString("Hello World!");
        GUI_TextLabel textLabel(text, "Health", "data/fonts/arial.ttf", 20);
        TextLabelList.push_back(textLabel);

        WindowRendering();
    }

};

int main() {

    sf::RenderWindow window;
    window.create(sf::VideoMode({800, 600}), "Wukong");

    Player_Class Player{1};
    Game_Class Game{window, Player};
    Game.Setup();

    return 0;
}
