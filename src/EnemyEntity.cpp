#include "EnemyEntity.hpp"
#include <cmath>

EnemyEntity::EnemyEntity(double x, double y, const sf::Texture &texture, const Player &player, const Map &map)
    : AEntity(x, y, texture), player(player), map(map)
{
}

void EnemyEntity::update(float dt) {
    Point playerPos = {player.getPosX(), player.getPosY()};
    Point enemyPos = {x, y};
    double distance = std::sqrt((playerPos.x - enemyPos.x) * (playerPos.x - enemyPos.x) + (playerPos.y - enemyPos.y) * (playerPos.y - enemyPos.y));
    if (distance < 1.0) {
        //player.damage(1);
    } else if (map.isEmpty(enemyPos.x + speed * (playerPos.x - enemyPos.x) / distance * dt, enemyPos.y + speed * (playerPos.y - enemyPos.y) / distance * dt)) {
        x += speed * (playerPos.x - enemyPos.x) / distance * dt;
        y += speed * (playerPos.y - enemyPos.y) / distance * dt;
    } else {
        std::vector<Point> path = map.aStar(enemyPos, playerPos);
        if (!path.empty()) {
            // move to the first point in the path
            Point nextStep = path.front();
            x = nextStep.x;
            y = nextStep.y;
        }
    }
}