#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "src/Player.h"
#include "src/Game_Manager.h"

int main() {

    sf::RenderWindow window;
    window.create(sf::VideoMode({1600, 900}), "Wukong");

    Player Player{25, 1.0f};
    Game_Manager Game{window, Player};
    Game.Run();

    return 0;
}
