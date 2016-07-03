#ifndef TMX_TILESET_HPP
#define TMX_TILESET_HPP

#include "Utils.hpp"

namespace tmx
{

class Tileset : public detail::PropertiesHolder
{
    public:
        Tileset();

        bool loadFromNode(pugi::xml_node& tileset);
        bool saveToNode(pugi::xml_node& tileset);

        std::string getName() const;
        unsigned int getFirstGid() const;
        unsigned int getTileCount() const;
        sf::Vector2u getTileSize() const;
        sf::Vector2i getTileOffset() const;

        sf::Texture& getTexture();
        sf::Vector2u toPos(unsigned int id);
        unsigned int toId(sf::Vector2u const& pos);

        class Terrain : public PropertiesHolder
        {
            public:
                Terrain();

                void loadFromNode(pugi::xml_node& terrain);
                void saveToNode(pugi::xml_node& terrain);

                std::string getName() const;
                unsigned int getTile() const;

            protected:
                std::string mName;
                unsigned int mTile;
        };

        class Tile : public PropertiesHolder
        {
            public:
                Tile();

                void loadFromNode(pugi::xml_node& tile);
                void saveToNode(pugi::xml_node& tile);

                class Animation
                {
                    public:
                        Animation();

                        void loadFromNode(pugi::xml_node& animation);
                        void saveToNode(pugi::xml_node& animation);

                        struct Frame
                        {
                            unsigned int tileId;
                            float duration; // TODO : Is float ?
                        };

                        Frame& getFrame(std::size_t index);
                        std::size_t getFrameCount() const;

                    private:
                        std::vector<Frame> mFrames;
                };

            private:
                unsigned int mId;
                std::array<std::string,4> mTerrains;
                float mProbability;

                std::vector<Animation> mAnimations;
        };

    protected:
        unsigned int mFirstGid;
        std::string mSource;
        std::string mName;
        sf::Vector2u mTileSize;
        unsigned int mSpacing;
        unsigned int mMargin;
        unsigned int mTileCount;
        unsigned int mColumns;
        sf::Vector2i mTileOffset;

        detail::Image mImage;
        sf::Texture mTexture;

        std::vector<Terrain> mTerrains;
        std::vector<Tile> mTiles;
};

} // namespace tmx

#endif // TMX_TILESET_HPP
