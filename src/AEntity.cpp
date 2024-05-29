#include "AEntity.hpp"

AEntity::AEntity(double x, double y, const sf::Texture &texture)
    : x(x), y(y), texture(texture)
{
}

double AEntity::getX() const
{
    return x;
}

double AEntity::getY() const
{
    return y;
}

double AEntity::getElevation() const
{
    return z;
}

void AEntity::setElevation(double z)
{
    this->z = z;
}

const sf::Texture &AEntity::getTexture() const
{
    return texture;
}

void AEntity::update(float dt)
{
    (void)dt;
}