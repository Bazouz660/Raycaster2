#include "StaticEntity.hpp"

class ItemEntity : public StaticEntity
{
    public:
        ItemEntity(double x, double y, const sf::Texture &texture);
        virtual void update(float dt) override;

    private:
        float timer = 0.0f;
};