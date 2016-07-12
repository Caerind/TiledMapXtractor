#ifndef TMX_TILESET_HPP
#define TMX_TILESET_HPP

#include "Utils.hpp"

namespace tmx
{

class Map;
class Tileset : public detail::PropertiesHolder
{
    public:
        Tileset(Map& map);

        bool loadFromNode(pugi::xml_node const& tileset, bool fromTsx = false);
        bool loadFromFile(std::string const& filename);
        bool saveToNode(pugi::xml_node& tileset, bool fromTsx = false);
        bool saveToFile(std::string const& filename);

        unsigned int getFirstGid() const;
        const std::string& getSource() const;
        const std::string& getName() const;
        const sf::Vector2i& getTileSize() const;
        unsigned int getSpacing() const;
        unsigned int getMargin() const;
        unsigned int getTileCount() const;
        unsigned int getColumns() const;
        const sf::Vector2f& getTileOffset() const;
        const std::string& getImageData() const;
        const std::string& getImageFormat() const;
        const std::string& getImageSource() const;
        sf::Color getImageTransparent() const;
        const sf::Vector2i& getImageSize() const;

        void setFirstGid(unsigned int id);
        void setSource(std::string const& source);
        void setName(std::string const& name);
        void setTileSize(sf::Vector2i const& tileSize);
        void setSpacing(unsigned int spacing);
        void setMargin(unsigned int margin);
        void setTileCount(unsigned int tileCount);
        void setColumns(unsigned int columns);
        void setOffset(sf::Vector2f const& offset);
        void setImageData(std::string const& data);
        void setImageFormat(std::string const& format);
        void setImageSource(std::string const& source);
        void setImageTransparent(sf::Color const& color);
        void setImageSize(sf::Vector2i const& size);

        bool loadTexture();

        sf::Texture& getTexture();
        sf::Vector2i toPos(unsigned int gid);
        sf::IntRect toRect(unsigned int gid);
        unsigned int toId(sf::Vector2i const& pos);

        class Terrain : public PropertiesHolder
        {
            public:
                Terrain();

                void loadFromNode(pugi::xml_node const& terrain);
                void saveToNode(pugi::xml_node& terrain);

                const std::string& getName() const;
                unsigned int getTile() const;

                void setName(std::string const& name);
                void setTile(unsigned int tile);

            protected:
                std::string mName;
                unsigned int mTile;
        };

        // TODO : T - Tile::ObjectGroup
        // TODO : T - Tile::Image
        class Tile : public PropertiesHolder
        {
            public:
                Tile();

                void loadFromNode(pugi::xml_node const& tile);
                void saveToNode(pugi::xml_node& tile);

                unsigned int getId() const;
                const std::string& getTerrain(std::size_t index) const;
                float getProbability() const;

                void setId(unsigned int id);
                void setTerrain(std::size_t index, std::string const& terrain);
                void setProbability(float probability);

                class Animation
                {
                    public:
                        Animation();

                        void loadFromNode(pugi::xml_node const& animation);
                        void saveToNode(pugi::xml_node& animation);

                        struct Frame
                        {
                            unsigned int tileId;
                            float duration;
                        };

                        void addFrame(unsigned int tileId, float duration);
                        Frame& getFrame(std::size_t index);
                        std::size_t frames() const;
                        void removeFrame(std::size_t index);

                    private:
                        std::vector<Frame> mFrames;
                };

                void addAnimation();
                Animation& getAnimation(std::size_t index);
                std::size_t animations() const;
                void removeAnimation(std::size_t index);

            private:
                unsigned int mId;
                std::array<std::string,4> mTerrains;
                float mProbability;
                std::vector<Animation> mAnimations;
        };

        void addTerrain(std::string const& name, unsigned int tileId);
        Terrain& getTerrain(std::size_t index);
        std::size_t terrains() const;
        void removeTerrain(std::size_t index);

        void addTile(unsigned int tileId);
        Tile& getTile(std::size_t index);
        std::size_t tiles() const;
        void removeTile(std::size_t index);

    protected:
        Map& mMap;

        unsigned int mFirstGid;
        std::string mSource;
        std::string mName;
        sf::Vector2i mTileSize;
        unsigned int mSpacing;
        unsigned int mMargin;
        unsigned int mTileCount;
        unsigned int mColumns;
        sf::Vector2f mTileOffset;

        detail::Image mImage;
        sf::Texture mTexture;

        std::vector<Terrain> mTerrains;
        std::vector<Tile> mTiles;
};

} // namespace tmx

#endif // TMX_TILESET_HPP
