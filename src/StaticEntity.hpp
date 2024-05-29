#include "AEntity.hpp"

class StaticEntity : public AEntity
{
    public:
        StaticEntity(double x, double y, const sf::Texture &texture);
};