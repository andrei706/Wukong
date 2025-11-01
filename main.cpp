#include <iostream>
#include <fstream>
#include <array>
#include <cmath>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Attack_Hitbox {
    float DamageValue;

    sf::RectangleShape Sprite;
    sf::Vector2f Size = {100, 20};
    sf::Vector2f Position = {100.f, 100.f};
    sf::Angle Rotation;

public:
    explicit Attack_Hitbox(float DamageValue_, sf::Vector2f Size_ = {100, 20}, sf::Vector2f Position_ = {0, 0}, sf::Angle Angle_ = sf::degrees(0))
    : DamageValue(DamageValue_), Size(Size_), Position(Position_), Rotation(Angle_)  {
        Sprite.setSize(Size_);
        Sprite.setOrigin({Size_.x / 2, Size_.y / 2});
        Sprite.setPosition(Position_);
        Sprite.setRotation(Angle_);

        Sprite.setFillColor(sf::Color::Magenta);

    }

    friend std::ostream & operator<<(std::ostream & out, const Attack_Hitbox & object) {
        out<<object.DamageValue<<"\n";
        return out;
    }

    float GetDamageValue() const {
        return DamageValue;
    }

    sf::FloatRect GetBounds() const {
        return Sprite.getGlobalBounds();
    }

    void ShowSprite(sf::RenderWindow& window) {
        window.draw(Sprite);
    }

};

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
    bool ReduceHealth(float DamagePoints) {
        if (Health - DamagePoints <= 0) {
            Health = 0;
            return false; //Alive state
        }
        Health -= DamagePoints;
        return true;
    }
    [[nodiscard]] float GetHealth() const {
        return Health;
    }
};

class Tool {
    std::string Name;
    float Damage, Cooldown, Range;
    int Critical_Chance;

    std::vector<Attack_Hitbox> Attacks;

public:
    Tool(const std::string& name_, float Damage_, float Cooldown_, float Range_ = 10000.0f, int Critical_Chance_ = 0)
        : Name(name_), Damage(Damage_), Cooldown(Cooldown_), Range(Range_), Critical_Chance(Critical_Chance_) {}
    friend std::ostream & operator<<(std::ostream & out, const Tool & object) {
        out<<object.Name<<" "<<object.Damage<<" "<<object.Range<<" "<<object.Cooldown<<" "<<object.Critical_Chance<<"\n";
        for (const auto & i : object.Attacks) {
            out<<i;
        }
        return out;
    }

    [[nodiscard]] float DamageCalculation() const {
        srand(time(nullptr));
        int Chance = rand() % 100;
        if (Chance < Critical_Chance) {
            return (float)Damage + 0.5 * Damage;
        }
        return Damage;
    }

    float Attack(const sf::RectangleShape &Sprite, sf::Angle Degrees) {
        float angleDegrees = (float)Degrees.asDegrees();
        float angleRadians = angleDegrees * (3.14 / 180.0);

        sf::Vector2f Offset{
            Range * std::cos(angleRadians),
            Range * std::sin(angleRadians)
        };

        Attack_Hitbox Entity{
            DamageCalculation(),
            {30, 100},
            Sprite.getPosition() + Offset,
            Degrees
        };

        Attacks.push_back(Entity);

        return Cooldown;
    }

    const std::string& GetName() {
        return Name;
    }

    [[nodiscard]] const std::vector <Attack_Hitbox>& GetAttackHitboxes() const {
        return Attacks;
    }

    void ClearAttacks() {
        Attacks.clear();
    }

    void ShowHitboxes(sf::RenderWindow& window) {
        for (auto & i : Attacks) {
            i.ShowSprite(window);
        }
    }
};

class Enemy {
    std::string Name;
    Character_Stats Stats{10, 5, 5 };
    Tool Weapon{"Sword", 3, 0.6f, 10};
    bool Damaged = false; //Daca a si-a primit damage de la atacul player-ului cu sabia sau nu
    int Experience = 5;

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
    Enemy& operator=(const Enemy& other) {
        Name = other.Name;
        Stats = other.Stats;
        Weapon = other.Weapon;
        Sprite = other.Sprite;
        Experience = other.Experience;
        return *this;
    }

    sf::FloatRect GetEnemyHitbox() {
        return Sprite.getGlobalBounds();
    }
    void ChangeDamagedStatus() {
        Damaged = !Damaged;
    }
    bool GetDamagedStatus() const {
        return Damaged;
    }
    float GetDamage() {
        return Weapon.DamageCalculation();
    }
    int GetExperience() const {
        return Experience;
    }
    bool TakeDamage (float Damage_Points) {
        return Stats.ReduceHealth(Damage_Points);
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
    void ShowSprite(sf::RenderWindow& window) {
        window.draw(Sprite);
    }


    void HandleMovement(const sf::Vector2f& PlayerPosition, float deltaTime, float deltaTimeMultiplier) {
        sf::Vector2f direction = PlayerPosition - Sprite.getPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (distance > 25.0f) {

            sf::Vector2f unitDirection = direction / distance;
            float speed = Stats.GetSpeed();
            // if (Damaged) {
            //     speed /= 2.0f;
            // }
            sf::Vector2f movement = unitDirection * speed * deltaTime * deltaTimeMultiplier;

            Sprite.move(movement);
            // float radians = std::atan2(direction.y, direction.x);
            // float angleDegrees = radians * 180.f / 3.14f;
            // Sprite.setRotation(sf::degrees(angleDegrees));
        }

    }
};

class Player_Class {
    int Experience, Gauge = 0;
    bool Invincibility = false, inAttack = false;
    Character_Stats Stats{30, 2, 0};
    Tool Pole{"Pole", 5, 0.4f, 40, 2};

    sf::RectangleShape Sprite;
    sf::Vector2f Position = {100.f, 100.f};
    sf::Vector2f Size = {50.f, 50.f};
    sf::Angle Rotation;

    sf::Time InvincibilityTime = sf::seconds(1.0f);
    sf::Clock ClockInvincibilityTime;

private:
    bool MakeInvincibile() {
        if (Invincibility == 1) {
            if (ClockInvincibilityTime.getElapsedTime() >= InvincibilityTime) {
                Invincibility = false;
            }
            return true;
        }
        ClockInvincibilityTime.restart();
        Invincibility = true;
        return false;
    }

public:
    explicit Player_Class(int Experience_ = 0, float InvincibilityTime_ = 1.0f) : Experience(Experience_), InvincibilityTime(sf::seconds(InvincibilityTime_)) {

        Sprite.setSize(Size);
        Sprite.setOrigin({Size.x / 2, Size.y / 2});
        Sprite.setFillColor(sf::Color::Blue);
        Sprite.setPosition(Position);

        Gauge = 0;
        Invincibility = false;
    };

    friend std::ostream & operator<<(std::ostream & out, const Player_Class & object) {
        out<<object.Experience<<"\n"<<object.Gauge<<"\n"<<object.inAttack<<"\n"<<object.Invincibility<<object.Pole;
        return out;
    }

    void ShowSprite(sf::RenderWindow& window) {
        window.draw(Sprite);
        Pole.ShowHitboxes(window);
    }

    sf::RectangleShape& GetSprite() {
        return Sprite;
    }

    sf::Vector2f GetPosition() {
        return Sprite.getPosition();
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

    std::vector<Attack_Hitbox> GetHitboxes() {
        return Pole.GetAttackHitboxes();
    }

    void ClearAttackHitboxes() {
        Pole.ClearAttacks();
        inAttack = false;
    }

    float HandleAttack() {
        inAttack = false;
        Pole.ClearAttacks();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            inAttack = true;
            return Pole.Attack(Sprite, Rotation);
        }
        return 0.0f;
    }

    void HandleMovement(sf::RenderWindow &window, float deltaTime = 0.016, float deltaTimeMultiplier = 62.5) {
        if (inAttack)
            return;

        //Movement
        sf::Vector2f movement(0.f, 0.f);
        float speed = Stats.GetSpeed();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            movement.y -= speed * deltaTime * deltaTimeMultiplier;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            movement.y += speed * deltaTime * deltaTimeMultiplier;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            movement.x -= speed * deltaTime * deltaTimeMultiplier;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            movement.x += speed * deltaTime * deltaTimeMultiplier;

        Sprite.move(movement);

        sf::Vector2i MousePos = sf::Mouse::getPosition(window);
        sf::Vector2f MouseWorldPos = sf::Vector2f(MousePos.x, MousePos.y);
        sf::Vector2f PlayerPosition = Sprite.getPosition();
        sf::Vector2f diff = MouseWorldPos - PlayerPosition;

        float radians = std::atan2(diff.y, diff.x);
        float angleDegrees = radians * 180.f / 3.14f;
        //Sprite.setRotation(sf::degrees(angleDegrees));
        Rotation = sf::degrees(angleDegrees);
    }
};

//unused for now
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
    bool Status = true;
public:
    explicit GUI_TextLabel(sf::Text TextValue_,const std::string& Name_ = "TextLabel",  const std::string& FontPath = "data/fonts/arial.ttf", int TextSize = 10, sf::Color TextColor = sf::Color::Black)
    : TextValue(TextValue_), Name(Name_){
        if (!TextFont.openFromFile(FontPath)) {
            std::cout<<"Error: Font not found for Text Label, must give the path to font_name.ttf.";
        }
        TextValue.setFont(TextFont);
        TextValue.setCharacterSize(TextSize);
        TextValue.setFillColor(TextColor);
    }

    friend std::ostream & operator<<(std::ostream & out, const GUI_TextLabel & object) {
        out<<object.Name<<"\nStatus: "<<object.Status<<"\n";
        return out;
    }

    void SetText(std::string TextValue_) {
        TextValue.setString(TextValue_);
    }

    void SetPosition(sf::Vector2f Position_) {
        Position = Position_;
        TextValue.setPosition(Position_);
    }

    void SetColor(sf::Color TextColor) {
        TextValue.setFillColor(TextColor);
    }

    void SetSize(int Size_) {
        TextValue.setCharacterSize(Size_);
    }

    void SetName(const std::string & Name_) {
        Name = Name_;
    }

    const std::string& GetName() const {
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
    float dt = 0.f;
    float dtMultiplier = 62.5f;
    Player_Class &player;

    sf::Clock GameClock, ActionClock;
    float ActionCooldown = 0;

    std::vector<Enemy> EnemyList, SpawnedEnemies;
    std::vector<Tool> ToolList;
    std::vector<Attack_Hitbox> PlayerAttackHitbox;
    std::vector<GUI_TextLabel> TextLabelList;

    bool PlayerLost = false;

private:
    void RenderEntities() {
        //Render Player
        if (!PlayerLost)
            player.ShowSprite(window);
        //Render Enemies
        for (auto &i : SpawnedEnemies) {
            i.ShowSprite(window);
        }
        //Render Hitboxes
        for (auto &i : PlayerAttackHitbox) {
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
            Tool WeaponAux{WeaponName, Damage, Cooldown, 20.0f, Critical_Chance};
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
                if (Weapon.GetName() == WeaponName) {
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
            sf::FloatRect EnemyBounds = i.GetEnemyHitbox();

            if (EnemyBounds.findIntersection(PlayerBounds)) {
                player.TakeDamage(i.GetDamage());
            }
        }

        //Player Events Functions and Actions
        sf::Time ActionDurationTime = ActionClock.getElapsedTime();
        if (!ActionCooldown) {
            for (auto &i : SpawnedEnemies) {
                if (i.GetDamagedStatus()) {
                    i.ChangeDamagedStatus();
                }
            }
            ActionCooldown = player.HandleAttack(); //Returneaza cat dureaza attackul
            if (ActionCooldown > 0.0f) {
                ActionClock.restart();
            }
        }
        else {
            if (ActionDurationTime < sf::seconds(ActionCooldown)) {
                //Verify if enemies are touching the player hitbox
                for (auto it = SpawnedEnemies.begin(); it != SpawnedEnemies.end();) {
                    auto& i = *it;
                    bool EnemyWasKilled = false;
                    for (auto &j : PlayerAttackHitbox) {
                        if (i.GetEnemyHitbox().findIntersection(j.GetBounds())) {
                            if (!i.GetDamagedStatus()) {
                                std::cout<<"Damaged!\n";
                                bool isDead = i.TakeDamage(j.GetDamageValue());
                                i.ChangeDamagedStatus();

                                if (!isDead) {
                                    player.AddExperience(i.GetExperience());
                                    it = SpawnedEnemies.erase(it);
                                    EnemyWasKilled = true;
                                    std::cout<<"Enemy has no health left!\n";
                                    break;
                                }
                            }
                        }
                    }
                    if (!EnemyWasKilled)
                        ++it;
                }
            }
            else {
                if (ActionDurationTime > sf::seconds(ActionCooldown) + sf::seconds(0.1f))
                    ActionCooldown = 0;
                player.ClearAttackHitboxes();
            }
        }
        PlayerAttackHitbox = player.GetHitboxes();
        player.HandleMovement(window, dt, dtMultiplier);
        for (auto &i : SpawnedEnemies) {
            i.HandleMovement(player.GetPosition(), dt, dtMultiplier);
        }


        //Update GUI
        UpdateHealthbar();

        //Win Conditions
        if (SpawnedEnemies.empty()) {
            for (auto &i : TextLabelList) {
                if (i.GetName() == "WinText" && i.GetStatus() == false) {
                    i.ToggleActive();
                }
            }
        }
        if (player.GetHealth() == 0) {
            for (auto &i : TextLabelList) {
                if (i.GetName() == "LoseText" && i.GetStatus() == false) {
                    i.ToggleActive();
                    PlayerLost = true;
                    player.ClearAttackHitboxes();
                }
            }
        }
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

            dt = GameClock.restart().asSeconds();
            dtMultiplier = 1/dt;

            if (!PlayerLost)
                EventHandler();

            //Rendering
            window.clear(sf::Color::White);
            RenderEntities();
            window.display();
        }
    }

public:
    Game_Class(sf::RenderWindow& window_, Player_Class& player_) : window(window_), player(player_) {}

    friend std::ostream & operator<<(std::ostream & out, const Game_Class & object) {
        out<<object.dt<<"\n"<<object.dtMultiplier<<"\n";
        for (const auto &i : object.EnemyList) {
            out<<i<<"\n";
        }
        for (const auto &i : object.ToolList) {
            out<<i<<"\n";
        }
        return out;
    }

    void Setup() {
        ActionClock.start();
        ReadData();

        SpawnedEnemies.insert(SpawnedEnemies.begin(), {EnemyList[0], EnemyList[1]});
        SpawnedEnemies[0].SetPosition(500, 200);
        SpawnedEnemies[1].SetPosition(500, 300);

        sf::Font font("data/fonts/arial.ttf");
        sf::Text text(font);
        text.setString("Hello World!");
        GUI_TextLabel textLabel(text, "Health", "data/fonts/arial.ttf", 20);
        TextLabelList.push_back(textLabel);

        textLabel.SetText("You won!");
        textLabel.SetName("WinText");
        textLabel.SetColor(sf::Color::Yellow);
        textLabel.SetPosition({0, 200});
        textLabel.SetSize(50);
        textLabel.ToggleActive();
        TextLabelList.push_back(textLabel);

        textLabel.SetText("You lost!");
        textLabel.SetName("LoseText");
        textLabel.SetColor(sf::Color::Red);
        textLabel.SetPosition({0, 200});
        textLabel.SetSize(50);
        TextLabelList.push_back(textLabel);

        WindowRendering();
    }

};

int main() {

    sf::RenderWindow window;
    window.create(sf::VideoMode({800, 600}), "Wukong");

    Player_Class Player{1, 1.0f};
    Game_Class Game{window, Player};
    Game.Setup();

    return 0;
}
