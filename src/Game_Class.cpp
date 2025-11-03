

#include "Game_Class.h"

void Game_Class::RenderEntities() const {
    //Render Player
    if (!PlayerLost)
        player.ShowSprite(window);
    //Render Enemies
    for (const auto &i : SpawnedEnemies) {
        i.ShowSprite(window);
    }
    //Render Hitboxes
    for (const auto &i : PlayerAttackHitbox) {
        i.ShowSprite(window);
    }
    //Render GUI
    for (const auto &i : TextLabelList) {
        if (i.GetStatus())
            i.ShowSprite(window);
    }
}

void Game_Class::ReadData() {
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

void Game_Class::UpdateHealthbar() {
    for (auto &i : TextLabelList) {
        if (i.GetName() == "Health") {
            i.SetText("Health: " + std::to_string(player.GetHealth()));
        }
    }
}

void Game_Class::EventHandler() {
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

void Game_Class::WindowRendering() {
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

Game_Class::Game_Class(sf::RenderWindow &window_, Player_Class &player_): window(window_), player(player_) {}

void Game_Class::Setup() {
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

std::ostream & operator<<(std::ostream &out, const Game_Class &object) {
    out<<object.dt<<"\n"<<object.dtMultiplier<<"\n";
    for (const auto &i : object.EnemyList) {
        out<<i<<"\n";
    }
    for (const auto &i : object.ToolList) {
        out<<i<<"\n";
    }
    return out;
}
