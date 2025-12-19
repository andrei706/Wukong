
#include "Game_Class.h"


void Game_Class::RenderEntities() const {
    //Render Player
    if (!PlayerLost)
        player.ShowSprite(window);
    //Render Enemies
    for (const auto &i : SpawnedEnemies) {
        i->ShowSprite(window);

    }
    for (const auto &i :SpawnedEnemies) {
        i->RenderHitboxes(window);
    }
    //Render Hitboxes
    for (const auto &i : PlayerAttackHitbox) {
        i->ShowSprite(window);
    }
    //Render GUI
    for (const auto &i : TextLabelList) {
        if (i.GetStatus())
            i.ShowSprite(window);
    }
    if (isPaused || PlayerLost)
        for (const auto &i : PauseButtonList) {
            if (i.GetName() == "Resume" && PlayerLost == true) {
                continue;
            }
            i.ShowSprite(window);
        }

}
void Game_Class::ReadData() {
    std::ifstream toolFile("data/ToolList.json");
    if (!toolFile.is_open()) {
        throw AssetMissingException("data/ToolList.json");
    }

    nlohmann::json toolData;
    try {
        toolFile >> toolData;
    } catch (const nlohmann::json::parse_error&) {
        throw InvalidDataException("ToolList.json JSON Syntax", 0);
    }
    toolFile.close();

    for (const auto& tool : toolData) {
        if (!tool.contains("Type")) throw JsonMissingAttributeException("data/ToolList.json", "Type");
        if (!tool.contains("WeaponName")) throw JsonMissingAttributeException("data/ToolList.json", "WeaponName");
        if (!tool.contains("Cooldown")) throw JsonMissingAttributeException("data/ToolList.json", "Cooldown");
        if (!tool.contains("Range")) throw JsonMissingAttributeException("data/ToolList.json", "Range");
        if (!tool.contains("Critical_Chance")) throw JsonMissingAttributeException("data/ToolList.json", "Critical_Chance");

        std::shared_ptr<Tool> toolPtr;
        std::string type = tool.at("Type").get<std::string>();

        if (type == "Punch") {
            if (!tool.contains("Damage")) throw JsonMissingAttributeException("data/ToolList.json", "Damage");
            if (!tool.contains("PunchRadius")) throw JsonMissingAttributeException("data/ToolList.json", "PunchRadius");

            toolPtr = std::make_shared<Tool_Punch>(
                tool.at("WeaponName").get<std::string>(),
                tool.at("Damage").get<float>(),
                tool.at("Cooldown").get<float>(),
                tool.at("Range").get<float>(),
                tool.at("Critical_Chance").get<int>(),
                tool.at("PunchRadius").get<float>()
            );
            ToolList.push_back(toolPtr);
        }
        else if (type == "Ranged") {
            if (!tool.contains("Damage")) throw JsonMissingAttributeException("data/ToolList.json", "Damage");
            if (!tool.contains("ProjectileSize")) throw JsonMissingAttributeException("data/ToolList.json", "ProjectileSize");
            if (!tool.contains("ProjectileSpeed")) throw JsonMissingAttributeException("data/ToolList.json", "ProjectileSpeed");
            if (!tool.contains("Lifetime")) throw JsonMissingAttributeException("data/ToolList.json", "Lifetime");

            toolPtr = std::make_shared<Tool_Ranged>(
                tool.at("WeaponName").get<std::string>(),
                tool.at("Damage").get<float>(),
                tool.at("Cooldown").get<float>(),
                tool.at("Range").get<float>(),
                tool.at("Critical_Chance").get<int>(),
                tool.at("ProjectileSize").get<float>(),
                tool.at("ProjectileSpeed").get<float>(),
                tool.at("Lifetime").get<float>()
            );
            ToolList.push_back(toolPtr);
        }
    }

    std::ifstream enemyFile("data/EnemyList.json");
    if (!enemyFile.is_open()) {
        throw AssetMissingException("data/EnemyList.json");
    }

    nlohmann::json enemyData;
    try {
        enemyFile >> enemyData;
    } catch (const nlohmann::json::parse_error&) {
        throw InvalidDataException("EnemyList.json JSON Syntax", 0);
    }
    enemyFile.close();

    for (const auto& enemy : enemyData) {
        if (!enemy.contains("Type")) throw JsonMissingAttributeException("data/EnemyList.json", "Type");
        if (!enemy.contains("MaxHealth")) throw JsonMissingAttributeException("data/EnemyList.json", "MaxHealth");
        if (!enemy.contains("Speed")) throw JsonMissingAttributeException("data/EnemyList.json", "Speed");
        if (!enemy.contains("Mana")) throw JsonMissingAttributeException("data/EnemyList.json", "Mana");
        if (!enemy.contains("Name")) throw JsonMissingAttributeException("data/EnemyList.json", "Name");

        std::shared_ptr<Enemy> EnemyAuxPtr;
        Character_Stats StatsAux{
            enemy.at("MaxHealth").get<float>(),
            enemy.at("Speed").get<float>(),
            enemy.at("Mana").get<int>()
        };

        std::string type = enemy.at("Type").get<std::string>();

        if (type == "Walker") {
            EnemyAuxPtr = std::make_shared<Enemy_Walker>(enemy.at("Name").get<std::string>());
            EnemyAuxPtr->AssignStats(StatsAux);
        }
        else if (type == "Ranged") {
            EnemyAuxPtr = std::make_shared<Enemy_Ranger>(enemy.at("Name").get<std::string>());
            EnemyAuxPtr->AssignStats(StatsAux);
        }
        else if (type == "Rotator") {
            if (!enemy.contains("RotationSpeed")) throw JsonMissingAttributeException("data/EnemyList.json", "RotationSpeed");
            if (!enemy.contains("IsRotatingRight")) throw JsonMissingAttributeException("data/EnemyList.json", "IsRotatingRight");

            EnemyAuxPtr = std::make_shared<Enemy_Rotator>(
                enemy.at("Name").get<std::string>(),
                enemy.at("RotationSpeed").get<float>(),
                enemy.at("IsRotatingRight").get<bool>());
            EnemyAuxPtr->AssignStats(StatsAux);
        }

        if (!enemy.contains("WeaponName")) throw JsonMissingAttributeException("data/EnemyList.json", "WeaponName");
        std::string WeaponName = enemy.at("WeaponName").get<std::string>();

        bool weaponFound = false;
        for (auto &Weapon : ToolList) {
            if (Weapon->GetName() == WeaponName) {
                EnemyAuxPtr->AssignWeapon(Weapon);
                weaponFound = true;
                break;
            }
        }

        if (!weaponFound) {
            throw InvalidDataException("Weapon Not Found in ToolList", 0);
        }

        EnemyList.push_back(EnemyAuxPtr);
    }
}

void Game_Class::UpdateHealthbar() {
    for (auto &i : TextLabelList) {
        if (i.GetName() == "Health") {
            i.SetText("Health: " + std::to_string(player.GetHealth()));
        }
    }
}

void Game_Class::Replay() {
    SpawnedEnemies.clear();
    SpawnedEnemies.push_back(EnemyList[0]->clone());
    SpawnedEnemies.push_back(EnemyList[1]->clone());
    SpawnedEnemies[0]->SetPosition(500, 200);
    SpawnedEnemies[1]->SetPosition(500, 300);
    PlayerLost = false;
    player.RestoreHealth(999999999.9f);
    player.SetPosition({100, 100});
    for (auto &i : TextLabelList) {
        if (i.GetName() == "WinText" && i.GetStatus()) {
            i.ToggleActive();
        }
        if (i.GetName() == "LoseText" && i.GetStatus()) {
            i.ToggleActive();
        }
    }
    isPaused = false;
}

void Game_Class::PauseHandler() {
    sf::Vector2i MousePos = sf::Mouse::getPosition(window);
    sf::Vector2f MouseWorldPos = window.mapPixelToCoords(MousePos);

    for (auto &i : PauseButtonList) {
        if (i.isClicked(MouseWorldPos, KeyManager)) {
            if (i.GetName() == "Exit")
                window.close();
            else if (i.GetName() == "Resume")
                isPaused = false;
            else if (i.GetName() == "Replay")
                Replay();
        }
    }
}

void Game_Class::AdjustView(unsigned int newWidth, unsigned int newHeight) {
    float targetWidth = 1280.0f;
    float targetHeight = 720.0f;

    float windowRatio = (float)newWidth / (float)newHeight;
    float viewRatio = targetWidth / targetHeight;

    float scaleX = 1.0f;
    float scaleY = 1.0f;
    float viewportX = 0.0f;
    float viewportY = 0.0f;

    if (windowRatio > viewRatio) {
        scaleX = viewRatio / windowRatio;
        viewportX = (1.0f - scaleX) / 2.0f;
    } else if (windowRatio < viewRatio) {
        scaleY = windowRatio / viewRatio;
        viewportY = (1.0f - scaleY) / 2.0f;
    }

    sf::Vector2f position(viewportX, viewportY);

    sf::Vector2f size(scaleX, scaleY);
    sf::FloatRect viewportRect(position, size);

    view.setViewport(viewportRect);

    window.setView(view);

}

void Game_Class::EventHandler() {

    //Check if the player is getting damaged
    sf::FloatRect PlayerBounds = player.GetSprite().getGlobalBounds();
    for (auto &i : SpawnedEnemies) {
        std::vector<std::shared_ptr<Attack_Hitbox>> EnemyAttackHitbox = i->GetHitboxes();

        for (auto &j : EnemyAttackHitbox) {
            if (j->GetBounds().findIntersection(PlayerBounds)) {
                //std::cout<<"Intersection"<<std::endl;
                player.TakeDamage(j->GetDamageValue(-1));
            }
        }
    }

    //Check if enemies get damaged
    PlayerAttackHitbox = player.GetHitboxes();
    for (auto it = SpawnedEnemies.begin(); it != SpawnedEnemies.end();) {
        auto& i = *it;
        bool EnemyWasKilled = false;
        for (auto &j : PlayerAttackHitbox) {
            if (i->GetEnemyHitbox().findIntersection(j->GetBounds())) {
                if (!i->GetDamagedStatus()) {
                    float DamageCount = j->GetDamageValue(i->GetLocalId());
                    if (DamageCount) {
                        std::cout<<DamageCount<<"Damaged: "<<DamageCount<<std::endl;
                        bool isDead = i->TakeDamage(DamageCount);
                        i->ChangeDamagedStatus(true, 0.5f);

                        if (!isDead) {
                            player.AddExperience(i->GetExperience());
                            it = SpawnedEnemies.erase(it);
                            EnemyWasKilled = true;
                            std::cout<<"Enemy has no health left!\n";
                            break;
                        }
                    }
                }
            }
        }
        if (!EnemyWasKilled)
            ++it;
    }

    //Update the goated player
    player.Update(window, dt, dtMultiplier, KeyManager);

    //Update enemies behavior
    for (auto &i : SpawnedEnemies) {
        i->Update(player.GetPosition(), dt, dtMultiplier);
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
                //player.ClearAttackHitboxes();
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
            // else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            // {
            //     if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            //         if (!KeyManager.CheckButton("Escape")) {
            //             KeyManager.ToggleActivation("Escape");
            //             isPaused = !isPaused;
            //         }
            //     }
            //     else
            //         KeyManager.ToggleActivation("Escape");
            // }
            else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                AdjustView(resized->size.x, resized->size.y);
            }

        }

        dt = GameClock.restart().asSeconds();
        dtMultiplier = 1/dt;

        if (KeyManager.CheckInput("Escape")) {
            isPaused = !isPaused;
        }
        if (isPaused || PlayerLost)
            PauseHandler();

        if (!PlayerLost && !isPaused)
            EventHandler();

        //Rendering
        window.clear(sf::Color::White);
        RenderEntities();
        window.display();
    }
}

Game_Class::Game_Class(sf::RenderWindow &window_, Player_Class &player_): window(window_), player(player_) {
    view.setSize({1280.0f, 720.0f});
    view.setCenter({640.0f, 360.0f});

    //Enemy::id = 0;

    window.setView(view);
}

void Game_Class::Setup() {
    ReadData();

    SpawnedEnemies.push_back(EnemyList[0]->clone());
    SpawnedEnemies.push_back(EnemyList[2]->clone());
    SpawnedEnemies.push_back(EnemyList[3]->clone());
    SpawnedEnemies[0]->SetPosition(500, 200);
    SpawnedEnemies[1]->SetPosition(500, 300);
    SpawnedEnemies[2]->SetPosition(500, 400);
    Enemy::AssignID(SpawnedEnemies[0]);
    Enemy::AssignID(SpawnedEnemies[1]);
    Enemy::AssignID(SpawnedEnemies[2]);

    std::cout<< *SpawnedEnemies[0] << std::endl << *SpawnedEnemies[1];
    sf::Font font;
    try {
        if (!font.openFromFile("data/fonts/Tiny5-Regular.ttf")) {
            throw AssetMissingException("data/fonts/Tiny5-Regular.ttf");
        }
    }
    catch (const sf::Exception&) {
        std::cout<<"\n";
        throw AssetMissingException("data/fonts/Tiny5-Regular.ttf");
    }

    sf::Text text(font);
    text.setString("Hello World!");
    GUI_TextLabel textLabel(text, "Health", "data/fonts/Tiny5-Regular.ttf", 20);
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

    GUI_Button Button1("Resume", "data/textures/buttons/Resume_Button.png", {550, 500});
    PauseButtonList.push_back(Button1);
    GUI_Button Button2("Replay", "data/textures/buttons/Replay_Button.png", {300, 500});
    PauseButtonList.push_back(Button2);
    GUI_Button Button3("Exit", "data/textures/buttons/Exit_Button.png", {25, 500});
    PauseButtonList.push_back(Button3);

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
