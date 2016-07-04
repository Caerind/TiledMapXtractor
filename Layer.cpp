#include "Layer.hpp"
#include "Map.hpp"
#include "Tileset.hpp"

namespace tmx
{

Layer::Layer(Map& map)
: mMap(map)
, mTileset(nullptr)
, mVertices(sf::Triangles)
, mEncoding("")
, mCompression("")
{
}

LayerType Layer::getLayerType() const
{
    return tmx::ELayer;
}

bool Layer::loadFromNode(pugi::xml_node const& layer)
{
    if (!layer)
    {
        return false;
    }
    if (!LayerBase::loadFromNode(layer))
    {
        return false;
    }
    pugi::xml_node data = layer.child("data");
    if (!data)
    {
        return false;
    }
    for (pugi::xml_attribute attr = data.first_attribute(); attr; attr = attr.next_attribute())
    {
        if (attr.name() == std::string("encoding"))
        {
            mEncoding = attr.as_string();
        }
        if (attr.name() == std::string("compression"))
        {
            mCompression = attr.as_string();
        }
    }
    update();
    if (!loadFromCode(data.text().get()))
    {
        return false;
    }
    return true;
}

void Layer::saveToNode(pugi::xml_node& layer)
{
    if (!layer)
    {
        return;
    }
    LayerBase::saveToNode(layer);
    layer.append_attribute("width") = mMap.getMapSize().x;
    layer.append_attribute("height") = mMap.getMapSize().y;
    pugi::xml_node data = layer.append_child("data");
    data.append_attribute("encoding") = mEncoding.c_str();
    data.append_attribute("compression") = mCompression.c_str();
    data.text().set(getCode().c_str());
}

void Layer::setTileId(sf::Vector2u coords, unsigned int id)
{
    if (0 <= coords.x && coords.x < mMap.getMapSize().x && 0 <= coords.y && coords.y < mMap.getMapSize().y)
    {
        sf::Vertex* tri = getVertex(coords);
        if (id == 0 && tri != nullptr)
        {
            for (std::size_t i = 0; i < 6; i++)
            {
                tri[i].texCoords = sf::Vector2f(0.f, 0.f);
                tri[i].color = sf::Color::Transparent;
            }
        }
        else if (tri != nullptr)
        {
            if (mTileset == nullptr)
            {
                mTileset = mMap.getTileset(id);
                update();
            }
            if (mTileset != nullptr)
            {
                sf::Vector2u pos = mTileset->toPos(id);
                sf::Vector2u size = mTileset->getTileSize();
                tri[0].texCoords = sf::Vector2f(pos.x, pos.y);
                tri[1].texCoords = sf::Vector2f(pos.x + size.x, pos.y);
                tri[2].texCoords = sf::Vector2f(pos.x + size.x, pos.y + size.y);
                tri[4].texCoords = sf::Vector2f(pos.x, pos.y + size.y);
                tri[3].texCoords = tri[2].texCoords;
                tri[5].texCoords = tri[0].texCoords;
                for (std::size_t i = 0; i < 6; i++)
                {
                    tri[i].color = sf::Color::White;
                    tri[i].color.a = 255 * mOpacity;
                }
            }
        }
    }
}

unsigned int Layer::getTileId(sf::Vector2u coords)
{
    sf::Vector2u size = mMap.getMapSize();
    if (0 <= coords.x && coords.x < size.x && 0 <= coords.y && coords.y < size.y)
    {
        sf::Vertex* tri = &mVertices[(coords.x + coords.y * size.x) * 6];
        if (tri[0].texCoords != tri[2].texCoords && mTileset != nullptr)
        {
            return mTileset->toId(sf::Vector2u(tri->texCoords.x, tri->texCoords.y));
        }
    }
    return 0;
}

void Layer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mVisible)
    {
        if (mTileset != nullptr)
        {
            states.transform.translate(mTileset->getTileOffset() + mOffset);
            states.texture = &mTileset->getTexture();
        }
        else
        {
            states.transform.translate(mOffset);
        }
        target.draw(mVertices, states);
    }
}

bool Layer::loadFromCode(std::string const& code)
{
    if (mEncoding != "base64")
    {
        return false;
    }
    if (mCompression != "zlib")
    {
        return false;
    }

    // TODO : Handle decoding and decompressing

    sf::Vector2u coords;
    std::string data;
    std::stringstream ss;
    ss << code;
    ss >> data;
    if (!decompress(data))
    {
        return false;
    }
    std::vector<unsigned char> byteVector;
    sf::Vector2u size = mMap.getMapSize();
    byteVector.reserve(size.x * size.y * 4);
    for (std::string::iterator i = data.begin(); i != data.end(); ++i)
    {
        byteVector.push_back(*i);
    }
    for (std::size_t i = 0; i < byteVector.size() - 3; i += 4)
    {
        setTileId(coords, byteVector[i] | byteVector[i+1] << 8 | byteVector[i+2] << 16 | byteVector[i+3] << 24);
        coords.x = (coords.x + 1) % size.x;
        if (coords.x == 0)
        {
            coords.y++;
        }
    }
    return true;
}

std::string Layer::getCode()
{
    if (mEncoding != "base64")
    {
        std::cerr << "Unsupported encoding : use base64" << std::endl;
    }
    if (mCompression != "zlib")
    {
        std::cerr << "Unsupported compression : use zlib" << std::endl;
    }

    // TODO : Handle encoding and compressing

    std::string data;
    sf::Vector2u size = mMap.getMapSize();
    data.reserve(size.x * size.y * 4);
    sf::Vector2u coords;
    for (coords.y = 0; coords.y < size.y; coords.y++)
    {
        for (coords.x = 0; coords.x < size.x; coords.x++)
        {
            const std::size_t id = getTileId(coords);
            data.push_back((char)(id));
            data.push_back((char)(id >> 8));
            data.push_back((char)(id >> 16));
            data.push_back((char)(id >> 24));
        }
    }
    if (compress(data))
    {
        return data;
    }
    return "";
}

void Layer::update()
{
    std::string orientation = mMap.getOrientation();
    sf::Vector2u size = mMap.getMapSize();
    sf::Vector2f tileSize = static_cast<sf::Vector2f>(mMap.getTileSize());
    sf::Vector2f texSize;
    if (mTileset != nullptr)
    {
        texSize = static_cast<sf::Vector2f>(mTileset->getTileSize());
    }
    else
    {
        texSize = tileSize;
    }
    std::string axis = mMap.getStaggerAxis();
    unsigned int index = (mMap.getStaggerIndex() == "odd") ? 0 : 1;

    mVertices.resize(size.x * size.y * 6);
    for (std::size_t i = 0; i < size.x; ++i)
    {
        for (std::size_t j = 0; j < size.y; ++j)
        {
            sf::Vector2f pos;
            if (orientation == "orthogonal")
            {
                pos.x = i * tileSize.x;
                pos.y = j * tileSize.y;
            }
            else if (orientation == "isometric")
            {
                pos.x = ((float)i-(float)j) * tileSize.x * 0.5f;
                pos.y = (i+j) * tileSize.y * 0.5f;
            }
            else if (orientation == "staggered")
            {
                if (axis == "y")
                {
                    if ((j % 2) == index)
                    {
                        pos.x = i * tileSize.x;
                    }
                    else
                    {
                        pos.x = (i + 0.5f) * tileSize.x;
                    }
                    pos.y = j * tileSize.y * 0.5f;
                }
                else
                {
                    if ((i % 2) == index)
                    {
                        pos.y = j * tileSize.y;
                    }
                    else
                    {
                        pos.y = (j + 0.5f) * tileSize.y;
                    }
                    pos.x = i * tileSize.x * 0.5f;
                }
            }
            else if (orientation == "hexagonal")
            {
                float hexSide = static_cast<float>(mMap.getHexSideLength());
                if (axis == "y")
                {
                    if ((j % 2) == index)
                    {
                        pos.x = i * tileSize.x;
                    }
                    else
                    {
                        pos.x = (i + 0.5f) * tileSize.x;
                    }
                    pos.y = j * ((tileSize.y - hexSide) * 0.5f + hexSide);
                }
                else
                {
                    if ((i % 2) == index)
                    {
                        pos.y = j * tileSize.y;
                    }
                    else
                    {
                        pos.y = (j + 0.5f) * tileSize.y;
                    }
                    pos.x = i * ((tileSize.x - hexSide ) * 0.5f + hexSide);
                }
            }

            sf::Vertex* tri = getVertex({i,j});
            if (tri != nullptr)
            {
                tri[0].position = sf::Vector2f(pos.x, pos.y);
                tri[1].position = sf::Vector2f(pos.x + texSize.x, pos.y);
                tri[2].position = sf::Vector2f(pos.x + texSize.x, pos.y + texSize.y);
                tri[4].position = sf::Vector2f(pos.x, pos.y + texSize.y);
                tri[3].position = tri[2].position;
                tri[5].position = tri[0].position;
            }
        }
    }
}

sf::Vertex* Layer::getVertex(sf::Vector2u const& coords)
{
    std::string order = mMap.getRenderOrder();
    unsigned int tile;
    if (order == "right-up")
    {
        tile = (coords.x + (mMap.getMapSize().y - coords.y - 1) * mMap.getMapSize().x);
    }
    else if (order == "left-up")
    {
        tile = ((mMap.getMapSize().x - coords.x - 1) + (mMap.getMapSize().y - coords.y - 1) * mMap.getMapSize().x);
    }
    else if (order == "left-down")
    {
        tile = ((mMap.getMapSize().x - coords.x - 1) + mMap.getMapSize().y * mMap.getMapSize().x);
    }
    else
    {
        tile = (coords.x + coords.y * mMap.getMapSize().x);
    }
    return &mVertices[tile * 6];
}

} // namespace tmx
