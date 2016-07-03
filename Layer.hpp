#ifndef TMX_TILELAYER_HPP
#define TMX_TILELAYER_HPP

#include <SFML/Graphics/VertexArray.hpp>

#include "Map.hpp"
#include "Tileset.hpp"
#include "Utils.hpp"

namespace tmx
{

class Layer : public detail::LayerBase
{
    public:
        Layer(Map* map);

        typedef std::shared_ptr<Layer> Ptr;

        LayerType getLayerType() const;

        bool loadFromNode(pugi::xml_node& layer);
        void saveToNode(pugi::xml_node& layer);

        void setTileId(sf::Vector2u coords, unsigned int id);
        unsigned int getTileId(sf::Vector2u coords);

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

        bool loadFromCode(std::string const& code);
        std::string getCode();

    protected:
        void update();

    protected:
        Map* mMap;
        Tileset* mTileset;
        sf::VertexArray mVertices;

        std::string mEncoding;
        std::string mCompression;
};

} // namespace tmx

#endif // TMX_LAYER_HPP
