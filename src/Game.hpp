#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Map.hpp"
#include "Raycaster.hpp"
#include "Clock.hpp"
#include "AEntity.hpp"
#include <memory>

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();

    void lockMouse();
    void unlockMouse();

    void computeDeltaTime();

    void addEntity(const std::shared_ptr<AEntity> &entity);

    sf::RenderWindow window;
    sf::RenderTexture raycastViewport;
    Player player;
    Map map;
    Raycaster raycaster;
    Clock clock;
    std::vector<std::shared_ptr<AEntity>> entities; // List of entities
    sf::Text fpsCounter;
    sf::Font font;
    float dt = 0.0f;
    const int resolutionX = 800;
    const int resolutionY = 600;
    const int screenWidth = 800;
    const int screenHeight = 600;
    bool mouseLocked = true;
};

#endif // GAME_HPP
