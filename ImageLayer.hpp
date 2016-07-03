#ifndef TMX_IMAGELAYER_HPP
#define TMX_IMAGELAYER_HPP

#include <SFML/Graphics/Sprite.hpp>

#include "Utils.hpp"

namespace tmx
{

class ImageLayer : public detail::LayerBase
{
    public:
        ImageLayer();

        LayerType getLayerType() const;

        bool loadFromNode(pugi::xml_node& layer);
        void saveToNode(pugi::xml_node& layer);

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

    protected:
        detail::Image mImage;
        sf::Sprite mSprite;
        sf::Texture mTexture;
};

} // namespace tmx

#endif // TMX_IMAGELAYER_HPP
