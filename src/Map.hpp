#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "TextureMap.hpp"

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
};

#endif // MAP_HPP
