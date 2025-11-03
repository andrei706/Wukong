#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "src/Player_Class.h"
#include "src/Game_Class.h"



int main() {

    sf::RenderWindow window;
    window.create(sf::VideoMode({800, 600}), "Wukong");

    Player_Class Player{1, 1.0f};
    Game_Class Game{window, Player};
    Game.Setup();

    return 0;
}
