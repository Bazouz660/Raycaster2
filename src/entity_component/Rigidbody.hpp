#pragma once

#include <YPI/math.hpp>
#include "../Map.hpp"
#include "../Vector2.hpp"

class Rigidbody
{
    public:

        Rigidbody() : velocity({0, 0}), acceleration({0, 0}), direction({0, 0}) {}

        void applyForce(const ypi::Vector2f &force)
        {
            acceleration += force;
        }

        void setVelocity(const ypi::Vector2f &vel)
        {
            velocity = vel;
        }

        ypi::Vector2f getVelocity() const
        {
            return velocity;
        }

        void setAcceleration(const ypi::Vector2f &acc)
        {
            acceleration = acc;
        }

        void accelerate(float speed)
        {
            acceleration += direction * speed;
        }

        ypi::Vector2f getAcceleration() const
        {
            return acceleration;
        }

        void update(float dt)
        {
            velocity += acceleration * dt;
            acceleration = {0, 0};
        }

        void setDirection(const ypi::Vector2f &dir)
        {
            direction = {dir.x, dir.y};
            direction = ypi::math::normalize(direction);
        }

        ypi::Vector2f getDirection() const
        {
            return {direction.x, direction.y};
        }

    private:
        ypi::Vector2f velocity;
        ypi::Vector2f acceleration;
        ypi::Vector2f direction;
};