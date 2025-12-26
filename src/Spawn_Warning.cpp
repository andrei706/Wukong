//
// Created by Andrei on 12/21/2025.
//

#include "Spawn_Warning.h"

Spawn_Warning::Spawn_Warning(const std::string &Name_, sf::Vector2f Position_, float Duration_, float Size_): EnemyName(Name_), Position(Position_), Timer(Duration_), maxTime(Duration_), Size(Size_) {

    shape.setRadius(Size_);
    shape.setOrigin({Size_, Size_});
    shape.setPosition(Position);
    shape.setFillColor(sf::Color(255, 255, 255, 100));
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::Red);
}

void Spawn_Warning::Update(float dt) {
    Timer -= dt;
    if (Timer <= 0.f) {
        Finished = true;
    }

    float progress = 1.0f - (Timer / maxTime);
    shape.setScale({0.5f + progress, 0.5f + progress});
    shape.setFillColor(sf::Color(255, 255, 255, 150 * progress + 50));
}

void Spawn_Warning::Render(sf::RenderWindow &window) const {
    window.draw(shape);
}

bool Spawn_Warning::IsReady() const { return Finished; }

std::string Spawn_Warning::GetEnemyName() const { return EnemyName; }

sf::Vector2f Spawn_Warning::GetPosition() const { return Position; }
