#include "Tileset.hpp"

namespace tmx
{

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
    mTileOffset = {0, 0};
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

    pugi::xml_node offset = tileset.child("offset");
    if (offset)
    {
        pugi::xml_attribute attr = offset.attribute("x");
        if (attr)
            mTileOffset.x = attr.as_int();
        attr = offset.attribute("y");
        if (attr)
            mTileOffset.y = attr.as_int();
    }

    pugi::xml_node image = tileset.child("image");
    mImage.loadFromNode(image);

    std::string source = mImage.getSource();
    if (source != "")
    {
        sf::Image image;
        if (image.loadFromFile(source))
        {
            sf::Color trans = mImage.getTransparent();
            if (trans != sf::Color::Transparent)
            {
                image.createMaskFromColor(trans);
            }
            if (!mTexture.loadFromImage(image))
            {
                std::cerr << "Due to incomplete data, texture hasn't been loaded" << std::endl;
                return false;
            }
        }
    }

    pugi::xml_node terrains = tileset.child("terraintypes");
    if (terrains)
    {
        for (pugi::xml_node terrain = terrains.child("terrain"); terrain; terrain = terrain.next_sibling("terrain"))
        {
            Terrain t;
            t.loadFromNode(terrain);
            mTerrains.push_back(t);
        }
    }

    for (pugi::xml_node tile = tileset.child("tile"); tile; tile = tile.next_sibling("tile"))
    {
        Tile t;
        t.loadFromNode(tile);
        mTiles.push_back(t);
    }

    loadProperties(tileset);

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

    if (mTileOffset != sf::Vector2i())
    {
        pugi::xml_node tileoffset = tileset.append_child("tileoffset");
        if (mTileOffset.x != 0)
            tileoffset.append_attribute("x") = mTileOffset.x;
        if (mTileOffset.y != 0)
            tileoffset.append_attribute("y") = mTileOffset.y;
    }

    pugi::xml_node image = tileset.append_child("image");
    mImage.saveToNode(image);

    if (mTerrains.size() > 0)
    {
        pugi::xml_node terrains = tileset.append_child("terraintypes");
        for (std::size_t i = 0; i < mTerrains.size(); i++)
        {
            pugi::xml_node terrain = terrains.append_child("terrain");
            mTerrains[i].saveToNode(terrain);
        }
    }

    for (std::size_t i = 0; i < mTiles.size(); i++)
    {
        pugi::xml_node tile = tileset.append_child("tile");
        mTiles[i].saveToNode(tile);
    }

    saveProperties(tileset);

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

sf::Vector2i Tileset::getTileOffset() const
{
    return mTileOffset;
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

Tileset::Terrain::Terrain()
{
    mName = "";
    mTile = 1; // TODO : Or 0 ?
}

void Tileset::Terrain::loadFromNode(pugi::xml_node& terrain)
{
    if (!terrain)
        std::cerr << "Incorrect terrain node" << std::endl;

    pugi::xml_attribute attr = terrain.attribute("name");
    if (!attr)
    {
        std::cerr << "Terrain node has no attribute name" << std::endl;
        return;
    }
    mName = attr.value();
    attr = terrain.attribute("tile");
    if (attr)
        mTile = attr.as_uint();
    loadProperties(terrain);
}

void Tileset::Terrain::saveToNode(pugi::xml_node& terrain)
{
    if (!terrain)
    {
        std::cerr << "Incorrect terrain node" << std::endl;
        return;
    }
    terrain.append_attribute("name") = mName.c_str();
    terrain.append_attribute("tile") = mTile;
    saveProperties(terrain);
}

std::string Tileset::Terrain::getName() const
{
    return mName;
}

unsigned int Tileset::Terrain::getTile() const
{
    return mTile;
}

Tileset::Tile::Tile()
{
    mId = 0;
    mProbability = 0.f;
}

void Tileset::Tile::loadFromNode(pugi::xml_node& tile)
{

}

void Tileset::Tile::saveToNode(pugi::xml_node& tile)
{

}

Tileset::Tile::Animation::Animation()
{
}

void Tileset::Tile::Animation::loadFromNode(pugi::xml_node& animation)
{
    if (!animation)
    {
        std::cerr << "Incorrect animation node" << std::endl;
        return;
    }
    for (pugi::xml_node frame = animation.child("frame"); frame; frame = frame.next_sibling("frame"))
    {
        Frame f;
        f.tileId = frame.attribute("tileid").as_int();
        f.duration = frame.attribute("duration").as_float();
        mFrames.push_back(f);
    }
}

void Tileset::Tile::Animation::saveToNode(pugi::xml_node& animation)
{
    if (!animation)
    {
        std::cerr << "Incorrect animation node" << std::endl;
        return;
    }
    for (std::size_t i = 0; i < mFrames.size(); i++)
    {
        pugi::xml_node frame = animation.append_child("frame");
        frame.append_attribute("tileid") = mFrames[i].tileId;
        frame.append_attribute("duration") = mFrames[i].duration;
    }
}

Tileset::Tile::Animation::Frame& Tileset::Tile::Animation::getFrame(std::size_t index)
{
    if (0 > index && index >= mFrames.size())
    {
        std::cerr << "Out of range" << std::endl;
    }
    return mFrames[index];
}

std::size_t Tileset::Tile::Animation::getFrameCount() const
{
    return mFrames.size();
}

} // namespace tmx
