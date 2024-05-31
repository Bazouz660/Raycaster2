#include "Game.hpp"
#include "ItemEntity.hpp"
#include "EnemyEntity.hpp"
#include "imgui-SFML.h"
#include <imgui.h>

Game::Game() :
      player(20.5, 13.0, -1.0, 0.0, 0.0, 0.66),
      map()
{
    window.create(sf::VideoMode(screenWidth, screenHeight), "Raycasting");
    lockMouse();

    ImGui::SFML::Init(window);
    // set ImGui style
    ImGui::StyleColorsDark();

    raycastViewport.create(resolutionX, resolutionY);

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
    mouseLocked = true;
}

void Game::unlockMouse()
{
    window.setMouseCursorVisible(true);
    mouseLocked = false;
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
        ImGui::SFML::ProcessEvent(window, event);
        if (event.type == sf::Event::LostFocus || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LControl))
            unlockMouse();
        if (event.type == sf::Event::GainedFocus || (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::LControl))
            lockMouse();
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update()
{
    computeDeltaTime();
    ImGui::SFML::Update(window, sf::seconds(dt));
    for (auto &entity : entities) {
        entity->update(dt);
    }
    player.handleInput(map, window, dt);

    if (mouseLocked) // Reset mouse position to center
        sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);

    ImGui::ShowDemoWindow();
    ImGui::Begin("Hello, world!");
    ImGui::Button("Look at this pretty button");
    ImGui::End();
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
    ImGui::SFML::Render(window);
    window.draw(fpsCounter);
    window.display();
}

void Game::addEntity(const std::shared_ptr<AEntity> &entity)
{
    entities.push_back(entity);
}