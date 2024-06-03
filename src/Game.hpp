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
    void run();
    static Game &getInstance();

    static bool isMouseLocked();

private:
    Game();
    void processEvents();
    void update();
    void render();

    void updateGUI();

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
    const int resolutionX = 800;
    const int resolutionY = 600;
    const int screenWidth = 800;
    const int screenHeight = 600;
    bool mouseLocked = true;

    float dt = 0.0f;
    float frameTimes[100];
    int frameCounter = 0;

    int fps = 0;
    int maxFps = 0;
    int minFps = 0;
    int avgFps = 0;
};

#endif // GAME_HPP
