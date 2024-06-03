#include <SFML/Graphics.hpp>
#include "Game.hpp"

int main()
{
    Game &game = Game::getInstance();
    game.run();
    return 0;
}
