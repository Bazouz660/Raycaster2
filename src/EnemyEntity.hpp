#pragma once

#include "AEntity.hpp"
#include "Player.hpp"
#include "Vector2.hpp"
#include <SFML/Audio.hpp>
#include <memory>

class EnemyEntity : public AEntity
{
    public:
        EnemyEntity(double x, double y, const sf::Texture &texture, const Player &player, const Map &map);

        virtual void update(float dt) override;

    private:
        const Player &player;
        const Map &map;
        float speed = 1.0f;
        Vector2f direction;
        Vector2f velocity;
        std::shared_ptr<sf::Sound> sound;
};