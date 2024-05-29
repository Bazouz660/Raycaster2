#include "TextureMap.hpp"
#include <iostream>

TextureMap::TextureMap(const int width, const int height)
    : width(width), height(height)
{
}

sf::IntRect TextureMap::getTextureRect(int index) const
{
    return textureRects.at(index);
}

bool TextureMap::isTransparent(const sf::IntRect &rect, const sf::Image& image) const
{
    // Check if the rectangle only contains transparent pixels
    for (int i = rect.left; i < rect.left + rect.width; ++i)
    {
        for (int j = rect.top; j < rect.top + rect.height; ++j)
        {
            if (image.getPixel(i, j) != sf::Color::Transparent)
            {
                return false;
            }
        }
    }

    return true;
}

void TextureMap::loadFromFile(const std::string &filename)
{
    texture.loadFromFile(filename);

    // Calculate the number of textures in the texture map
    int textureMapWidth = texture.getSize().x / width;
    int textureMapHeight = texture.getSize().y / height;

    sf::Image image = texture.copyToImage();

    // Only add the texture rect if it is not transparent
    for (int i = 0; i < textureMapWidth; ++i)
    {
        for (int j = 0; j < textureMapHeight; ++j)
        {
            sf::IntRect rect(i * width, j * height, width, height);
            if (!isTransparent(rect, image))
            {
                textureRects.push_back(rect);
            }
        }
    }

    std::cout << "Loaded " << textureRects.size() << " textures" << std::endl;
}

const sf::Texture &TextureMap::getTexture() const
{
    return texture;
}
