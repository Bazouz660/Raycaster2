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

    using MapData = std::vector<std::vector<int>>;

    static const int width = 24;
    static const int height = 24;
    std::vector<MapData> levelData;

private:
    TextureMap textureMap;

    double heuristic(Point a, Point b) const {
        return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
    }

public:
    std::vector<Point> aStar(Point start, Point goal) const {
        std::priority_queue<Point> openSet;
        std::unordered_set<Point> closedSet;
        std::unordered_map<Point, Point> cameFrom;

        std::unordered_map<Point, double> gScore;
        gScore[start] = 0;

        std::unordered_map<Point, double> fScore;
        fScore[start] = heuristic(start, goal);

        openSet.push(start);

        while (!openSet.empty()) {
            Point current = openSet.top();
            if (current == goal) {
                std::vector<Point> path;
                while (current != start) {
                    path.push_back(current);
                    current = cameFrom[current];
                }
                std::reverse(path.begin(), path.end());
                return path;
            }

            openSet.pop();
            closedSet.insert(current);

            // Neighbors could be defined by your game logic
            for (Point neighbor : {Point{-1, 0}, Point{1, 0}, Point{0, -1}, Point{0, 1}}) {
                Point next = current + neighbor;
                if (!isEmpty(next.x, next.y) || closedSet.find(next) != closedSet.end()) continue;

                double tentative_gScore = gScore[current] + 1; // Assuming cost between neighboring nodes is 1
                if (!gScore.count(next) || tentative_gScore < gScore[next]) {
                    cameFrom[next] = current;
                    gScore[next] = tentative_gScore;
                    fScore[next] = gScore[next] + heuristic(next, goal);
                    openSet.push(next);
                }
            }
        }
        return {}; // return empty path if no path is found
    }
};

#endif // MAP_HPP
