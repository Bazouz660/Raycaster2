#include "EnemyEntity.hpp"
#include <cmath>
#include <iostream>
#include "YPI/data.hpp"

EnemyEntity::EnemyEntity(double x, double y, const sf::Texture &texture, const Player &player, const Map &map)
    : AEntity(x, y, texture), player(player), map(map)
{
    speed = 2.f;
    //sound->setLoop(true);
    //sound->setRelativeToListener(true);
    //sound->setAttenuation(1);
}

void EnemyEntity::update(float dt) {
    Point playerPos = {player.getPosX(), player.getPosY()};
    Point enemyPos = {x, y};
    double distance = std::sqrt((playerPos.x - enemyPos.x) * (playerPos.x - enemyPos.x) + (playerPos.y - enemyPos.y) * (playerPos.y - enemyPos.y));
    if (distance < 1.0) {
        direction = {0, 0};
        //player.damage(1);
    } else if (map.hasLineOfSight(enemyPos, playerPos)) {
        direction = {playerPos.x - enemyPos.x, playerPos.y - enemyPos.y};
    } else {
        std::vector<Point> path = map.aStar(enemyPos, playerPos);
        if (!path.empty()) {
            // move to the first point in the path
            Point nextStep = path.front();
            direction = {nextStep.x - enemyPos.x, nextStep.y - enemyPos.y};
        }
    }
    direction = direction.normalize();
    velocity.x = speed * direction.x * dt;
    velocity.y = speed * direction.y * dt;

    x += velocity.x;
    y += velocity.y;

    //sound->setPosition(x, y, 0);
}