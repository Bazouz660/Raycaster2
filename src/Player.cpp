#include "Player.hpp"
#include <cmath>
#include "Game.hpp"

Player::Player(double posX, double posY, double dirX, double dirY, double planeX, double planeY)
    : posX(posX), posY(posY), dirX(dirX), dirY(dirY), planeX(planeX), planeY(planeY)
{
}

bool Player::isCollision(double x, double y, const Map &map)
{
    // Check four corners of the bounding box
    return !map.isEmpty(int(x + collisionRadius), int(y + collisionRadius)) ||
           !map.isEmpty(int(x - collisionRadius), int(y + collisionRadius)) ||
           !map.isEmpty(int(x + collisionRadius), int(y - collisionRadius)) ||
           !map.isEmpty(int(x - collisionRadius), int(y - collisionRadius));
}

void Player::handleInput(const Map &map, const sf::Window &window, float dt)
{
    double newPosX = posX;
    double newPosY = posY;

    float dtMoveSpeed = moveSpeed * dt;
    float oldPosX = posX;
    float oldPosY = posY;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        newPosX = posX + dirX * dtMoveSpeed;
        newPosY = posY + dirY * dtMoveSpeed;
        if (!isCollision(newPosX, posY, map)) posX = newPosX;
        if (!isCollision(posX, newPosY, map)) posY = newPosY;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        newPosX = posX - dirX * dtMoveSpeed;
        newPosY = posY - dirY * dtMoveSpeed;
        if (!isCollision(newPosX, posY, map)) posX = newPosX;
        if (!isCollision(posX, newPosY, map)) posY = newPosY;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        newPosX = posX - planeX * dtMoveSpeed;
        newPosY = posY - planeY * dtMoveSpeed;
        if (!isCollision(newPosX, posY, map)) posX = newPosX;
        if (!isCollision(posX, newPosY, map)) posY = newPosY;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        newPosX = posX + planeX * dtMoveSpeed;
        newPosY = posY + planeY * dtMoveSpeed;
        if (!isCollision(newPosX, posY, map)) posX = newPosX;
        if (!isCollision(posX, newPosY, map)) posY = newPosY;
    }

    // average new pos and old pos
    posX = (posX + oldPosX) / 2;
    posY = (posY + oldPosY) / 2;

    // Handle mouse movement
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    int mouseXDelta = mousePosition.x - window.getSize().x / 2;
    handleMouseMovement(mouseXDelta, window, dt);
}

void Player::handleMouseMovement(int mouseXDelta, const sf::Window &window, float dt)
{
    if (!window.hasFocus() || !Game::isMouseLocked())
        return;
    double actualRotSpeed = rotSpeed * -mouseXDelta * dt;

    // Rotate the direction vector
    double oldDirX = dirX;
    dirX = dirX * cos(actualRotSpeed) - dirY * sin(actualRotSpeed);
    dirY = oldDirX * sin(actualRotSpeed) + dirY * cos(actualRotSpeed);

    // Rotate the camera plane
    double oldPlaneX = planeX;
    planeX = planeX * cos(actualRotSpeed) - planeY * sin(actualRotSpeed);
    planeY = oldPlaneX * sin(actualRotSpeed) + planeY * cos(actualRotSpeed);
}

double Player::getPosX() const { return posX; }
double Player::getPosY() const { return posY; }
double Player::getDirX() const { return dirX; }
double Player::getDirY() const { return dirY; }
double Player::getPlaneX() const { return planeX; }
double Player::getPlaneY() const { return planeY; }
