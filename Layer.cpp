#include "Layer.hpp"
#include "Map.hpp"

Layer::Layer(Map* map)
{
    mMap = map;
    mTileset = nullptr;
    mVertices.setPrimitiveType(sf::Triangles);

    mName = "";
    mSize = {0, 0};
    mOpacity = 1.f;
    mVisible = true;
    mOffset = {0.f, 0.f};

    mEncoding = "";
    mCompression = "";
}

bool Layer::loadFromNode(pugi::xml_node& layer)
{
    if (!layer)
    {
        std::cerr << "Invalid layer node" << std::endl;
        return false;
    }
    for (pugi::xml_attribute attr = layer.first_attribute(); attr; attr = attr.next_attribute())
    {
        if (attr.name() == std::string("name")) mName = attr.value();
        if (attr.name() == std::string("width")) mSize.x = attr.as_uint();
        if (attr.name() == std::string("height")) mSize.y = attr.as_uint();
        if (attr.name() == std::string("opacity")) mOpacity = attr.as_float();
        if (attr.name() == std::string("visible")) mVisible = attr.as_bool();
        if (attr.name() == std::string("offsetx")) mOffset.x = attr.as_float();
        if (attr.name() == std::string("offsety")) mOffset.y = attr.as_float();
    }

    pugi::xml_node data = layer.child("data");
    if (!data)
    {
        std::cerr << "Invalid data node" << std::endl;
        return false;
    }
    for (pugi::xml_attribute attr = data.first_attribute(); attr; attr = attr.next_attribute())
    {
        if (attr.name() == std::string("encoding")) mEncoding = attr.value();
        if (attr.name() == std::string("compression")) mCompression = attr.value();
    }

    update();
    if (!loadFromCode(data.text().get()))
    {
        std::cerr << "Unable to load from code" << std::endl;
        return false;
    }

    pugi::xml_node properties = layer.child("properties");
    if (properties)
        mProperties.loadFromNode(properties);

    return true;
}

bool Layer::saveToNode(pugi::xml_node& layer)
{
    layer.append_attribute("name") = mName.c_str();
    layer.append_attribute("width") = mSize.x;
    layer.append_attribute("height") = mSize.y;
    if (mOpacity != 1.f)
        layer.append_attribute("opacity") = mOpacity;
    if (!mVisible)
        layer.append_attribute("visible") = mVisible;
    if (mOffset.x != 0.f)
        layer.append_attribute("offsetx") = mOffset.x;
    if (mOffset.y != 0.f)
        layer.append_attribute("offsety") = mOffset.y;

    pugi::xml_node data = layer.append_child("data");
    data.append_attribute("encoding") = mEncoding.c_str();
    data.append_attribute("compression") = mCompression.c_str();

    data.text().set(getCode().c_str());

    mProperties.saveToNode(layer);

    return true;
}

void Layer::setTileId(sf::Vector2u coords, unsigned int id)
{
    if (0 <= coords.x && 0 <= coords.y && coords.x < mSize.x && coords.y < mSize.y)
    {
        sf::Vertex* tri = &mVertices[(coords.x + coords.y * mSize.x) * 6];
        if (id == 0)
        {
            for (std::size_t i = 0; i < 6; i++)
            {
                tri[i].texCoords = sf::Vector2f(0.f, 0.f);
                tri[i].color = sf::Color::Transparent;
            }
        }
        else
        {
            if (mTileset == nullptr)
            {
                mTileset = mMap->getTileset(id);
            }
            if (mTileset != nullptr)
            {
                sf::Vector2u pos = mTileset->toPos(id);
                sf::Vector2u size = mTileset->getTileSize();
                tri[0].texCoords = sf::Vector2f(pos.x, pos.y);
                tri[1].texCoords = sf::Vector2f(pos.x + size.x, pos.y);
                tri[2].texCoords = sf::Vector2f(pos.x + size.x, pos.y + size.y);
                tri[3].texCoords = tri[2].texCoords;
                tri[4].texCoords = sf::Vector2f(pos.x, pos.y + size.y);
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
    if (0 <= coords.x && coords.x < mSize.x && 0 <= coords.y && coords.y < mSize.y)
    {
        sf::Vertex* tri = &mVertices[(coords.x + coords.y * mSize.x) * 6];
        if (tri[0].texCoords != tri[2].texCoords && mTileset != nullptr)
        {
            return mTileset->toId(sf::Vector2u(tri->texCoords.x, tri->texCoords.y));
        }
    }
    return 0;
}

std::string Layer::getName() const
{
    return mName;
}

void Layer::render(sf::RenderTarget& target, sf::RenderStates states)
{
    if (mVisible)
    {
        states.transform.translate(mOffset);
        if (mTileset != nullptr)
        {
            states.texture = &mTileset->getTexture();
        }
        target.draw(mVertices, states);
    }
}

bool Layer::loadFromCode(std::string const& code)
{
    if (mEncoding != "base64")
        std::cerr << "Unsupported encoding : use base64" << std::endl;
    if (mCompression != "zlib")
        std::cerr << "Unsupported compression : use zlib" << std::endl;

    // TODO : Handle decoding and decompressing

    sf::Vector2u coords;
    std::string data;
    std::stringstream ss;
    ss << code;
    ss >> data;
    if (!decompress(data))
    {
        std::cerr << "Unable to decompress the data" << std::endl;
        return false;
    }
    std::vector<unsigned char> byteVector;
    byteVector.reserve(mSize.x * mSize.y * 4);
    for (std::string::iterator i = data.begin(); i != data.end(); ++i)
        byteVector.push_back(*i);
    for (std::size_t i = 0; i < byteVector.size() - 3; i += 4)
    {
        std::size_t id = byteVector[i] | byteVector[i+1] << 8 | byteVector[i+2] << 16 | byteVector[i+3] << 24;
        setTileId(coords, id);
        //std::cout << id << " ";
        coords.x = (coords.x + 1) % mSize.x;
        if (coords.x == 0)
        {
            coords.y++;
            //std::cout << "/" << std::endl;
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
    data.reserve(mSize.x * mSize.y * 4);
    sf::Vector2u coords;
    for (coords.y = 0; coords.y < mSize.y; coords.y++)
    {
        for (coords.x = 0; coords.x < mSize.x; coords.x++)
        {
            const std::size_t id = getTileId(coords);
            //std::cout << id << " ";
            data.push_back((char)(id));
            data.push_back((char)(id >> 8));
            data.push_back((char)(id >> 16));
            data.push_back((char)(id >> 24));
        }
        // std::cout << "/" << std::endl;
    }
    if (compress(data))
        return data;
    std::cerr << "Unable to compress the data" << std::endl;
    return "";
}

void Layer::update()
{
    if (mMap != nullptr)
    {
        std::string orientation = mMap->getOrientation();
        sf::Vector2u tileSize = mMap->getTileSize();

        // TODO : Handle different orientation

        mVertices.resize(mSize.x * mSize.y * 6);
        for (std::size_t i = 0; i < mSize.x; ++i)
        {
            for (std::size_t j = 0; j < mSize.y; ++j)
            {
                sf::Vertex* quad = &mVertices[(i + j * mSize.x) * 6];
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = quad[2].position;
                quad[4].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
                quad[5].position = quad[0].position;
            }
        }
    }
}
