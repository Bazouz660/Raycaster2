#include "Raycaster.hpp"
#include <cmath>
#include <iostream>

// This function sorts sprites from farthest to nearest
void sortSprites(int* order, double* dist, int count) {
    std::vector<std::pair<double, int>> spritePairs(count);
    for (int i = 0; i < count; ++i) {
        spritePairs[i] = std::make_pair(dist[i], order[i]);
    }
    std::sort(spritePairs.begin(), spritePairs.end(), [](const std::pair<double, int>& left, const std::pair<double, int>& right) {
        return left.first > right.first; // Sort in descending order
    });
    for (int i = 0; i < count; ++i) {
        dist[i] = spritePairs[i].first;
        order[i] = spritePairs[i].second;
    }
}

Raycaster::Raycaster()
{
    env.fogStart = 0.0;
    env.fogEnd = 5.0;
}

void Raycaster::setSize(int width, int height)
{
    this->screenWidth = width;
    this->screenHeight = height;

    screenBuffer.setPrimitiveType(sf::PrimitiveType::Points);
    screenBuffer.setUsage(sf::VertexBuffer::Usage::Dynamic);
    screenBuffer.create(width * height);

    zBuffer.resize(screenWidth);

    verticesBuffer.resize(width * height);
}

void Raycaster::prepareSprites(std::vector<std::shared_ptr<AEntity>> &sprites, const Player &player)
{
    // Prepare sprite data once per frame, not per column
    spriteDistances.resize(sprites.size());
    spriteOrder.resize(sprites.size());
    for (size_t i = 0; i < sprites.size(); ++i) {
        spriteOrder[i] = i;
        double dx = player.getPosX() - sprites[i]->getX();
        double dy = player.getPosY() - sprites[i]->getY();
        spriteDistances[i] = dx * dx + dy * dy;  // Square of the distance
    }

    sortSprites(spriteOrder.data(), spriteDistances.data(), sprites.size());
}

void Raycaster::performDDA(const Player &player, const Map &map, int &x, Ray &ray)
{
    double cameraX = 2 * x / double(screenWidth) - 1;
    ray.dirX = player.getDirX() + player.getPlaneX() * cameraX;
    ray.dirY = player.getDirY() + player.getPlaneY() * cameraX;

    ray.mapX = int(player.getPosX());
    ray.mapY = int(player.getPosY());

    ray.deltaDistX = std::abs(1 / ray.dirX);
    ray.deltaDistY = std::abs(1 / ray.dirY);

    int hit = 0;

    if (ray.dirX < 0) {
        ray.stepX = -1;
        ray.sideDistX = (player.getPosX() - ray.mapX) * ray.deltaDistX;
    } else {
        ray.stepX = 1;
        ray.sideDistX = (ray.mapX + 1.0 - player.getPosX()) * ray.deltaDistX;
    }
    if (ray.dirY < 0) {
        ray.stepY = -1;
        ray.sideDistY = (player.getPosY() - ray.mapY) * ray.deltaDistY;
    } else {
        ray.stepY = 1;
        ray.sideDistY = (ray.mapY + 1.0 - player.getPosY()) * ray.deltaDistY;
    }

    while (hit == 0) {
        if (ray.sideDistX < ray.sideDistY) {
            ray.sideDistX += ray.deltaDistX;
            ray.mapX += ray.stepX;
            ray.side = 0;
        } else {
            ray.sideDistY += ray.deltaDistY;
            ray.mapY += ray.stepY;
            ray.side = 1;
        }
        // Ensure ray.mapX and ray.mapY are within map boundaries
        if (ray.mapX < 0 || ray.mapX >= map.width || ray.mapY < 0 || ray.mapY >= map.height) {
            hit = 1;
        } else if (map.levelData[0][ray.mapX][ray.mapY] > 0) {
            hit = 1;
        }
    }

    if (ray.side == 0) ray.perpWallDist = (ray.mapX - player.getPosX() + (1 - ray.stepX) / 2) / ray.dirX;
    else          ray.perpWallDist = (ray.mapY - player.getPosY() + (1 - ray.stepY) / 2) / ray.dirY;
}

void Raycaster::castFloorAndCeiling(sf::RenderTarget &window, const Map &map, int x, const Ray &ray, const Player &player)
{
    // Floor casting
    double floorXWall, floorYWall;
    if (ray.side == 0 && ray.dirX > 0) {
        floorXWall = ray.mapX;
        floorYWall = ray.mapY + ray.wallX;
    } else if (ray.side == 0 && ray.dirX < 0) {
        floorXWall = ray.mapX + 1.0;
        floorYWall = ray.mapY + ray.wallX;
    } else if (ray.side == 1 && ray.dirY > 0) {
        floorXWall = ray.mapX + ray.wallX;
        floorYWall = ray.mapY;
    } else {
        floorXWall = ray.mapX + ray.wallX;
        floorYWall = ray.mapY + 1.0;
    }

    double distWall, distPlayer, currentDist;

    distWall = ray.perpWallDist;
    distPlayer = 0.0;

    int drawEnd = ray.drawEnd;

    if (drawEnd < 0) drawEnd = screenHeight;

    for (int y = drawEnd + 1; y < screenHeight; y++) {
        currentDist = screenHeight / (2.0 * y - screenHeight);

        double weight = (currentDist - distPlayer) / (distWall - distPlayer);

        double currentFloorX = weight * floorXWall + (1.0 - weight) * player.getPosX();
        double currentFloorY = weight * floorYWall + (1.0 - weight) * player.getPosY();

        sf::IntRect floorTextureRect = map.getTextureMap().getTextureRect(map.getTextureID(int(currentFloorX), int(currentFloorY), 1));
        sf::IntRect ceilingTextureRect = map.getTextureMap().getTextureRect(map.getTextureID(int(currentFloorX), int(currentFloorY), 2));

        int floorTexX, floorTexY, ceilingTexX, ceilingTexY;
        floorTexX = int(currentFloorX * floorTextureRect.width) % floorTextureRect.width;
        floorTexY = int(currentFloorY * floorTextureRect.height) % floorTextureRect.height;

        ceilingTexX = floorTexX;
        ceilingTexY = floorTexY;

        ceilingTexX += ceilingTextureRect.left;
        ceilingTexY += ceilingTextureRect.top;

        floorTexX += floorTextureRect.left;
        floorTexY += floorTextureRect.top;


        // Calculate fog intensity based on distance
        double fogIntensity = std::min(std::max((currentDist - env.fogStart) / (env.fogEnd - env.fogStart), 0.0), 1.0);
        double invFogIntensity = 1.0 - fogIntensity;

        sf::Color color = sf::Color(255 * invFogIntensity, 255 * invFogIntensity, 255 * invFogIntensity);

        // Floor
        verticesBuffer[x + y * screenWidth].position = sf::Vector2f(x, y - 1);
        verticesBuffer[x + y * screenWidth].color = color;
        verticesBuffer[x + y * screenWidth].texCoords = sf::Vector2f(floorTexX, floorTexY);

        // Ceiling (symmetrical)
        verticesBuffer[x + (screenHeight - y) * screenWidth].position = sf::Vector2f(x, screenHeight - y);
        verticesBuffer[x + (screenHeight - y) * screenWidth].color = color;
        verticesBuffer[x + (screenHeight - y) * screenWidth].texCoords = sf::Vector2f(ceilingTexX, ceilingTexY);
    }
}

void Raycaster::castWalls(sf::RenderTarget &window, const Player &player, const Map &map, int x, Ray &ray)
{
    int lineHeight = (int)(screenHeight / ray.perpWallDist);

    int drawStart = -lineHeight / 2 + screenHeight / 2;
    if (drawStart < 0) drawStart = 0;
    int drawEnd = lineHeight / 2 + screenHeight / 2;
    if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;
    ray.drawEnd = drawEnd;

    int textureID = map.getTextureID(ray.mapX, ray.mapY, 0);
    sf::IntRect textureRect = map.getTextureMap().getTextureRect(textureID);

    double wallX;
    if (ray.side == 0) wallX = player.getPosY() + ray.perpWallDist * ray.dirY;
    else           wallX = player.getPosX() + ray.perpWallDist * ray.dirX;
    wallX -= floor(wallX);
    ray.wallX = wallX;

    int texX = int(wallX * double(textureRect.width));
    if (ray.side == 0 && ray.dirX > 0) texX = textureRect.width - texX - 1;
    if (ray.side == 1 && ray.dirY < 0) texX = textureRect.width - texX - 1;

    // Calculate the intensity of the fog (linear interpolation between fogStart and fogEnd)
    double fogIntensity = std::min(std::max((ray.perpWallDist - env.fogStart) / (env.fogEnd - env.fogStart), 0.0), 1.0);

    RenderSettings renderSettings = {lineHeight, fogIntensity, ray.side};
    TextureInfo textureInfo = {textureRect, map.getTextureMap().getTexture(), texX, textureRect.height, drawStart, drawEnd};

    drawTexturedVerticalLine(window, x, drawStart, drawEnd, textureInfo, renderSettings);
}

void Raycaster::castRays(sf::RenderTarget &window, const Player &player, std::vector<std::shared_ptr<AEntity>> &sprites ,const Map &map)
{
    const int screenWidth = window.getSize().x;
    const int screenHeight = window.getSize().y;

    zBuffer.resize(screenWidth);

    std::fill(verticesBuffer.begin(), verticesBuffer.end(), sf::Vertex(sf::Vector2f(0, 0), sf::Color::Transparent));

    // 1. Prepare the sprites
    prepareSprites(sprites, player);

    // 2. Transform and draw each wall stripe
    Ray ray;
    for (int x = 0; x < screenWidth; x++)
    {
        // Calculate ray information
        performDDA(player, map, x, ray);

        // Draw walls
        castWalls(window, player, map, x, ray);

        // Store the perpendicular wall distance in the Z-buffer
        zBuffer[x] = ray.perpWallDist;

        // Draw the floor and ceiling
        castFloorAndCeiling(window, map, x, ray, player);
    }

    screenBuffer.update(verticesBuffer.data());

    window.draw(screenBuffer, &map.getTextureMap().getTexture());

    // 2. Transform and draw each sprite
    drawSprites(window, player, map, sprites);
}

void Raycaster::drawSprites(sf::RenderTarget &window, const Player &player, const Map &map, std::vector<std::shared_ptr<AEntity>> &sprites)
{
    for (size_t i = 0; i < sprites.size(); ++i) {
        auto& sprite = sprites[spriteOrder[i]];
        double spriteX = sprite->getX() - player.getPosX();
        double spriteY = sprite->getY() - player.getPosY();
        double spriteElevation = sprite->getElevation();

        // Inverse camera matrix
        double invDet = 1.0 / (player.getPlaneX() * player.getDirY() - player.getDirX() * player.getPlaneY());

        double transformX = invDet * (player.getDirY() * spriteX - player.getDirX() * spriteY);
        double transformY = invDet * (-player.getPlaneY() * spriteX + player.getPlaneX() * spriteY);

        int elevationAdjustment = static_cast<int>((spriteElevation / transformY) * window.getSize().y);

        int spriteScreenX = int((window.getSize().x / 2) * (1 + transformX / transformY));

        // Calculate height of the sprite on screen
        int spriteHeight = abs(int(window.getSize().y / (transformY))); // Using "transformY" instead of the real distance to avoid fisheye effect
        int drawStartY = -spriteHeight / 2 + window.getSize().y / 2;
        int drawEndY = spriteHeight / 2 + window.getSize().y / 2;
        int drawStartYWithOffset = -spriteHeight / 2 + window.getSize().y / 2 + elevationAdjustment;
        int drawEndYWithOffset = spriteHeight / 2 + window.getSize().y / 2 + elevationAdjustment;

        // Calculate width of the sprite
        int spriteWidth = abs(int(window.getSize().y / (transformY)));
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0) drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= window.getSize().x) drawEndX = window.getSize().x - 1;

        // Compute the fog intensity for the sprite
        double spriteDistance = sqrt(spriteX * spriteX + spriteY * spriteY);
        double fogIntensity = std::min(std::max((spriteDistance - env.fogStart) / (env.fogEnd - env.fogStart), 0.0), 1.0);

        sf::IntRect textureRect(0, 0, sprite->getTexture().getSize().x, sprite->getTexture().getSize().y);

        // Draw each stripe of the sprite
        for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
            int texX = int((stripe - (-spriteWidth / 2 + spriteScreenX)) * double(sprite->getTexture().getSize().x) / spriteWidth);
            if (transformY > 0 && stripe > 0 && stripe < window.getSize().x && transformY < zBuffer[stripe]) { // Conditions to avoid drawing sprites behind walls
                RenderSettings renderSettings = {spriteHeight, fogIntensity, 0};
                TextureInfo textureInfo = {textureRect, sprite->getTexture(), texX, textureRect.height, drawStartY, drawEndY};
                drawTexturedVerticalLine(window, stripe, drawStartYWithOffset, drawEndYWithOffset, textureInfo, renderSettings);
            }
        }
    }
}

void Raycaster::drawTexturedVerticalLine(sf::RenderTarget &window, int x, int drawStart, int drawEnd, const TextureInfo& texInfo, const RenderSettings& renderSettings)
{
    int screenHeight = window.getSize().y;
    int lineHeight = renderSettings.lineHeight;
    double fogIntensity = renderSettings.fogIntensity;
    int side = renderSettings.side;

    const sf::IntRect& textureRect = texInfo.textureRect;
    const sf::Texture& texture = texInfo.texture;
    int texX = texInfo.texX;
    int texHeight = texInfo.texHeight;

    sf::VertexArray quads(sf::Quads, 4);

    // Set vertex positions
    quads[0].position = sf::Vector2f(x, drawStart);
    quads[1].position = sf::Vector2f(x, drawEnd);
    quads[2].position = sf::Vector2f(x + 1, drawEnd);
    quads[3].position = sf::Vector2f(x + 1, drawStart);


    // Compute the starting and ending texture coordinates
    int startOffset = texInfo.start * 256 - screenHeight * 128 + lineHeight * 128;
    int endOffset = texInfo.end * 256 - screenHeight * 128 + lineHeight * 128;

    float texStartY = ((startOffset * texHeight) / lineHeight) / 256.0f;
    float texEndY = ((endOffset * texHeight) / lineHeight) / 256.0f;
    texStartY += textureRect.top;
    texEndY += textureRect.top;

    // Set texture coordinates
    quads[0].texCoords = sf::Vector2f(texX, texStartY);
    quads[1].texCoords = sf::Vector2f(texX, texEndY);
    quads[2].texCoords = sf::Vector2f(texX + 1, texEndY);
    quads[3].texCoords = sf::Vector2f(texX + 1, texStartY);

    // Set color based on the side
    sf::Color color = (side == 1) ? sf::Color(150, 150, 150) : sf::Color(255, 255, 255);
    // Mix the color with the fog color based on the fog intensity (linear interpolation) at full intensity the color is completely replaced by the fog color
    int r = static_cast<int>(color.r * (1 - fogIntensity) + 0.0 * fogIntensity);
    int g = static_cast<int>(color.g * (1 - fogIntensity) + 0.0 * fogIntensity);
    int b = static_cast<int>(color.b * (1 - fogIntensity) + 0.0 * fogIntensity);

    color = sf::Color(r, g, b);
    for (int i = 0; i < 4; ++i) {
        quads[i].color = color;
    }

    sf::RenderStates states;
    states.texture = &texture;
    window.draw(quads, states);
}
