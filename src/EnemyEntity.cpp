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
    ypi::Vector2f playerPos(player.getPosX(), player.getPosY());
    ypi::Vector2f enemyPos(x, y);
    double distance = std::sqrt((playerPos.x - enemyPos.x) * (playerPos.x - enemyPos.x) + (playerPos.y - enemyPos.y) * (playerPos.y - enemyPos.y));

    if (distance < 1.0) {
        rigidbody.setDirection({0, 0});
        //player.damage(1);
    } else if (map.hasLineOfSight(enemyPos, playerPos)) {
        rigidbody.setDirection(ypi::Vector2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y));
    } else {
        std::vector<Point> path = map.aStar({enemyPos.x, enemyPos.y}, {playerPos.x, playerPos.y});
        if (!path.empty()) {
            // move to the first point in the path
            Point nextStep = path.front();
            rigidbody.setDirection(ypi::Vector2f(nextStep.x - enemyPos.x, nextStep.y - enemyPos.y));
        }
    }

    rigidbody.accelerate(speed);
    rigidbody.update(dt);

    x += rigidbody.getVelocity().x;
    y += rigidbody.getVelocity().y;

    rigidbody.setVelocity({0, 0});

    //sound->setPosition(x, y, 0);
}