#include "Game.hpp"
#include "ItemEntity.hpp"
#include "EnemyEntity.hpp"

Game::Game() :
      player(20.0, 12.0, -1.0, 0.0, 0.0, 0.66),
      map()
{
    window.create(sf::VideoMode(screenWidth, screenHeight), "Raycasting");
    raycastViewport.create(resolutionX, resolutionY);
    lockMouse();

    font.loadFromFile("asset/arial.ttf");
    fpsCounter.setFont(font);
    fpsCounter.setCharacterSize(24);
    fpsCounter.setFillColor(sf::Color::White);
    fpsCounter.setPosition(10, 10);

    raycaster.setSize(resolutionX, resolutionY);

    sf::Texture* entityTexture = new sf::Texture();
    entityTexture->loadFromFile("asset/entity.png");

    EnemyEntity entity1(20.0, 12.0, *entityTexture, player, map);
    addEntity(std::make_shared<EnemyEntity>(entity1));
}

void Game::lockMouse()
{
    sf::Mouse::setPosition(sf::Vector2i(resolutionX / 2, resolutionY / 2), window);
    window.setMouseCursorVisible(false);
}

void Game::unlockMouse()
{
    window.setMouseCursorVisible(true);
}

void Game::computeDeltaTime()
{
    static float lastFrameTime = 0.0f;
    float currentFrameTime = clock.getElapsedTime();
    dt = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    fpsCounter.setString("FPS: " + std::to_string(int(1.0f / dt)));
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::LostFocus)
            unlockMouse();
        if (event.type == sf::Event::GainedFocus)
            lockMouse();
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update()
{
    computeDeltaTime();
    for (auto &entity : entities) {
        entity->update(dt);
    }
    player.handleInput(map, window, dt);
}

void Game::render()
{
    window.clear(sf::Color::Black);
    raycastViewport.clear(sf::Color::Black);
    raycaster.castRays(raycastViewport, player, entities, map);
    raycastViewport.display();
    sf::RectangleShape rectangle(sf::Vector2f(screenWidth, screenHeight));
    rectangle.setTexture(&raycastViewport.getTexture());
    window.draw(rectangle);
    window.draw(fpsCounter);
    window.display();
}

void Game::addEntity(const std::shared_ptr<AEntity> &entity)
{
    entities.push_back(entity);
}