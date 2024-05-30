#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

class AEntity
{
public:
    AEntity(double x, double y, const sf::Texture &texture);
    double getX() const;
    double getY() const;
    double getElevation() const;
    void setElevation(double z);
    void setRadius(float radius);
    bool collidesWith(const AEntity &other) const;
    bool collidesWith(double x, double y) const;
    const sf::Texture &getTexture() const;
    virtual void update(float dt);

protected:
    double x, y, z;
    float radius = 0.5f;
    const sf::Texture &texture;
};

#endif // ENTITY_HPP
