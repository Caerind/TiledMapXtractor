#ifndef TILESET_HPP
#define TILESET_HPP

#include <iostream>
#include <sstream>

#include <SFML/Graphics/Texture.hpp>

#include "pugixml.hpp"

#include "Properties.hpp"

class Tileset
{
    public:
        Tileset();

        bool loadFromNode(pugi::xml_node& tileset);
        bool saveToNode(pugi::xml_node& tileset);

        std::string getName() const;
        unsigned int getFirstGid() const;
        unsigned int getTileCount() const;
        sf::Vector2u getTileSize() const;

        bool isTextureLoaded() const;
        sf::Texture& getTexture();
        sf::Vector2u toPos(unsigned int id);
        unsigned int toId(sf::Vector2u const& pos);

    protected:
        unsigned int mFirstGid;
        std::string mSource;
        std::string mName;
        sf::Vector2u mTileSize;
        unsigned int mSpacing;
        unsigned int mMargin;
        unsigned int mTileCount;
        unsigned int mColumns;

        std::string mImageFormat;
        std::string mImageSource;
        std::string mImageTransparent;
        sf::Vector2u mImageSize;

        sf::Texture mTexture;
        bool mTextureLoaded;

        Properties mProperties;
};

#endif // TILESET_HPP
