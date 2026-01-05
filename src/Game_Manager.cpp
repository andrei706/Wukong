
#include "Game_Manager.h"


void Game_Manager::RenderEntities() const {
    //map rendering
    RenderCollection(window, MapBlocks);
    //Difficulty screen rendering
    if (isSelectingDifficulty) {
        window.draw(PauseTint);
        for (const auto &button : DifficultyButtons) {
            button.ShowSprite(window);
        }
        for (const auto &label : TextLabelList) {
            if (label.GetName() == "DiffTitle" || label.GetName() == "EasyDesc") {
                label.ShowSprite(window);
            }
        }
        return;
    }
    //Render Player
    if (!PlayerLost)
        player.ShowSprite(window);
    //Render Enemy Spawn Warnings
    for (const auto& warn : ActiveSpawnWarnings) {
        warn.Render(window);
    }
    //Render Enemies
    RenderCollection(window, SpawnedEnemies);
    for (const auto &i :SpawnedEnemies) {
        i->RenderHitboxes(window);
    }
    //Render Hitboxes
    for (const auto &i : PlayerAttackHitbox) {
        i->ShowSprite(window);
    }
    //Render GUI
    for (const auto& pair : DamageCounter) {
        pair.first.ShowSprite(window);
    }
    if (isPaused || PlayerLost) {
        window.draw(PauseTint);

        for (const auto &i : PauseButtonList) {
            if (i.GetName() == "Resume" && PlayerLost == true) {
                continue;
            }
            i.ShowSprite(window);
        }
        if (isPaused && !PlayerLost) {
            UpgradeMenu.Render(window);
        }
    }
    for (const auto &i : TextLabelList) {
        if (i.GetStatus() && !(i.GetName() == "DiffTitle" || i.GetName() == "EasyDesc"))
            i.ShowSprite(window);
    }
}
void Game_Manager::ReadData() {
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

        std::shared_ptr<Tool> toolPtr = Tool_Factory::createWeapon(tool);

        if (toolPtr) {
            ToolList.push_back(toolPtr);
        } else {
            std::cerr << "Warning: Unknown tool type in JSON: " << tool.at("Type") << std::endl;
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

    Enemy_Builder eb;
    for (const auto& enemyJson : enemyData) {
        if (!enemyJson.contains("Type")) throw JsonMissingAttributeException("data/EnemyList.json", "Type");
        if (!enemyJson.contains("MaxHealth")) throw JsonMissingAttributeException("data/EnemyList.json", "MaxHealth");
        if (!enemyJson.contains("Speed")) throw JsonMissingAttributeException("data/EnemyList.json", "Speed");
        if (!enemyJson.contains("Mana")) throw JsonMissingAttributeException("data/EnemyList.json", "Mana");
        if (!enemyJson.contains("Name")) throw JsonMissingAttributeException("data/EnemyList.json", "Name");

        std::shared_ptr<Tool> meleePtr = nullptr;
        if (enemyJson.contains("MeleeWeaponName")) {
            std::string weaponName = enemyJson.at("MeleeWeaponName").get<std::string>();
            for (auto& t : ToolList) {
                if (t->GetName() == weaponName) { meleePtr = t; break; }
            }
        }

        std::shared_ptr<Tool> rangedPtr = nullptr;
        if (enemyJson.contains("RangedWeaponName")) {
            std::string weaponName = enemyJson.at("RangedWeaponName").get<std::string>();
            for (auto& t : ToolList) {
                if (t->GetName() == weaponName) { rangedPtr = t; break; }
            }
        }

        std::shared_ptr<Enemy> Enemy = eb.startNew(enemyJson)
                           .SetStats(enemyJson)
                           .SetMelee(meleePtr)
                           .SetRanged(rangedPtr)
                           .Build();

        if (Enemy)
            EnemyList.push_back(Enemy);
    }
}

void Game_Manager::UpdateHealthbar() {
    float currentHealth = player.GetHealth();
    int currentGauge = player.GetGauge();

    if (currentHealth < lastHealth) {
        healthFlashTimer = 0.4f;
        healthFlashColor = sf::Color(139, 0, 0);
    }
    else if (currentHealth > lastHealth) {
        healthFlashTimer = 0.4f;
        healthFlashColor = sf::Color::Green;
    }
    if (currentGauge < lastGauge) {
        gaugeFlashTimer = 0.4f;
        gaugeFlashColor = sf::Color(0, 0, 220 - lastGauge + currentGauge);
    }
    else if (currentGauge > lastGauge) {
        gaugeFlashTimer = 0.4f;
        gaugeFlashColor = sf::Color(28, 118, 255);
    }
    lastGauge = currentGauge;
    lastHealth = currentHealth;

    if (healthFlashTimer > 0.f) healthFlashTimer -= dt;
    if (gaugeFlashTimer > 0.f) gaugeFlashTimer -= dt;

    for (auto &i : TextLabelList) {
        if (i.GetName() == "Health") {
            i.SetText("Health: " + std::to_string(currentHealth));

            if (healthFlashTimer > 0.f) {
                i.SetColor(healthFlashColor);
            } else {
                i.SetColor(sf::Color::Red);
            }
        }
        if (i.GetName() == "GaugeLabel") {
            i.SetText("Gauge: " + std::to_string(player.GetGauge()));
            if (gaugeFlashTimer > 0.f) {
                i.SetColor(gaugeFlashColor);
            } else {
                i.SetColor(sf::Color::Blue);
            }
        }
    }
}

void Game_Manager::Replay() {
    SpawnedEnemies.clear();
    ActiveSpawnWarnings.clear();
    PlayerAttackHitbox.clear();

    isSelectingDifficulty = true;
    ToggleRotatorRotation();
    WaveManager.Reset("data/WaveList.json");

    PlayerLost = false;
    isPaused = false;
    player.Restart();
    UpgradeMenu.Reset();
    lastHealth = player.GetHealth();

    totalRunTime = 0.f;
    gameBeaten = false;

    for (auto &i : TextLabelList) {
        if (i.GetName() == "WinText") {
            i.SetText("You won!");
        }
    }

    for (auto &i : TextLabelList) {
        if ((i.GetName() == "WinText" || i.GetName() == "LoseText") && i.GetStatus()) {
            i.ToggleActive();
        }
        if (i.GetName() == "WaveAnnouncer" && !i.GetStatus()) {
            i.ToggleActive();
        }
    }

    GameClock.restart();
}

std::string Game_Manager::CalculateRank() const {
    if (totalRunTime < 120.f) return "S";
    if (totalRunTime < 240.f) return "A";
    if (totalRunTime < 360.f) return "B";
    return "C";
}

void Game_Manager::HandleDifficultySelection() {
    sf::Vector2i MousePos = sf::Mouse::getPosition(window);
    sf::Vector2f MouseWorldPos = window.mapPixelToCoords(MousePos);

    for (auto &button : DifficultyButtons) {
        if (button.isClicked(MouseWorldPos, KeyManager)) {
            if (button.GetName() == "Easy") difficultyMultiplier = 2.0f;
            else if (button.GetName() == "Hard") difficultyMultiplier = 1.0f;

            for (auto &label : TextLabelList) {
                if (label.GetName() == "DiffTitle") label.ToggleActive();
            }

            isSelectingDifficulty = false;
            GameClock.restart();
        }
    }
}

void Game_Manager::PauseHandler() {
    sf::Vector2i MousePos = sf::Mouse::getPosition(window);
    sf::Vector2f MouseWorldPos = window.mapPixelToCoords(MousePos);

    for (auto &i : PauseButtonList) {
        if (i.isClicked(MouseWorldPos, KeyManager)) {
            if (i.GetName() == "Exit")
                window.close();
            else if (i.GetName() == "Resume") {
                isPaused = false;
                player.PauseClocks(false);
                for (auto &j : SpawnedEnemies)
                    j->PauseClocks(false);
            }
            else if (i.GetName() == "Replay")
                Replay();
        }
    }

    if (isPaused && !PlayerLost) {
        UpgradeMenu.UpdateLabels(player);
        UpgradeMenu.HandleInput(MouseWorldPos, KeyManager, player);
    }
    UpdateHealthbar();
}

void Game_Manager::SpawnEnemy(const std::string &Name, sf::Vector2f Position) {
    for (auto const &EnemyPtr : EnemyList) {
        if (EnemyPtr->GetName() == Name) {
            std::shared_ptr<Enemy> newEnemy = EnemyPtr->clone();
            newEnemy->SetPosition(Position.x, Position.y);
            Enemy::AssignID(newEnemy);
            SpawnedEnemies.push_back(newEnemy);
            return;
        }
    }
    throw InvalidDataException("Enemy prototype not found: " + Name, 0);
}

void Game_Manager::AdjustView(unsigned int newWidth, unsigned int newHeight) {
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

void Game_Manager::EventHandler() {
    UpdateSpawnWarnings();

    //Check if the player is getting damaged
    sf::FloatRect PlayerBounds = player.GetSprite().getGlobalBounds();
    for (auto &i : SpawnedEnemies) {
        std::vector<std::shared_ptr<Attack>> EnemyAttackHitbox = i->GetHitboxes();

        for (auto &j : EnemyAttackHitbox) {
            if (j->GetBounds().findIntersection(PlayerBounds)) {
                //std::cout<<"Intersection"<<std::endl;
                float CurrentHealth = player.GetHealth();
                bool isDamaged = player.TakeDamage(j->GetDamageValue(-1));
                if (isDamaged) {
                    AddDamageCounter(CurrentHealth - player.GetHealth(), player.GetPosition(), sf::Color(139, 0, 0));
                }
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
                //if (!i->GetDamagedStatus()) {
                    float DamageCount = j->GetDamageValue(i->GetLocalId());
                    if (DamageCount) {
                        //std::cout<<DamageCount<<"Damaged: "<<DamageCount<<std::endl;
                        bool isDead = i->TakeDamage(DamageCount);
                        i->ChangeDamagedStatus(true, 0.5f);
                        AddDamageCounter(DamageCount, i->GetPosition(), sf::Color::Red);
                        if (!isDead) {
                            player.AddExperience(i->GetExperience() * difficultyMultiplier);
                            it = SpawnedEnemies.erase(it);
                            EnemyWasKilled = true;
                            break;
                        }
                    }
                //}
            }
        }
        if (!EnemyWasKilled)
            ++it;
    }

    //Update the player
    player.Update(window, dt, dtMultiplier, KeyManager);

    //Update enemies behavior
    for (auto &i : SpawnedEnemies) {
        i->Update(player.GetPosition(), dt, dtMultiplier, SpawnedEnemies);
    }

    //Update GUI
    UpdateDamageCounters();
    UpdateHealthbar();

    //Lose Conditions
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

void Game_Manager::WindowRendering() {
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(60);
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                AdjustView(resized->size.x, resized->size.y);
            }

        }

        dt = GameClock.restart().asSeconds();
        dtMultiplier = 1/dt;
        if (isSelectingDifficulty) {
            HandleDifficultySelection();
        }
        else {
            if (KeyManager.CheckInput("Escape")) {
                isPaused = !isPaused;
                if (isPaused) {
                    player.PauseClocks();
                    for (auto &i : SpawnedEnemies) {
                        i->PauseClocks();
                    }
                }
                else {
                    player.PauseClocks(false);
                    for (auto &i : SpawnedEnemies) {
                        i->PauseClocks(false);
                    }
                }
            }
            if (isPaused || PlayerLost)
                PauseHandler();

            if (!PlayerLost && !isPaused) {
                if (!gameBeaten) {
                    totalRunTime += dt;
                    WaveHandler();
                }
                EventHandler();
            }
        }
        //Rendering
        window.clear(sf::Color::Black);
        RenderEntities();
        window.display();
    }
}

void Game_Manager::UpdateSpawnWarnings() {
    for (auto it = ActiveSpawnWarnings.begin(); it != ActiveSpawnWarnings.end(); ) {
        it->Update(dt);

        if (it->IsReady()) {
            SpawnEnemy(it->GetEnemyName(), it->GetPosition());
            it = ActiveSpawnWarnings.erase(it);
        } else {
            ++it;
        }
    }
}

void Game_Manager::WaveHandler() {
    srand(time(NULL));
    Wave* currentWave = WaveManager.GetCurrentWave();
    int currentCount = static_cast<int>(SpawnedEnemies.size() + ActiveSpawnWarnings.size());

    if (currentWave == nullptr && currentCount == 0) {
        if (!gameBeaten) {
            gameBeaten = true;
            std::string finalRank = CalculateRank();

            for (auto &i : TextLabelList) {
                if (i.GetName() == "WinText") {
                    if (!i.GetStatus()) i.ToggleActive();

                    int mins = static_cast<int>(totalRunTime) / 60;
                    int secs = static_cast<int>(totalRunTime) % 60;

                    i.SetText("YOU WON! RANK: " + finalRank +
                              "\nTime: " + std::to_string(mins) + ":" + (secs < 10 ? "0" : "") + std::to_string(secs));
                }
            }
        }
        return;
    }

    for (auto &label : TextLabelList) {
        if (label.GetName() == "WaveAnnouncer") {
            label.SetText("WAVE " + std::to_string(currentWave->GetIndex()));
            if (!label.GetStatus()) label.ToggleActive();
        }
    }

    if (currentWave->IsFinished() && SpawnedEnemies.empty() && ActiveSpawnWarnings.empty()) {
        WaveManager.MoveToNextWave();
        return;
    }

    if (!currentWave->IsFinished() && currentCount < currentWave->GetEnemiesOnDisplay()) {
        std::string nextEnemyName = currentWave->GetNextEnemy();

        Random_Value_Generator<float> posXGen{100.0f, 1100.0f};
        Random_Value_Generator<float> posYGen{100.0f, 600.0f};

        if (!nextEnemyName.empty()) {
            sf::Vector2f spawnPos = {
                posXGen.generate(),
                posYGen.generate()
            };
            ActiveSpawnWarnings.emplace_back(nextEnemyName, spawnPos, 1.5f);
        }
    }
}

void Game_Manager::ToggleRotatorRotation() const {
    for (auto& enemyPtr : EnemyList) {
        // convertim pointerul de baza intr-un pointer de tip enemy rotator
        // enemylist are pointeri de tip enemy
        std::shared_ptr<Enemy_Rotator> Rotator = std::dynamic_pointer_cast<Enemy_Rotator>(enemyPtr);
        if (Rotator) {
            Rotator->SwapRotation();
        }
    }
}

void Game_Manager::AddDamageCounter(float damage, sf::Vector2f position, sf::Color color) {
    GUI_TextLabel dmgLabel("Dmg", "data/fonts/Tiny5-Regular.ttf", 25, color);

    dmgLabel.SetText("-" + std::to_string(static_cast<int>(damage)));
    dmgLabel.SetPosition({position.x, position.y});

    DamageCounter.emplace_back(dmgLabel, 1.0f);
}
void Game_Manager::UpdateDamageCounters() {
    for (auto it = DamageCounter.begin(); it != DamageCounter.end(); ) {
        it->second -= dt;

        sf::Vector2f pos = it->first.GetPosition();
        it->first.SetPosition({pos.x, pos.y - (60*dt)});

        if (it->second <= 0.f) {
            it = DamageCounter.erase(it);
        } else {
            ++it;
        }
    }
}

void Game_Manager::AddTextLabel(const std::string &name, const std::string &content, sf::Vector2f position, int size,
                              sf::Color color, bool isActive, bool OutlineEnabled) {

    GUI_TextLabel label(name, "data/fonts/Tiny5-Regular.ttf", size, color, OutlineEnabled);
    label.SetText(content);
    label.SetPosition(position);
    if (!isActive) label.ToggleActive();
    TextLabelList.push_back(label);
}

void Game_Manager::SetupMap() {
    const float blockSize = 40.f;
    const float screenWidth = 1280.f;
    const float screenHeight = 720.f;

    const float minX = blockSize;
    const float maxX = screenWidth - blockSize;
    const float minY = blockSize;
    const float maxY = screenHeight - blockSize;

    for (float x = 0; x < screenWidth; x += blockSize) {
        for (float y = 0; y < screenHeight; y += blockSize) {
            std::string path;

            if (x >= minX && x < maxX && y >= minY && y < maxY) {
                path = "data/textures/blocks/grass.png"; // Interior
            } else {
                path = "data/textures/blocks/dirt.png";  // Border
            }

            MapBlocks.push_back(std::make_shared<Enviroment_Object>(
                sf::Vector2f(x, y),
                sf::Vector2f(blockSize, blockSize),
                path
            ));
        }
    }
}

Game_Manager::Game_Manager(sf::RenderWindow &window_, Player &player_): window(window_), player(player_) {
    view.setSize({1280.0f, 720.0f});
    view.setCenter({640.0f, 360.0f});

    if (!GameFont.openFromFile("data/fonts/Tiny5-Regular.ttf")) {
        throw AssetMissingException("data/fonts/Tiny5-Regular.ttf");
    }

    PauseTint.setSize({1280.f, 720.f});
    PauseTint.setFillColor(sf::Color(0, 0, 0, 150));

    window.setView(view);
}

void Game_Manager::Run() {
    ReadData();
    WaveManager.LoadWaves("data/WaveList.json");
    std::cout<<WaveManager;

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

    AddTextLabel("Health", "Health: 100", {0, 0}, 20, sf::Color::Red, true, true);
    AddTextLabel("GaugeLabel", "Gauge: 0", {0, 30}, 20, sf::Color::Blue, true, true);
    AddTextLabel("WinText", "You won!", {0, 200}, 50, sf::Color::Yellow, false,true);
    AddTextLabel("LoseText", "You lost!", {0, 200}, 50, sf::Color::Red, false, true);
    AddTextLabel("WaveAnnouncer", "WAVE 1", {600, 0}, 30, sf::Color::Black);

    GUI_Button Button1("Resume", "data/textures/buttons/Resume_Button.png", {1050, 600});
    PauseButtonList.push_back(Button1);
    GUI_Button Button2("Replay", "data/textures/buttons/Replay_Button.png", {800, 600});
    PauseButtonList.push_back(Button2);
    GUI_Button Button3("Exit", "data/textures/buttons/Exit_Button.png", {50, 600});
    PauseButtonList.push_back(Button3);

    AddTextLabel("DiffTitle", "SELECT DIFFICULTY", {450, 200}, 40, sf::Color::White, true, true);
    GUI_Button Button4("Easy", "data/textures/buttons/Easy_Button.png", sf::Vector2f(200, 350));
    AddTextLabel("EasyDesc", "Experience gained is doubled", {185, 460}, 18, sf::Color::Yellow, true, true);
    DifficultyButtons.emplace_back(Button4);
    DifficultyButtons.emplace_back("Hard", "data/textures/buttons/Hard_Button.png", sf::Vector2f(800, 350));

    SetupMap();
    WindowRendering();
}

std::ostream & operator<<(std::ostream &out, const Game_Manager &object) {
    out<<object.dt<<"\n"<<object.dtMultiplier<<"\n";
    for (const auto &i : object.EnemyList) {
        out<<i<<"\n";
    }
    for (const auto &i : object.ToolList) {
        out<<i<<"\n";
    }
    return out;
}
