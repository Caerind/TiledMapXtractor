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
        ~Map();

        void clear();

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

        sf::Vector2i worldToCoords(sf::Vector2f const& world);

        void renderBackground(sf::RenderTarget& target);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void render(std::size_t index, sf::RenderTarget& target, sf::RenderStates states) const;

        std::size_t getTilesetCount() const;
        Tileset* getTileset(unsigned int gid);
        Tileset* getTileset(std::string const& name);
        Tileset* createTileset(std::string const& name);
        void removeTileset(std::string const& name);

        const std::string& getOrientation() const;
        const std::string& getRenderOrder() const;
        const sf::Vector2i& getMapSize() const;
        const sf::Vector2i& getTileSize() const;
        unsigned int getHexSideLength() const;
        const std::string& getStaggerAxis() const;
        const std::string& getStaggerIndex() const;
        const std::string& getBackgroundColor() const;
        unsigned int getNextObjectId() const;

        void setOrientation(std::string const& orientation);
        void setRenderOrder(std::string const& renderOrder);
        void setMapSize(sf::Vector2i const& mapSize);
        void setTileSize(sf::Vector2i const& tileSize);
        void setHexSideLength(unsigned int hexSide);
        void setStaggerAxis(std::string const& axis);
        void setStaggerIndex(std::string const& index);
        void setBackgroundColor(std::string const& color);
        void setNextObjectId(unsigned int nextObject);

        const std::string& getPath() const;
        void setPath(std::string const& path);

        bool getRenderObjects() const;
        void setRenderObjects(bool renderObjects);

        const sf::Vector2f& getMapOffset() const;
        void setMapOffset(sf::Vector2f const& offset);

    private:
        float mVersion;
        std::string mOrientation;
        std::string mRenderOrder;
        sf::Vector2i mMapSize;
        sf::Vector2i mTileSize;
        unsigned int mHexSideLength;
        std::string mStaggerAxis;
        std::string mStaggerIndex;
        std::string mBackgroundColor;
        unsigned int mNextObjectId;

        std::string mPath;
        bool mRenderObjects;
        sf::Vector2f mMapOffset;

        std::vector<Tileset*> mTilesets;
        std::vector<LayerBase*> mLayers;
};

template <typename T>
T* Map::getLayer(std::size_t index)
{
    return static_cast<T*>(mLayers[index]);
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
