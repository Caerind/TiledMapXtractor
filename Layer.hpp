#ifndef LAYERCOMPONENT_HPP
#define LAYERCOMPONENT_HPP

#include <sstream>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "Compression.hpp"
#include "pugixml.hpp"

#include "Properties.hpp"
#include "Tileset.hpp"

class Map;
class Layer
{
    public:
        Layer(Map* map);

        bool loadFromNode(pugi::xml_node& layer);
        bool saveToNode(pugi::xml_node& layer);

        void setTileId(sf::Vector2u coords, unsigned int id);
        unsigned int getTileId(sf::Vector2u coords);

        std::string getName() const;

        void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());

        bool loadFromCode(std::string const& code);
        std::string getCode();

    protected:
        void update();

    protected:
        Map* mMap;
        Tileset* mTileset;
        sf::VertexArray mVertices;

        std::string mName;
        sf::Vector2u mSize;
        float mOpacity;
        bool mVisible;
        sf::Vector2f mOffset;

        std::string mEncoding;
        std::string mCompression;

        Properties mProperties;
};

#endif // LAYERCOMPONENT_HPP
