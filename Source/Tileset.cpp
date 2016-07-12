#include "Tileset.hpp"
#include "Map.hpp"

namespace tmx
{

Tileset::Tileset(Map& map)
: mMap(map)
, mFirstGid(1)
, mSource("")
, mName("")
, mTileSize({0, 0})
, mSpacing(0)
, mMargin(0)
, mTileCount(0)
, mColumns(0)
, mTileOffset({0.f, 0.f})
, mImage()
, mTexture()
, mTerrains()
, mTiles()
{
}

bool Tileset::loadFromNode(pugi::xml_node const& tileset, bool fromTsx)
{
    for (const pugi::xml_attribute& attr : tileset.attributes())
    {
        if (attr.name() == std::string("firstgid"))
        {
            mFirstGid = attr.as_uint();
        }
        if (attr.name() == std::string("source"))
        {
            mSource = attr.as_string();
        }
        if (attr.name() == std::string("name"))
        {
            mName = attr.as_string();
        }
        if (attr.name() == std::string("tilewidth"))
        {
            mTileSize.x = attr.as_int();
        }
        if (attr.name() == std::string("tileheight"))
        {
            mTileSize.y = attr.as_int();
        }
        if (attr.name() == std::string("spacing"))
        {
            mSpacing = attr.as_uint();
        }
        if (attr.name() == std::string("margin"))
        {
            mMargin = attr.as_uint();
        }
        if (attr.name() == std::string("tilecount"))
        {
            mTileCount = attr.as_uint();
        }
        if (attr.name() == std::string("columns"))
        {
            mColumns = attr.as_uint();
        }
    }

    if (mSource != "" && !fromTsx)
    {
        return loadFromFile(mMap.getPath() + mSource);
    }

    pugi::xml_node offset = tileset.child("offset");
    if (offset)
    {
        mTileOffset.x = offset.attribute("x").as_float();
        mTileOffset.y = offset.attribute("y").as_float();
    }

    pugi::xml_node image = tileset.child("image");
    if (image)
    {
        mImage.loadFromNode(image);
    }

    pugi::xml_node terrains = tileset.child("terraintypes");
    if (terrains)
    {
        for (const pugi::xml_node& terrain : terrains.children("terrain"))
        {
            mTerrains.push_back(Terrain());
            mTerrains.back().loadFromNode(terrain);
        }
    }

    for (const pugi::xml_node& tile : tileset.children("tile"))
    {
        mTiles.push_back(Tile());
        mTiles.back().loadFromNode(tile);
    }

    loadProperties(tileset);

    return mImage.loadTexture(mTexture, mMap.getPath());
}

bool Tileset::loadFromFile(std::string const& filename)
{
    if (filename == "")
    {
        return false;
    }
    pugi::xml_document doc;
    if (!doc.load_file(filename.c_str()))
    {
        detail::log("Unable to load tileset from file : " + mMap.getPath() + filename);
        return false;
    }
    pugi::xml_node tileset = doc.child("tileset");
    if (!tileset)
    {
        return false;
    }
    return loadFromNode(tileset, true);
}

bool Tileset::saveToNode(pugi::xml_node& tileset, bool fromTsx)
{
    if (!fromTsx)
    {
        tileset.append_attribute("firstgid") = mFirstGid;
    }
    if (mSource != "" && !fromTsx)
    {
        tileset.append_attribute("source") = mSource.c_str();
        return saveToFile(mSource);
    }
    tileset.append_attribute("name") = mName.c_str();
    tileset.append_attribute("tilewidth") = mTileSize.x;
    tileset.append_attribute("tileheight") = mTileSize.y;
    if (mSpacing != 0)
    {
        tileset.append_attribute("spacing") = mSpacing;
    }
    if (mMargin != 0)
    {
        tileset.append_attribute("margin") = mMargin;
    }
    tileset.append_attribute("tilecount") = mTileCount;
    tileset.append_attribute("columns") = mColumns;

    if (mTileOffset != sf::Vector2f())
    {
        pugi::xml_node tileoffset = tileset.append_child("tileoffset");
        tileoffset.append_attribute("x") = mTileOffset.x;
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

bool Tileset::saveToFile(std::string const& filename)
{
    if (filename == "")
    {
        return false;
    }
    pugi::xml_document doc;
    pugi::xml_node tileset = doc.append_child("tileset");
    if (!saveToNode(tileset, true))
    {
        return false;
    }
    if (!doc.save_file(filename.c_str()))
    {
        return false;
    }
    return true;
}

unsigned int Tileset::getFirstGid() const
{
    return mFirstGid;
}

const std::string& Tileset::getSource() const
{
    return mSource;
}

const std::string& Tileset::getName() const
{
    return mName;
}

const sf::Vector2i& Tileset::getTileSize() const
{
    return mTileSize;
}

unsigned int Tileset::getSpacing() const
{
    return mSpacing;
}

unsigned int Tileset::getMargin() const
{
    return mMargin;
}

unsigned int Tileset::getTileCount() const
{
    return mTileCount;
}

unsigned int Tileset::getColumns() const
{
    return mColumns;
}

const sf::Vector2f& Tileset::getTileOffset() const
{
    return mTileOffset;
}

const std::string& Tileset::getImageData() const
{
    return mImage.getData();
}

const std::string& Tileset::getImageFormat() const
{
    return mImage.getFormat();
}

const std::string& Tileset::getImageSource() const
{
    return mImage.getSource();
}

sf::Color Tileset::getImageTransparent() const
{
    return mImage.getTransparent();
}

const sf::Vector2i& Tileset::getImageSize() const
{
    return mImage.getSize();
}

void Tileset::setFirstGid(unsigned int id)
{
    mFirstGid = id;
}

void Tileset::setSource(std::string const& source)
{
    mSource = source;
}

void Tileset::setName(std::string const& name)
{
    mName = name;
}

void Tileset::setTileSize(sf::Vector2i const& tileSize)
{
    mTileSize = tileSize;
}

void Tileset::setSpacing(unsigned int spacing)
{
    mSpacing = spacing;
}

void Tileset::setMargin(unsigned int margin)
{
    mMargin = margin;
}

void Tileset::setTileCount(unsigned int tileCount)
{
    mTileCount = tileCount;
}

void Tileset::setColumns(unsigned int columns)
{
    mColumns = columns;
}

void Tileset::setOffset(sf::Vector2f const& offset)
{
    mTileOffset = offset;
}

void Tileset::setImageData(std::string const& data)
{
    mImage.setData(data);
}

void Tileset::setImageFormat(std::string const& format)
{
    mImage.setFormat(format);
}

void Tileset::setImageSource(std::string const& source)
{
    mImage.setSource(source);
}

void Tileset::setImageTransparent(sf::Color const& color)
{
    mImage.setTransparent(color);
}

void Tileset::setImageSize(sf::Vector2i const& size)
{
    mImage.setSize(size);
}

bool Tileset::loadTexture()
{
    return mImage.loadTexture(mTexture, mMap.getPath());
}

sf::Texture& Tileset::getTexture()
{
    return mTexture;
}

sf::Vector2i Tileset::toPos(unsigned int gid)
{
    if (gid < mFirstGid || gid >= mFirstGid + mTileCount)
    {
        return sf::Vector2i();
    }

    gid -= mFirstGid; // Local id
    sf::Vector2i pos;
    if (mColumns > 0) // Avoid div 0
    {
        pos.x = (gid % mColumns) * (mTileSize.x + mSpacing) + mMargin;
        pos.y = (gid / mColumns) * (mTileSize.y + mSpacing) + mMargin;
    }
    return pos;
}

sf::IntRect Tileset::toRect(unsigned int gid)
{
    if (gid < mFirstGid || gid >= mFirstGid + mTileCount)
    {
        return sf::IntRect();
    }

    gid -= mFirstGid;
    sf::IntRect rect;
    if (mColumns > 0) // Avoid div 0
    {
        rect.left = (gid % mColumns) * (mTileSize.x + mSpacing) + mMargin;
        rect.top = (gid / mColumns) * (mTileSize.y + mSpacing) + mMargin;
        rect.width = mTileSize.x;
        rect.height = mTileSize.y;
    }
    return rect;
}

unsigned int Tileset::toId(sf::Vector2i const& pos)
{
    if (mTileSize != sf::Vector2i())
    {
        return 1 + (pos.x - mMargin) / (mTileSize.x + mSpacing) + (pos.y - mMargin) / (mTileSize.y + mSpacing) * mColumns;
    }
    return 0;
}

Tileset::Terrain::Terrain()
: mName("")
, mTile(0)
{
}

void Tileset::Terrain::loadFromNode(pugi::xml_node const& terrain)
{
    mName = terrain.attribute("name").as_string();
    mTile = terrain.attribute("tile").as_uint();
    loadProperties(terrain);
}

void Tileset::Terrain::saveToNode(pugi::xml_node& terrain)
{
    terrain.append_attribute("name") = mName.c_str();
    terrain.append_attribute("tile") = mTile;
    saveProperties(terrain);
}

const std::string& Tileset::Terrain::getName() const
{
    return mName;
}

unsigned int Tileset::Terrain::getTile() const
{
    return mTile;
}

void Tileset::Terrain::setName(std::string const& name)
{
    mName = name;
}

void Tileset::Terrain::setTile(unsigned int tile)
{
    mTile = tile;
}

Tileset::Tile::Tile()
: mId(0)
, mTerrains()
, mProbability(1.f)
, mAnimations()
{
}

void Tileset::Tile::loadFromNode(pugi::xml_node const& tile)
{
    mId = tile.attribute("id").as_uint();

    const pugi::xml_attribute& terrain = tile.attribute("terrain");
    if (terrain)
    {
        std::stringstream ss(terrain.as_string());
        std::string temp;
        std::size_t i = 0;
        while (std::getline(ss, temp, ','))
        {
            if (i < 4)
            {
                mTerrains[i] = temp;
            }
            i++;
        }
    }

    mProbability = tile.attribute("probability").as_float();

    for (const pugi::xml_node& animation : tile.children("animation"))
    {
        mAnimations.push_back(Animation());
        mAnimations.back().loadFromNode(animation);
    }

    loadProperties(tile);
}

void Tileset::Tile::saveToNode(pugi::xml_node& tile)
{
    tile.append_attribute("id") = mId;

    std::string t;
    for (std::size_t i = 0; i < mTerrains.size(); i++)
    {
        t += mTerrains[i] + ",";
    }
    t.pop_back();
    if (t != ",,,")
    {
        tile.append_attribute("terrain") = t.c_str();
    }

    if (mProbability != 1.f)
    {
        tile.append_attribute("probability") = mProbability;
    }

    for (std::size_t i = 0; i < mAnimations.size(); i++)
    {
        pugi::xml_node animation = tile.append_child("animation");
        mAnimations[i].saveToNode(animation);
    }
}

unsigned int Tileset::Tile::getId() const
{
    return mId;
}

const std::string& Tileset::Tile::getTerrain(std::size_t index) const
{
    return mTerrains.at(index);
}

float Tileset::Tile::getProbability() const
{
    return mProbability;
}

void Tileset::Tile::setId(unsigned int id)
{
    mId = id;
}

void Tileset::Tile::setTerrain(std::size_t index, std::string const& terrain)
{
    mTerrains[index] = terrain;
}

void Tileset::Tile::setProbability(float probability)
{
    mProbability = probability;
}

Tileset::Tile::Animation::Animation()
: mFrames()
{
}

void Tileset::Tile::Animation::loadFromNode(pugi::xml_node const& animation)
{
    for (const pugi::xml_node& frame : animation.children("frame"))
    {
        Frame f;
        f.tileId = frame.attribute("tileid").as_int();
        f.duration = frame.attribute("duration").as_float();
        mFrames.push_back(f);
    }
}

void Tileset::Tile::Animation::saveToNode(pugi::xml_node& animation)
{
    for (std::size_t i = 0; i < mFrames.size(); i++)
    {
        pugi::xml_node frame = animation.append_child("frame");
        frame.append_attribute("tileid") = mFrames[i].tileId;
        frame.append_attribute("duration") = mFrames[i].duration;
    }
}

void Tileset::Tile::Animation::addFrame(unsigned int tileId, float duration)
{
    Frame frame;
    frame.tileId = tileId;
    frame.duration = duration;
    mFrames.push_back(frame);
}

Tileset::Tile::Animation::Frame& Tileset::Tile::Animation::getFrame(std::size_t index)
{
    return mFrames[index];
}

std::size_t Tileset::Tile::Animation::frames() const
{
    return mFrames.size();
}

void Tileset::Tile::Animation::removeFrame(std::size_t index)
{
    mFrames.erase(mFrames.begin() + index);
}

void Tileset::Tile::addAnimation()
{
    mAnimations.push_back(Animation());
}

Tileset::Tile::Animation& Tileset::Tile::getAnimation(std::size_t index)
{
    return mAnimations.at(index);
}

std::size_t Tileset::Tile::animations() const
{
    return mAnimations.size();
}

void Tileset::Tile::removeAnimation(std::size_t index)
{
    mAnimations.erase(mAnimations.begin() + index);
}

void Tileset::addTerrain(std::string const& name, unsigned int tileId)
{
    mTerrains.push_back(Terrain());
    mTerrains.back().setName(name);
    mTerrains.back().setTile(tileId);
}

Tileset::Terrain& Tileset::getTerrain(std::size_t index)
{
    return mTerrains[index];
}

std::size_t Tileset::terrains() const
{
    return mTerrains.size();
}

void Tileset::removeTerrain(std::size_t index)
{
    mTerrains.erase(mTerrains.begin() + index);
}

void Tileset::addTile(unsigned int tileId)
{
    mTiles.push_back(Tile());
    mTiles.back().setId(tileId);
}

Tileset::Tile& Tileset::getTile(std::size_t index)
{
    return mTiles[index];
}

std::size_t Tileset::tiles() const
{
    return mTiles.size();
}

void Tileset::removeTile(std::size_t index)
{
    mTiles.erase(mTiles.begin() + index);
}

} // namespace tmx
