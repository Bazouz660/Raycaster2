#pragma once

#include <SFML/Graphics.hpp>

template <typename T>
class Vector2
{
    public:
        friend class sf::Vector2<T>;

        T x, y = 0;

        // initializer list
        Vector2(T x = 0, T y = 0) : x(x), y(y) {}

        Vector2<T> normalize() const
        {
            if (x == 0 && y == 0)
                return Vector2<T>(0, 0);
            T length = sqrt(x * x + y * y);
            return Vector2<T>(x / length, y / length);
        }

        T dot(const Vector2<T> &other) const
        {
            return x * other.x + y * other.y;
        }
};

typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;
typedef Vector2<unsigned int> Vector2u;
