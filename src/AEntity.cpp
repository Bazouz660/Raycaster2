#include "AEntity.hpp"
#include <cmath>

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

void AEntity::setRadius(float radius)
{
    this->radius = radius;
}

bool AEntity::collidesWith(const AEntity &other) const
{
    double dx = x - other.x;
    double dy = y - other.y;
    double distance = std::sqrt(dx * dx + dy * dy);
    return distance < radius + other.radius;
}

bool AEntity::collidesWith(double x, double y) const
{
    double dx = this->x - x;
    double dy = this->y - y;
    double distance = std::sqrt(dx * dx + dy * dy);
    return distance < radius;
}