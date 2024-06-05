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

    //generateCorridor(500);
    //generateMaze();

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
bool Map::hasLineOfSight(const sf::Vector2f &from, const sf::Vector2f &to) const
{
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    float steps = std::max(abs(dx), abs(dy));

    float xIncrement = dx / steps;
    float yIncrement = dy / steps;

    float x = from.x;
    float y = from.y;

    for (int i = 0; i <= steps; i++) {
        if (!isEmpty(std::floor(x), std::floor(y))) {
            return false;
        }
        x += xIncrement;
        y += yIncrement;
    }

    return true;
}

std::vector<Point> Map::getNeighbors(const Point& current) const {
    std::vector<Point> neighbors;
    // Define your logic to find neighbors
    // Example: Consider neighbors within a radius
    float radius = 1.0; // Define your radius based on game logic
    int steps = 8; // Number of directions to check
    for (int i = 0; i < steps; i++) {
        float angle = 2 * std::numbers::pi * i / steps;
        Point neighbor{current.x + radius * std::cos(angle), current.y + radius * std::sin(angle)};
        if (isEmpty(neighbor.x, neighbor.y)) {
            neighbors.push_back(neighbor);
        }
    }
    return neighbors;
}

std::vector<Point> Map::aStar(const Point& start, const Point& goal) const {
    std::priority_queue<std::pair<float, Point>, std::vector<std::pair<float, Point>>, std::greater<>> openSet;
    std::unordered_map<Point, Point> cameFrom;
    std::unordered_map<Point, float> gScore;
    std::unordered_map<Point, float> fScore;

    gScore[start] = 0.0;
    fScore[start] = heuristic(start, goal);
    openSet.emplace(fScore[start], start);

    while (!openSet.empty()) {
        Point current = openSet.top().second;
        if (current == goal) {
            std::vector<Point> path;
            while (cameFrom.find(current) != cameFrom.end()) {
                path.push_back(current);
                current = cameFrom[current];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        openSet.pop();

        for (const Point& neighbor : getNeighbors(current)) {
            float tentative_gScore = gScore[current] + heuristic(current, neighbor);
            if (tentative_gScore < gScore[neighbor] || gScore.find(neighbor) == gScore.end()) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = tentative_gScore + heuristic(neighbor, goal);
                openSet.emplace(fScore[neighbor], neighbor);
            }
        }
    }

    return {}; // No path found
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