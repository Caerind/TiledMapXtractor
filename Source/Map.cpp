#include "Map.hpp"
#include "Layer.hpp"
#include "ObjectGroup.hpp"

namespace tmx
{

Map::Map()
{
    clear();
}

Map::~Map()
{
    clear();
}

void Map::clear()
{
    mVersion = 1.0f;
    mOrientation = "orthogonal";
    mRenderOrder = "right-down";
    mMapSize = sf::Vector2i({0, 0});
    mTileSize= sf::Vector2i({0, 0});
    mHexSideLength = 0;
    mStaggerAxis = "";
    mStaggerIndex = "";
    mBackgroundColor = "#808080";
    mNextObjectId = 1;
    mPath = "";
    mRenderObjects = false;
    for (std::size_t i = 0; i < mTilesets.size(); i++)
    {
        delete mTilesets[i];
    }
    mTilesets.clear();
    for (std::size_t i = 0; i < mLayers.size(); i++)
    {
        delete mLayers[i];
    }
    mLayers.clear();
}

bool Map::loadFromFile(std::string const& filename)
{
    clear();

    if (filename == "")
    {
        return false;
    }

    pugi::xml_document doc;
    if (!doc.load_file(filename.c_str()))
    {
        detail::log("Unable to load map from file : " + filename);
        return false;
    }
    else
    {
        std::size_t found = filename.find_last_of("/");
        if (found != std::string::npos)
        {
            mPath = filename.substr(0, found+1);
        }
        else
        {
            found = filename.find_last_of("\\");
            if (found != std::string::npos)
            {
                mPath = filename.substr(0, found+1);
            }
            else
            {
                mPath = "";
            }
        }
    }

    pugi::xml_node map = doc.child("map");
    if (!map)
    {
        return false;
    }

    for (pugi::xml_attribute attr = map.first_attribute(); attr; attr = attr.next_attribute())
    {
        if (attr.name() == std::string("version")) mVersion = attr.as_float();
        if (attr.name() == std::string("orientation")) mOrientation = attr.as_string();
        if (attr.name() == std::string("renderorder")) mRenderOrder = attr.as_string();
        if (attr.name() == std::string("width")) mMapSize.x = attr.as_int();
        if (attr.name() == std::string("height")) mMapSize.y = attr.as_int();
        if (attr.name() == std::string("tilewidth")) mTileSize.x = attr.as_int();
        if (attr.name() == std::string("tileheight")) mTileSize.y = attr.as_int();
        if (attr.name() == std::string("hexsidelength")) mHexSideLength = attr.as_uint();
        if (attr.name() == std::string("staggeraxis")) mStaggerAxis = attr.as_string();
        if (attr.name() == std::string("staggerindex")) mStaggerIndex = attr.as_string();
        if (attr.name() == std::string("backgroundcolor")) mBackgroundColor = attr.as_string();
        if (attr.name() == std::string("nextobjectid")) mNextObjectId = attr.as_uint();
    }

    loadProperties(map);

    for (pugi::xml_node tileset = map.child("tileset"); tileset; tileset = tileset.next_sibling("tileset"))
    {
        Tileset* tset = new Tileset(*this);
        if (tset->loadFromNode(tileset))
        {
            if (std::find_if(mTilesets.begin(),mTilesets.end(),[&tset](Tileset* t)->bool{return (t->getName() == tset->getName());}) == mTilesets.end())
            {
                mTilesets.push_back(tset);
            }
        }
    }
    for (pugi::xml_node layer = map.child("layer"); layer; layer = layer.next_sibling("layer"))
    {
        Layer* lyr = new Layer(*this);
        if (lyr->loadFromNode(layer))
        {
            if (std::find_if(mLayers.begin(),mLayers.end(),[&lyr](LayerBase* l)->bool{return (l->getName() == lyr->getName());}) == mLayers.end())
                mLayers.push_back(lyr);
        }
    }
    for (pugi::xml_node objectgroup = map.child("objectgroup"); objectgroup; objectgroup = objectgroup.next_sibling("objectgroup"))
    {
        ObjectGroup* obj = new ObjectGroup(*this);
        if (obj->loadFromNode(objectgroup))
        {
            if (std::find_if(mLayers.begin(),mLayers.end(),[&obj](LayerBase* l)->bool{return (l->getName() == obj->getName());}) == mLayers.end())
                mLayers.push_back(obj);
        }
    }
    for (pugi::xml_node imagelayer = map.child("imagelayer"); imagelayer; imagelayer = imagelayer.next_sibling("imagelayer"))
    {
        ImageLayer* lyr = new ImageLayer(*this);
        if (lyr->loadFromNode(imagelayer))
        {
            if (std::find_if(mLayers.begin(),mLayers.end(),[&lyr](LayerBase* l)->bool{return (l->getName() == lyr->getName());}) == mLayers.end())
                mLayers.push_back(lyr);
        }
    }

    return true;
}

bool Map::saveToFile(std::string const& filename)
{
    if (filename == "")
    {
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
    if (mBackgroundColor != "#808080")
        map.append_attribute("backgroundcolor") = mBackgroundColor.c_str();
    map.append_attribute("nextobjectid") = mNextObjectId;

    saveProperties(map);

    for (std::size_t i = 0; i < mTilesets.size(); i++)
    {
        pugi::xml_node tileset = map.append_child("tileset");
        if (!mTilesets[i]->saveToNode(tileset))
            return false;
    }

    for (std::size_t i = 0; i < mLayers.size(); i++)
    {
        pugi::xml_node layer;
        LayerType type = mLayers[i]->getLayerType();
        switch (type)
        {
            case tmx::EImageLayer: layer = map.append_child("imagelayer"); break;
            case tmx::EObjectGroup: layer = map.append_child("objectgroup"); break;
            default: layer = map.append_child("layer"); break;
        }
        mLayers[i]->saveToNode(layer);
    }

    doc.save_file(filename.c_str()," ");
    return true;
}

std::size_t Map::getLayerCount() const
{
    return mLayers.size();
}

LayerBase* Map::getLayer(std::size_t index)
{
    return mLayers[index];
}

LayerType Map::getLayerType(std::size_t index)
{
    return mLayers[index]->getLayerType();
}

void Map::removeLayer(std::string const& name)
{
    std::size_t size = mLayers.size();
    for (std::size_t i = 0; i < size;)
    {
        if (mLayers[i]->getName() == name)
        {
            delete mLayers[i];
            mLayers.erase(mLayers.begin() + i);
            size--;
        }
        else
        {
            i++;
        }
    }
}

sf::Vector2i Map::worldToCoords(sf::Vector2f const& world)
{
    return tmx::worldToCoords(mOrientation, world, mTileSize, mStaggerAxis, mStaggerIndex, mHexSideLength);
}

void Map::renderBackground(sf::RenderTarget& target)
{
    sf::View v = target.getView();
    sf::RectangleShape shape(v.getSize());
    shape.setFillColor(detail::fromString<sf::Color>(mBackgroundColor));
    target.setView(target.getDefaultView());
    target.draw(shape);
    target.setView(v);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (std::size_t i = 0; i < mLayers.size(); i++)
    {
        if (mLayers.at(i)->getLayerType() == EObjectGroup)
        {
            if (mRenderObjects)
            {
                target.draw(*mLayers.at(i), states);
            }
        }
        else
        {
            target.draw(*mLayers.at(i), states);
        }
    }
}

void Map::render(std::size_t index, sf::RenderTarget& target, sf::RenderStates states) const
{
    if (0 <= index && index < mLayers.size())
    {
        target.draw(*mLayers.at(index), states);
    }
}

std::size_t Map::getTilesetCount() const
{
    return mTilesets.size();
}

Tileset* Map::getTileset(unsigned int id)
{
    for (std::size_t i = 0; i < mTilesets.size(); i++)
    {
        unsigned int first = mTilesets[i]->getFirstGid();
        if (first <= id && id < first + mTilesets[i]->getTileCount())
        {
            return mTilesets[i];
        }
    }
    return nullptr;
}

Tileset* Map::getTileset(std::string const& name)
{
    for (std::size_t i = 0; i < mTilesets.size(); i++)
    {
        if (mTilesets[i]->getName() == name)
        {
            return mTilesets[i];
        }
    }
    return nullptr;
}

Tileset* Map::createTileset(std::string const& name)
{
    if (name != "")
    {
        if (std::find_if(mTilesets.begin(),mTilesets.end(),[&name](Tileset* t)->bool{return (t->getName() == name);}) == mTilesets.end())
        {
            Tileset* t = new Tileset(*this);
            t->setName(name);
            mTilesets.push_back(t);
            return t;
        }
    }
    return nullptr;
}

void Map::removeTileset(std::string const& name)
{
    std::size_t size = mTilesets.size();
    for (std::size_t i = 0; i < size;)
    {
        if (mTilesets[i]->getName() == name)
        {
            delete mTilesets[i];
            mTilesets.erase(mTilesets.begin() + i);
            size--;
        }
        else
        {
            i++;
        }
    }
}

const std::string& Map::getOrientation() const
{
    return mOrientation;
}

const std::string& Map::getRenderOrder() const
{
    return mRenderOrder;
}

const sf::Vector2i& Map::getMapSize() const
{
    return mMapSize;
}

const sf::Vector2i& Map::getTileSize() const
{
    return mTileSize;
}

unsigned int Map::getHexSideLength() const
{
    return mHexSideLength;
}

const std::string& Map::getStaggerAxis() const
{
    return mStaggerAxis;
}

const std::string& Map::getStaggerIndex() const
{
    return mStaggerIndex;
}

const std::string& Map::getBackgroundColor() const
{
    return mBackgroundColor;
}

unsigned int Map::getNextObjectId() const
{
    return mNextObjectId;
}

void Map::setOrientation(std::string const& orientation)
{
    mOrientation = orientation;
}

void Map::setRenderOrder(std::string const& renderOrder)
{
    mRenderOrder = renderOrder;
}

void Map::setMapSize(sf::Vector2i const& mapSize)
{
    mMapSize = mapSize;
}

void Map::setTileSize(sf::Vector2i const& tileSize)
{
    mTileSize = tileSize;
}

void Map::setHexSideLength(unsigned int hexSide)
{
    mHexSideLength = hexSide;
}

void Map::setStaggerAxis(std::string const& axis)
{
    mStaggerAxis = axis;
}

void Map::setStaggerIndex(std::string const& index)
{
    mStaggerIndex = index;
}

void Map::setBackgroundColor(std::string const& color)
{
    mBackgroundColor = color;
}

void Map::setNextObjectId(unsigned int nextObject)
{
    mNextObjectId = nextObject;
}

const std::string& Map::getPath() const
{
    return mPath;
}

void Map::setPath(std::string const& path)
{
    mPath = path;
}

bool Map::getRenderObjects() const
{
    return mRenderObjects;
}

void Map::setRenderObjects(bool renderObjects)
{
    mRenderObjects = renderObjects;
}

const sf::Vector2f& Map::getMapOffset() const
{
    return mMapOffset;
}

void Map::setMapOffset(sf::Vector2f const& offset)
{
    mMapOffset = offset;
}

} // namespace tmx
