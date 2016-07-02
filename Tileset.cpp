#include "Tileset.hpp"

Tileset::Tileset()
{
    mFirstGid = 1;
    mSource = "";
    mName = "";
    mTileSize = {0, 0};
    mSpacing = 0;
    mMargin = 0;
    mTileCount = 0;
    mColumns = 0;

    mImageFormat = "";
    mImageSource = "";
    mImageTransparent = "";
    mImageSize = {0, 0};

    mTextureLoaded = false;
}

bool Tileset::loadFromNode(pugi::xml_node& tileset)
{
    if (!tileset)
    {
        std::cerr << "Invalid tileset node" << std::endl;
        return false;
    }
    for (pugi::xml_attribute attr = tileset.first_attribute(); attr; attr = attr.next_attribute())
    {
        if (attr.name() == std::string("firstgid")) mFirstGid = attr.as_uint();
        if (attr.name() == std::string("source")) mSource = attr.value();
        if (attr.name() == std::string("name")) mName = attr.value();
        if (attr.name() == std::string("tilewidth")) mTileSize.x = attr.as_uint();
        if (attr.name() == std::string("tileheight")) mTileSize.y = attr.as_uint();
        if (attr.name() == std::string("spacing")) mSpacing = attr.as_uint();
        if (attr.name() == std::string("margin")) mMargin = attr.as_uint();
        if (attr.name() == std::string("tilecount")) mTileCount = attr.as_uint();
        if (attr.name() == std::string("columns")) mColumns = attr.as_uint();
    }

    pugi::xml_node image = tileset.child("image");
    if (!image)
    {
        std::cerr << "Invalid tileset node" << std::endl;
        return false;
    }
    for (pugi::xml_attribute attr = image.first_attribute(); attr; attr = attr.next_attribute())
    {
        if (attr.name() == std::string("format")) mImageFormat = attr.value();
        if (attr.name() == std::string("source")) mImageSource = attr.value();
        if (attr.name() == std::string("trans")) mImageTransparent = attr.value();
        if (attr.name() == std::string("width")) mImageSize.x = attr.as_uint();
        if (attr.name() == std::string("height")) mImageSize.y = attr.as_uint();
    }
    if (mImageSource != "")
    {
        sf::Image image;
        if (image.loadFromFile(mImageSource))
        {
            mImageSize.x = image.getSize().x;
            mImageSize.y = image.getSize().y;

            int hexTrans;
            std::stringstream ss(mImageTransparent);
            ss >> std::hex >> hexTrans;
            if (hexTrans >= 0)
            {
                unsigned char red, green, blue;
                red = hexTrans >> 16;
                green = (hexTrans >> 8) & 0xff;
                blue = hexTrans & 0xff;
                image.createMaskFromColor(sf::Color(red, green, blue));
            }
            if (mTexture.loadFromImage(image))
            {
                mTextureLoaded = true;
            }
        }
    }
    if (!mTextureLoaded)
    {
        std::cerr << "Due to incomplete data, texture hasn't been loaded" << std::endl;
    }
    pugi::xml_node properties = tileset.child("properties");
    if (properties)
        mProperties.loadFromNode(properties);
    return true;
}

bool Tileset::saveToNode(pugi::xml_node& tileset)
{
    tileset.append_attribute("firstgid") = mFirstGid;
    if (mSource != "")
        tileset.append_attribute("source") = mSource.c_str();
    tileset.append_attribute("name") = mName.c_str();
    tileset.append_attribute("tilewidth") = mTileSize.x;
    tileset.append_attribute("tileheight") = mTileSize.y;
    if (mSpacing != 0)
        tileset.append_attribute("spacing") = mSpacing;
    if (mMargin != 0)
        tileset.append_attribute("margin") = mMargin;
    tileset.append_attribute("tilecount") = mTileCount;
    tileset.append_attribute("columns") = mColumns;

    pugi::xml_node image = tileset.append_child("image");
    if (mImageFormat != "")
        image.append_attribute("format") = mImageFormat.c_str();
    image.append_attribute("source") = mImageSource.c_str();
    image.append_attribute("trans") = mImageTransparent.c_str();
    image.append_attribute("width") = mImageSize.x;
    image.append_attribute("height") = mImageSize.y;
    mProperties.saveToNode(tileset);
    return true;
}

std::string Tileset::getName() const
{
    return mName;
}

unsigned int Tileset::getFirstGid() const
{
    return mFirstGid;
}

unsigned int Tileset::getTileCount() const
{
    return mTileCount;
}

sf::Vector2u Tileset::getTileSize() const
{
    return mTileSize;
}

bool Tileset::isTextureLoaded() const
{
    return mTextureLoaded;
}

sf::Texture& Tileset::getTexture()
{
    return mTexture;
}

sf::Vector2u Tileset::toPos(unsigned int id)
{
    if (id < mFirstGid || id >= mFirstGid + mTileCount)
    {
        std::cerr << "This id (" << id << ") dont belong to this tileset" << std::endl;
        return sf::Vector2u();
    }

    id -= mFirstGid;
    sf::Vector2u pos;
    if (mColumns > 0) // Avoid div 0
    {
        pos.x = (id % mColumns) * (mTileSize.x + mSpacing) + mMargin;
        pos.y = (id / mColumns) * (mTileSize.y + mSpacing) + mMargin;
    }
    return pos;
}

unsigned int Tileset::toId(sf::Vector2u const& pos)
{
    if (mTileSize != sf::Vector2u())
    {
        return 1 + (pos.x - mMargin) / (mTileSize.x + mSpacing) + (pos.y - mMargin) / (mTileSize.y + mSpacing) * mColumns;
    }
    return 0;
}
