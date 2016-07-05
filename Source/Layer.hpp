#ifndef TMX_TILELAYER_HPP
#define TMX_TILELAYER_HPP

#include "Utils.hpp"

namespace tmx
{

class Map;
class Tileset;

class Layer : public LayerBase
{
    public:
        Layer(Map& map);

        LayerType getLayerType() const;

        bool loadFromNode(pugi::xml_node const& layer);
        void saveToNode(pugi::xml_node& layer);

        void setTileId(sf::Vector2u coords, unsigned int id);
        unsigned int getTileId(sf::Vector2u coords);

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

        bool loadFromCode(std::string const& code);
        std::string getCode();

        void update();

    protected:
        sf::Vertex* getVertex(sf::Vector2u const& coords);

    protected:
        Map& mMap;
        Tileset* mTileset;
        sf::VertexArray mVertices;

        std::string mEncoding;
        std::string mCompression;
};

} // namespace tmx

#endif // TMX_LAYER_HPP
