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

        typedef std::shared_ptr<ImageLayer> Ptr;

        LayerType getLayerType() const;

        bool loadFromNode(pugi::xml_node const& layer);
        void saveToNode(pugi::xml_node& layer);

        const std::string& getSource() const;
        const std::string& getFormat() const;
        sf::Color getTransparent() const;
        const sf::Vector2u& getSize() const;

        void setSource(std::string const& source);
        void setFormat(std::string const& format);
        void setTransparent(sf::Color const& transparent);
        void setSize(sf::Vector2u const& size);

        bool loadImage();

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

    protected:
        detail::Image mImage;
        sf::Sprite mSprite;
        sf::Texture mTexture;
};

} // namespace tmx

#endif // TMX_IMAGELAYER_HPP
