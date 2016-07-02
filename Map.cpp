#include "Map.hpp"

Map::Map()
{
    mVersion = 1.0f;
    mOrientation = "orthogonal";
    mRenderOrder = "right-down";
    mMapSize = {0, 0};
    mTileSize = {0, 0};
    mHexSideLength = 0;
    mStaggerAxis = "";
    mStaggerIndex = "";
    mBackgroundColor = "";
    mNextObjectId = 1;
}

bool Map::loadFromFile(std::string const& filename)
{
    if (filename == "")
    {
        std::cerr << "Uncorrect filename" << std::endl;
        return false;
    }
    pugi::xml_document doc;
    if (!doc.load_file(filename.c_str()))
    {
        std::cerr << "The document (" << filename << ") cannot be loaded" << std::endl;
        return false;
    }
    pugi::xml_node map = doc.child("map");
    if (!map)
    {
        std::cerr << "The document has no \"map\" node" << std::endl;
        return false;
    }

    for (pugi::xml_attribute attr = map.first_attribute(); attr; attr = attr.next_attribute())
    {
        if (attr.name() == std::string("version")) mVersion = attr.as_float();
        if (attr.name() == std::string("orientation")) mOrientation = attr.value();
        if (attr.name() == std::string("renderorder")) mRenderOrder = attr.value();
        if (attr.name() == std::string("width")) mMapSize.x = attr.as_uint();
        if (attr.name() == std::string("height")) mMapSize.y = attr.as_uint();
        if (attr.name() == std::string("tilewidth")) mTileSize.x = attr.as_uint();
        if (attr.name() == std::string("tileheight")) mTileSize.y = attr.as_uint();
        if (attr.name() == std::string("hexsidelength")) mHexSideLength = attr.as_uint();
        if (attr.name() == std::string("staggeraxis")) mStaggerAxis = attr.value();
        if (attr.name() == std::string("staggerindex")) mStaggerIndex = attr.value();
        if (attr.name() == std::string("backgroundcolor")) mBackgroundColor = attr.value();
        if (attr.name() == std::string("nextobjectid")) mNextObjectId = attr.as_uint();
    }

    pugi::xml_node properties = map.child("properties");
    if (properties)
        mProperties.loadFromNode(properties);

    for (pugi::xml_node tileset = map.child("tileset"); tileset; tileset = tileset.next_sibling("tileset"))
    {
        loadTileset(tileset);
    }

    for (pugi::xml_node layer = map.child("layer"); layer; layer = layer.next_sibling("layer"))
    {
        loadLayer(layer);
    }

    // TODO : Handle the rest of TMX files

    return true;
}

bool Map::saveToFile(std::string const& filename)
{
    if (filename == "")
    {
        std::cerr << "Uncorrect filename" << std::endl;
        return false;
    }
    pugi::xml_document doc;
    pugi::xml_node map = doc.append_child("map");
    map.append_attribute("version") = mVersion;
    map.append_attribute("orientation") = mOrientation.c_str();
    map.append_attribute("renderorder") = mRenderOrder.c_str();
    map.append_attribute("width") = mMapSize.x;
    map.append_attribute("height") = mMapSize.y;
    map.append_attribute("tilewidth") = mTileSize.x;
    map.append_attribute("tileheight") = mTileSize.y;
    if (mHexSideLength != 0)
        map.append_attribute("hexsidelength") = mHexSideLength;
    if (mOrientation == "staggered" || mOrientation == "hexagonal")
    {
        map.append_attribute("staggeraxis") = mStaggerAxis.c_str();
        map.append_attribute("staggerindex") = mStaggerIndex.c_str();
    }
    if (mBackgroundColor != "")
        map.append_attribute("backgroundcolor") = mBackgroundColor.c_str();
    map.append_attribute("nextobjectid") = mNextObjectId;

    mProperties.saveToNode(map);
    doc.save_file(filename.c_str());

    for (std::size_t i = 0; i < mTilesets.size(); i++)
    {
        pugi::xml_node tileset = map.append_child("tileset");
        if (!mTilesets[i].saveToNode(tileset))
            std::cerr << "Tileset " << mTilesets[i].getName() << " hasn't been save correctly" << std::endl;
    }

    for (std::size_t i = 0; i < mLayers.size(); i++)
    {
        pugi::xml_node layer = map.append_child("layer");
        if (!mLayers[i].saveToNode(layer))
            std::cerr << "Layer " << mLayers[i].getName() << " hasn't been save correctly" << std::endl;
    }
    return true;
}

void Map::loadTileset(pugi::xml_node& tilesetNode)
{
    Tileset tileset;
    if (tileset.loadFromNode(tilesetNode))
    {
        bool nameFound = false;
        std::size_t i;
        for (i = 0; i < mTilesets.size(); i++)
        {
            if (mTilesets[i].getName() == tileset.getName())
            {
                nameFound = true;
                break;
            }
        }
        if (nameFound)
        {
            if (mTilesets[i].isTextureLoaded())
            {
                std::cerr << "Tileset already loaded" << std::endl;
            }
            else
            {
                mTilesets.erase(mTilesets.begin() + i);
                mTilesets.push_back(tileset);
            }
        }
        else
        {
            mTilesets.push_back(tileset);
        }
    }
    else
    {
        std::cerr << "A tileset haven't been loaded" << std::endl;
    }
}

void Map::loadLayer(pugi::xml_node& layerNode)
{
    Layer layer(this);
    if (layer.loadFromNode(layerNode))
    {
        bool nameFound = false;
        std::size_t i;
        for (i = 0; i < mLayers.size(); i++)
        {
            if (mLayers[i].getName() == layer.getName())
            {
                nameFound = true;
                break;
            }
        }
        if (nameFound)
        {
            std::cerr << "Layer already loaded" << std::endl;
        }
        else
        {
            mLayers.push_back(layer);
        }
    }
    else
    {
        std::cerr << "A layer haven't been loaded" << std::endl;
    }
}

std::size_t Map::getLayerCount() const
{
    return mLayers.size();
}

Layer& Map::getLayer(std::size_t index)
{
    if (index < 0 || index >= mLayers.size())
    {
        std::cerr << "Out of range : " << index << " / " << mLayers.size() << std::endl;
    }
    return mLayers[index];
}

void Map::render(sf::RenderTarget& target, sf::RenderStates states)
{
    for (std::size_t i = 0; i < mLayers.size(); i++)
    {
        mLayers[i].render(target, states);
    }
}

void Map::render(std::size_t index, sf::RenderTarget& target, sf::RenderStates states)
{
    if (0 <= index && index < mLayers.size())
    {
        mLayers[index].render(target, states);
    }
    else
    {
        std::cerr << "Out of range" << std::endl;
    }
}

Tileset* Map::getTileset(unsigned int id)
{
    for (std::size_t i = 0; i < mTilesets.size(); i++)
    {
        unsigned int first = mTilesets[i].getFirstGid();
        if (first <= id && id < first + mTilesets[i].getTileCount())
        {
            return &mTilesets[i];
        }
    }
    return nullptr;
}

std::string Map::getOrientation() const
{
    return mOrientation;
}

sf::Vector2u Map::getTileSize() const
{
    return mTileSize;
}
