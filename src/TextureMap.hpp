#include <SFML/Graphics.hpp>

class TextureMap
{
public:
    TextureMap(const int width, const int height);
    sf::IntRect getTextureRect(int index) const;
    void loadFromFile(const std::string &filename);
    const sf::Texture &getTexture() const;
    unsigned int getTextureCount() const { return textureRects.size(); }

private:
    const int width, height; // resolution of a single texture
    sf::Texture texture;
    std::vector<sf::IntRect> textureRects;

    bool isTransparent(const sf::IntRect &rect, const sf::Image& image) const;
};
