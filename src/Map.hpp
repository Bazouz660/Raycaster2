#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "TextureMap.hpp"
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <algorithm>

struct Point {
    int x, y;

    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
    bool operator!=(const Point& o) const { return !(*this == o); }
    Point operator+(const Point& o) const { return {x + o.x, y + o.y}; }

    bool operator<(const Point& o) const {
        return std::tie(x, y) < std::tie(o.x, o.y);
    }

    Point operator*(int i) const {
        return {x * i, y * i};
    }
};

namespace std {
    template <>
    struct hash<Point> {
        size_t operator()(const Point& p) const {
            return hash<int>()(p.x) ^ hash<int>()(p.y);
        }
    };
}

class Map
{
public:
    Map();
    bool isEmpty(int x, int y) const;
    int getTextureID(int x, int y, int level) const;
    const TextureMap &getTextureMap() const;
    std::vector<Point> aStar(Point start, Point goal) const;

    // Cast a ray from point 'from' to point 'to' and return true if it hits a wall (line of sight test)
    bool hasLineOfSight(const Point &from, const Point &to) const;

    using MapData = std::vector<std::vector<int>>;

    int width = 24;
    int height = 24;
    std::vector<MapData> levelData;

private:
    TextureMap textureMap;

    double heuristic(Point a, Point b) const {
        return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
    }

    void generateMaze();

    void generateCorridor(unsigned int length);
};

#endif // MAP_HPP
