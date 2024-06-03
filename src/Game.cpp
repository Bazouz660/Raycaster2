#include "Game.hpp"
#include "ItemEntity.hpp"
#include "EnemyEntity.hpp"
#include "imgui-SFML.h"
#include <imgui.h>
#include "YPI/data.hpp"

bool my_tool_active = true;
float my_color[4] = { 0.4f, 0.7f, 0.0f, 0.5f };

Game &Game::getInstance()
{
    static Game game;
    return game;
}

Game::Game() :
      player(0.5, 0.5, -1.0, 0.0, 0.0, 0.66),
      map()
{
    window.create(sf::VideoMode(screenWidth, screenHeight), "Raycasting");
    lockMouse();

    ImGui::SFML::Init(window);
    // set ImGui style
    ImGui::StyleColorsDark();
    // load font
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("asset/arial.ttf", 16.0f);
    ImGui::SFML::UpdateFontTexture();

    ypi::ResourceManager::loadFromFolder(ypi::ResourceType::SoundBuffer, "asset/sound");
    ypi::ResourceManager::waitForLoading();

    auto sound = ypi::SoundManager::playSound("zombie_mono");
    sound->setLoop(true);
    sound->setRelativeToListener(true);
    sound->setMinDistance(3.f);
    sound->setAttenuation(0.8f);
    sound->setPosition(0.5, 0.5, 0);

    raycastViewport.create(resolutionX, resolutionY);

    raycaster.setSize(resolutionX, resolutionY);

    sf::Texture* entityTexture = new sf::Texture();
    entityTexture->loadFromFile("asset/entity.png");

    EnemyEntity entity1(0.5, 0.5, *entityTexture, player, map);
    addEntity(std::make_shared<EnemyEntity>(entity1));
}

void Game::lockMouse()
{
    window.setMouseCursorVisible(false);
    mouseLocked = true;
}

void Game::unlockMouse()
{
    window.setMouseCursorVisible(true);
    mouseLocked = false;
}

void Game::profileFrameTime()
{
    static float lastFrameTime = 0.0f;
    float currentFrameTime = clock.getElapsedTime();
    dt = currentFrameTime - lastFrameTime;

    fps = int(1.0f / dt);
    maxFps = std::max(maxFps, fps);

    frameTimes[frameCounter] = dt;

    if (frameCounter == 99) {
        float sum = 0.0f;
        minFps = 5000000;
        for (int i = 0; i < 100; i++) {
            sum += frameTimes[i];
            minFps = std::min(minFps, int(1.0f / frameTimes[i]));
        }
        avgFps = int(1.0f / (sum / 100.0f));
    }

    lastFrameTime = currentFrameTime;
    frameCounter = (frameCounter + 1) % 100;
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
    profileFrameTime();
    ImGui::SFML::Update(window, sf::seconds(dt));
    ypi::SoundManager::update();

    for (auto &entity : entities) {
        entity->update(dt);
    }
    player.handleInput(map, window, dt);

    if (mouseLocked) // Reset mouse position to center
        sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);

    updateGUI();
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
    window.display();
}

void Game::updateGUI()
{
    // ImGui window to display current fps, average fps, max fps, min fps, frame time, and a frame time graph
    ImGui::Begin("Stats", &my_tool_active, ImGuiWindowFlags_NoResize);
    ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::Text("Current FPS: %d", fps);
    ImGui::Text("Average FPS: %d", avgFps);
    ImGui::Text("Max FPS: %d", maxFps);
    ImGui::Text("Min FPS: %d", minFps);
    ImGui::Text("Frame Time: %f", dt);
    ImGui::PlotLines("Frame Times", frameTimes, IM_ARRAYSIZE(frameTimes), 0, NULL, 0.0f, 0.02f, ImVec2(0, 80));
    ImGui::End();
}

void Game::addEntity(const std::shared_ptr<AEntity> &entity)
{
    entities.push_back(entity);
}

bool Game::isMouseLocked()
{
    return getInstance().mouseLocked;
}