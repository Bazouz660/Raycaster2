#ifndef RAYCASTER_HPP
#define RAYCASTER_HPP

#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Map.hpp"
#include "AEntity.hpp"
#include <memory>

class Raycaster
{
public:
    Raycaster();
    void castRays(sf::RenderTarget &window, const Player &player, std::vector<std::shared_ptr<AEntity>> &sprites, const Map &map);
    void setSize(int width, int height);

private:

    struct Ray
    {
        double dirX, dirY;
        double sideDistX, sideDistY;
        double deltaDistX, deltaDistY;
        double perpWallDist;
        int stepX, stepY;
        int mapX, mapY;
        int side;
        double wallX;
        int drawEnd;
    };

    struct Environment
    {
        double fogStart;
        double fogEnd;
    };

    struct TextureInfo {
        const sf::IntRect textureRect;
        const sf::Texture &texture;
        int texX;
        int texHeight;
        int start;
        int end;
    };

    struct RenderSettings {
        int lineHeight;
        double fogIntensity;
        int side;
    };

    int texWidth = 16;
    int screenWidth = 800;
    int screenHeight = 600;

    std::vector<double> zBuffer;

    sf::VertexBuffer screenBuffer;
    std::vector<sf::Vertex> verticesBuffer;

    std::vector<double> spriteDistances;
    std::vector<int> spriteOrder;

    Environment env;

    void castFloorAndCeiling(sf::RenderTarget &window, const Map &map, int x, const Ray &ray, const Player &player);
    void castWalls(sf::RenderTarget &window, const Player &player, const Map &map, int x, Ray &ray);
    void performDDA(const Player &player, const Map &map, int &x, Ray &ray);
    void prepareSprites(std::vector<std::shared_ptr<AEntity>> &sprites, const Player &player);
    void drawSprites(sf::RenderTarget &window, const Player &player, const Map &map, std::vector<std::shared_ptr<AEntity>> &sprites);
    void drawTexturedVerticalLine(sf::RenderTarget &window, int x, int drawStart, int drawEnd, const TextureInfo& texInfo, const RenderSettings& renderSettings);
};

#endif // RAYCASTER_HPP
