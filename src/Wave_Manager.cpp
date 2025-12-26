

#include "Wave_Manager.h"

Wave::Wave(int Number_, int MaxEnemiesOnDisplay_, std::vector<std::pair<std::string, int>> Enemies_)
    : Index(Number_), MaxEnemiesOnDisplay(MaxEnemiesOnDisplay_), Enemies(std::move(Enemies_)) {}

int Wave::GetEnemiesOnDisplay() const { return MaxEnemiesOnDisplay; }

int Wave::GetIndex() const { return Index; }

std::string Wave::GetNextEnemy() {
    //srand(time(NULL));

    if (Enemies.empty()) return "";
    int EnemyIndex = rand() % Enemies.size();
    auto& selected = Enemies[EnemyIndex];

    std::string name = selected.first;
    selected.second--;
    if (selected.second <= 0) {
        Enemies.erase(Enemies.begin() + EnemyIndex);
    }
    return name;
}

bool Wave::IsFinished() const { return Enemies.empty(); }

Wave_Manager::Wave_Manager(int StartWave): CurrentWave(StartWave) {

}

void Wave_Manager::LoadWaves(const std::string &path) {
    allWaves.clear();

    std::ifstream file(path);
    if (!file.is_open()) throw AssetMissingException(path);

    nlohmann::json data;
    try {
        file >> data;
    } catch (const nlohmann::json::parse_error&) {
        throw InvalidDataException("WaveList.json Syntax Error", 0);
    }

    for (const auto& wJson : data) { //wJson = Wave Json
        if (!wJson.contains("Wave")) throw JsonMissingAttributeException(path, "Wave");
        if (!wJson.contains("MaxEnemiesOnDisplay")) throw JsonMissingAttributeException(path, "MaxEnemiesOnDisplay");
        if (!wJson.contains("Enemies")) throw JsonMissingAttributeException(path, "Enemies");

        std::vector<std::pair<std::string, int>> counts;

        auto& enemiesJson = wJson.at("Enemies");

        for (auto it = enemiesJson.begin(); it != enemiesJson.end(); ++it) {
            std::string name = it.key();
            int count = it.value().get<int>();
            counts.push_back({ name, count });
        }

        allWaves.emplace_back(
            wJson.at("Wave").get<int>(),
            wJson.at("MaxEnemiesOnDisplay").get<int>(),
            counts
        );
    }
}

Wave* Wave_Manager::GetCurrentWave() {
    for (auto &i : allWaves) {
        if (i.GetIndex() == CurrentWave) {
            return &i;
        }
    }
    return nullptr;
}

void Wave_Manager::Reset(const std::string& path) {
    CurrentWave = 1;
    LoadWaves(path);
}

void Wave_Manager::MoveToNextWave() { CurrentWave++; }

std::ostream & operator<<(std::ostream &os, const Wave &obj) {
    os<< "Index: " << obj.Index << std::endl
            << " MaxEnemiesOnDisplay: " << obj.MaxEnemiesOnDisplay << std::endl;
    for (auto &i : obj.Enemies) {
        os << i.first << ": " << i.second<< std::endl;
    }
    os<<std::endl;
    return os;
}

std::ostream & operator<<(std::ostream &os, const Wave_Manager &obj) {
    os << " CurrentWave: " << obj.CurrentWave << "\nWaves: "<< std::endl;
    for (auto &i : obj.allWaves) {
        os << i << std::endl;
    }
    return os;
}
