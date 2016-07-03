#ifndef TMX_MAP_HPP
#define TMX_MAP_HPP

#include "Tileset.hpp"
#include "Utils.hpp"

namespace tmx
{

class Map : public detail::PropertiesHolder, public sf::Drawable
{
    public:
        Map();

        bool loadFromFile(std::string const& filename);
        bool saveToFile(std::string const& filename);

        std::size_t getLayerCount() const;
        detail::LayerBase& getLayer(std::size_t index);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void render(std::size_t index, sf::RenderTarget& target, sf::RenderStates states) const;

        Tileset* getTileset(unsigned int id);

        std::string getOrientation() const;
        sf::Vector2u getMapSize() const;
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

        std::vector<Tileset> mTilesets;
        std::vector<detail::LayerBase*> mLayers;
};

} // namespace tmx

#endif // TMX_MAP_HPP
