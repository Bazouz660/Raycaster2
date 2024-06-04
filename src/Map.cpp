#include "Map.hpp"
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <random>
#include "YPI/system.hpp"

Map::Map() : textureMap(16, 16)
{
    // Simple map initialization with texture IDs
    int tempWallData[width][height] =
    {
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,0,0,0,6,0,6,0,0,0,0,0,6,0,0,6,0,0,0,0,6,0,0,6},
        {6,0,6,0,6,0,6,6,6,6,0,6,6,6,0,6,0,6,6,0,6,0,6,6},
        {6,0,6,0,0,0,0,0,0,6,0,0,0,0,0,6,0,0,6,0,0,0,6,6},
        {6,0,6,6,6,6,0,6,0,6,6,6,0,6,6,6,0,6,0,6,6,0,6,6},
        {6,0,0,0,0,0,0,6,0,0,0,6,0,0,0,6,0,0,0,0,6,0,6,6},
        {6,6,6,6,6,6,0,6,0,6,0,6,6,6,0,6,6,6,6,0,6,0,6,6},
        {6,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,6,0,0,6},
        {6,0,6,6,0,6,0,6,6,6,6,6,0,6,6,6,6,6,0,6,6,0,6,6},
        {6,0,6,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,6,6},
        {6,0,6,0,6,0,6,6,6,6,0,6,0,6,6,6,6,6,0,6,6,0,6,6},
        {6,0,6,0,6,0,0,0,0,6,0,6,0,0,0,6,0,0,0,0,6,0,6,6},
        {6,0,6,0,6,0,6,6,0,6,0,6,6,6,0,6,0,6,6,0,6,0,6,6},
        {6,0,6,0,6,0,6,6,0,6,0,6,0,0,0,6,0,0,0,0,6,0,6,6},
        {6,0,0,0,0,0,0,6,0,0,0,6,0,6,0,6,0,6,6,0,6,0,0,6},
        {6,6,6,6,6,6,0,6,0,6,6,6,0,6,0,6,0,6,0,6,6,0,6,6},
        {6,0,0,0,0,0,0,6,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,6},
        {6,6,6,6,6,6,0,6,6,6,6,6,6,6,0,6,0,6,0,6,6,0,6,6},
        {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
        {6,0,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,0,6},
        {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
        {6,6,6,6,2,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6}
    };


    int tempFloorData[width][height] =
    {
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,5,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,5,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,5,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,5,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6}
    };

    int tempCeilingData[width][height] =
    {
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,9,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,9,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,9,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,9,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,9,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,9,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,9,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,9,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,9,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,9,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6}
    };

    levelData.resize(3);
    for (int i = 0; i < 3; ++i)
    {
        levelData[i].resize(width);
        for (int j = 0; j < width; ++j)
        {
            levelData[i][j].resize(height);
        }
    }

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            levelData[0][i][j] = tempWallData[i][j];
            levelData[1][i][j] = tempFloorData[i][j];
            levelData[2][i][j] = tempCeilingData[i][j];
        }
    }

    generateCorridor(500);

    // Load the texture map
    textureMap.loadFromFile("asset/texture_map.png");
}

bool Map::isEmpty(int x, int y) const
{
    return levelData[0][x][y] == 0;
}

int Map::getTextureID(int x, int y, int level) const
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        ypi::logger::error() << "Invalid map coordinates: " << x << ", " << y;
    return levelData[level][x][y] - 1;
}

const TextureMap &Map::getTextureMap() const
{
    return textureMap;
}

// Cast a ray from point 'from' to point 'to' and return false if it hits a wall (line of sight test)
bool Map::hasLineOfSight(const Point &from, const Point &to) const
{
    // Bresenham's line algorithm
    int x0 = from.x;
    int y0 = from.y;
    int x1 = to.x;
    int y1 = to.y;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    while (x0 != x1 || y0 != y1)
    {
        if (!isEmpty(x0, y0))
        {
            return false;
        }

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }

    return true;
}

std::vector<Point> Map::aStar(Point start, Point goal) const
{
    // check if the start and goal are valid
    if (!isEmpty(start.x, start.y) || !isEmpty(goal.x, goal.y)) {
        return {};
    }

    // check if the start and goal are the same
    if (start == goal) {
        return {start};
    }

    // check if the start and goal and inside the map
    if (start.x < 0 || start.x >= width || start.y < 0 || start.y >= height ||
        goal.x < 0 || goal.x >= width || goal.y < 0 || goal.y >= height) {
        return {};
    }

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

void Map::generateMaze() {
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::stack<Point> stack;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, 3);

    auto directions = std::vector<Point>{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}; // Up, Right, Down, Left

    // Start the maze generation from a random point
    Point start{1, 1};
    stack.push(start);
    visited[start.y][start.x] = true;
    levelData[0][start.y][start.x] = 0; // Start point is a path

    while (!stack.empty()) {
        Point current = stack.top();
        stack.pop();

        std::vector<int> randDirs = {0, 1, 2, 3};
        std::shuffle(randDirs.begin(), randDirs.end(), rng);

        for (int i : randDirs) {
            Point direction = directions[i];
            Point next = current + direction * 2;

            if (next.x > 0 && next.x < width-1 && next.y > 0 && next.y < height-1) {
                if (!visited[next.y][next.x]) {
                    visited[next.y][next.x] = true;
                    stack.push(next);
                    levelData[0][next.y][next.x] = 0;
                    levelData[0][current.y + direction.y][current.x + direction.x] = 0;
                }
            }
        }
    }
}

// generate a closed corridor map with a given length (basically a hollow rectangle)
void Map::generateCorridor(unsigned int length)
{
    this->width = length;
    this->height = 3;

    levelData.resize(3);

    for (int i = 0; i < 3; ++i)
    {
        levelData[i].resize(width);
        for (int j = 0; j < width; ++j)
        {
            levelData[i][j].resize(height);
        }
    }

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            levelData[0][i][j] = 6;
            levelData[1][i][j] = 5;
            levelData[2][i][j] = 6;
        }
    }

    // cut a path in the middle of the map
    for (int i = 1; i < width -1; ++i)
    {
        levelData[0][i][1] = 0;
    }
}