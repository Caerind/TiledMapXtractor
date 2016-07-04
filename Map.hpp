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
        LayerBase* getLayer(std::size_t index);
        LayerType getLayerType(std::size_t index);
        template <typename T>
        T* getLayer(std::size_t index);
        template <typename T>
        T* createLayer(std::string const& name);
        void removeLayer(std::string const& name);

        void renderBackground(sf::RenderTarget& target);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void render(std::size_t index, sf::RenderTarget& target, sf::RenderStates states) const;

        Tileset* getTileset(unsigned int id);

        std::string getOrientation() const;
        std::string getRenderOrder() const;
        sf::Vector2u getMapSize() const;
        sf::Vector2u getTileSize() const;
        unsigned int getHexSideLength() const;
        std::string getStaggerAxis() const;
        std::string getStaggerIndex() const;

        bool getRenderObjects() const;
        void setRenderObjects(bool renderObjects);

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

        bool mRenderObjects;

        std::vector<Tileset> mTilesets;
        std::vector<LayerBase*> mLayers;
};

template <typename T>
T* Map::getLayer(std::size_t index)
{
    return static_cast<T>(mLayers[index]);
}

template <typename T>
T* Map::createLayer(std::string const& name)
{
    if (name != "")
    {
        if (std::find_if(mLayers.begin(),mLayers.end(),[&name](LayerBase* l)->bool{return (l->getName() == name);}) == mLayers.end())
        {
            T* p = new T(*this);
            p->setName(name);
            mLayers.push_back(p);
            return p;
        }
    }
    return nullptr;
}

} // namespace tmx

#endif // TMX_MAP_HPP
