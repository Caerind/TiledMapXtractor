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
        LayerBase::Ptr getLayer(std::size_t index);
        LayerType getLayerType(std::size_t index);
        template <typename T>
        std::shared_ptr<T> getLayer(std::size_t index);
        template <typename T>
        std::shared_ptr<T> createLayer(std::string const& name);
        void removeLayer(std::string const& name);

        void renderBackground(sf::RenderTarget& target);
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
        std::vector<LayerBase::Ptr> mLayers;
};

template <typename T>
std::shared_ptr<T> Map::getLayer(std::size_t index)
{
    return std::static_pointer_cast<T>(mLayers[index]);
}

template <typename T>
std::shared_ptr<T> Map::createLayer(std::string const& name)
{
    if (name != "")
    {
        if (std::find_if(mLayers.begin(),mLayers.end(),[&name](LayerBase::Ptr l)->bool{return (l->getName() == name);}) == mLayers.end())
        {
            std::shared_ptr<T> p = std::make_shared<T>(*this);
            p->setName(name);
            mLayers.push_back(p);
            return p;
        }
    }
    return nullptr;
}

} // namespace tmx

#endif // TMX_MAP_HPP
