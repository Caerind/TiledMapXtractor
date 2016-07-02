#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "pugixml.hpp"

#include "Layer.hpp"
#include "Properties.hpp"
#include "Tileset.hpp"

class Map
{
    public:
        Map();

        bool loadFromFile(std::string const& filename);
        bool saveToFile(std::string const& filename);

        void loadTileset(pugi::xml_node& tilesetNode);
        void loadLayer(pugi::xml_node& layerNode);

        std::size_t getLayerCount() const;
        Layer& getLayer(std::size_t index);

        void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());
        void render(std::size_t index, sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());

        Tileset* getTileset(unsigned int id);

        std::string getOrientation() const;
        sf::Vector2u getTileSize() const;

    private:
        float mVersion;
        std::string mOrientation;
        std::string mRenderOrder;
        sf::Vector2u mMapSize;
        sf::Vector2u mTileSize;
        unsigned int mHexSideLength;
        std::string mStaggerAxis;
        std::string mStaggerIndex;
        std::string mBackgroundColor;
        unsigned int mNextObjectId;

        std::vector<Layer> mLayers;
        std::vector<Tileset> mTilesets;

        Properties mProperties;
};

#endif // MAP_HPP
