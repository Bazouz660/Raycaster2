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
    const sf::Texture &getTexture() const;
    virtual void update(float dt);

private:
    double x, y, z;
    const sf::Texture &texture;
};

#endif // ENTITY_HPP
