
#ifndef OOP_SPAWN_WARNING_H
#define OOP_SPAWN_WARNING_H
#include "SFML/Graphics.hpp"

class Spawn_Warning {
    std::string EnemyName;
    sf::Vector2f Position;
    float Timer;
    float maxTime;
    float Size;
    bool Finished = false;

    sf::CircleShape shape;

public:
    Spawn_Warning(const std::string& Name_, sf::Vector2f Position_, float Duration_, float Size_ = 25.f);

    void Update(float dt);

    void Render(sf::RenderWindow& window) const;

    [[nodiscard]] bool IsReady() const;

    [[nodiscard]] std::string GetEnemyName() const;

    [[nodiscard]] sf::Vector2f GetPosition() const;
};


#endif //OOP_SPAWN_WARNING_H