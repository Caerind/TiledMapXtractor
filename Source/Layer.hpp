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

        sf::Vector2i worldToCoords(sf::Vector2f const& world);

        void setTileId(sf::Vector2i coords, unsigned int id);
        unsigned int getTileId(sf::Vector2i coords);

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

        bool loadFromCode(std::string const& code);
        std::string getCode();

        const std::string& getEncoding() const;
        void setEncoding(std::string const& encoding);

        const std::string& getCompression() const;
        void setCompression(std::string const& compression);

        void update();

    protected:
        sf::Vertex* getVertex(sf::Vector2i const& coords);

    protected:
        Map& mMap;
        Tileset* mTileset;
        sf::VertexArray mVertices;

        std::string mEncoding;
        std::string mCompression;
};

} // namespace tmx

#endif // TMX_LAYER_HPP
