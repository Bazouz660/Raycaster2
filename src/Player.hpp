#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Map.hpp"
#include <SFML/Window.hpp>

class Player
{
public:
    Player(double posX, double posY, double dirX, double dirY, double planeX, double planeY);
    void handleInput(const Map &map, const sf::Window &window, float dt);

    double getPosX() const;
    double getPosY() const;
    double getDirX() const;
    double getDirY() const;
    double getPlaneX() const;
    double getPlaneY() const;

private:
    void handleMouseMovement(int mouseXDelta, const sf::Window &window, float dt);
    bool isCollision(double x, double y, const Map &map);

    double posX, posY;
    double dirX, dirY;
    double planeX, planeY;
    const double moveSpeed = 10.0;
    const double rotSpeed = 1.5;
    const double collisionRadius = 0.2; // Radius around the player for collision detection
};

#endif // PLAYER_HPP
