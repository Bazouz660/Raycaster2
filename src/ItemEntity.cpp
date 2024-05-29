#include "ItemEntity.hpp"
#include <cmath>
#include <numbers>

ItemEntity::ItemEntity(double x, double y, const sf::Texture &texture)
    : StaticEntity(x, y, texture)
{
}

void ItemEntity::update(float dt)
{
    timer += dt;
    // make the item bob up and down in a sine wave between -0.5 and 0
    float speed = 1.0;
    float min = -0.1;
    float max = 0.0;
    setElevation(min + (max - min) * std::sin(timer * speed * std::numbers::pi));
}
