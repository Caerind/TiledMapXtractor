#include "Utils.hpp"
#include "Map.hpp"

namespace tmx
{

sf::Vector2i worldToOrthoCoords(sf::Vector2f const& world, sf::Vector2i const& tileSize)
{
    return sf::Vector2i(world.x / tileSize.x, world.y / tileSize.y);
}

sf::Vector2i worldToIsoCoords(sf::Vector2f const& world, sf::Vector2i const& tileSize)
{
    // TODO : M - Conversion test
    return sf::Vector2i();
}

// TODO : M - Conversion test
sf::Vector2i worldToStaggerCoords(sf::Vector2f const& world, sf::Vector2i const& tileSize, std::string const& axis, std::string const& index)
{
    sf::Vector2f s = sf::Vector2f(tileSize.x * 0.5f, tileSize.y * 0.5f);
    sf::Vector2f mc = sf::Vector2f(floor(world.x / s.x), floor(world.y / s.y));
    sf::Vector2f p = world - sf::Vector2f(mc.x * s.x, mc.y * s.y);
    const float rad = 0.523599f; // = 30 degrees
    int indexInt = (index == "odd") ? 0 : 1;
    if (axis == "y")
    {
        if (((int)mc.x + (int)mc.y) % 2 == indexInt)
        {
            if (std::atan2(s.y - p.y,p.x) > rad)
            {
                mc.x--;
                mc.y--;
            }
        }
        else
        {
            if (std::atan2(-p.y,p.x) > -rad)
            {
                mc.y--;
            }
            else
            {
                mc.x--;
            }
        }
        return sf::Vector2i(floor(mc.x * 0.5f), mc.y);
    }
    else
    {
        if (((int)mc.x + (int)mc.y) % 2 == indexInt)
        {
            if (std::atan2(s.x - p.x,p.y) > rad)
            {
                mc.x--;
                mc.y--;
            }
        }
        else
        {
            if (std::atan2(-p.x,p.y) > -rad)
            {
                mc.x--;
            }
            else
            {
                mc.y--;
            }
        }
        return sf::Vector2i(mc.x, floor(mc.y * 0.5f));
    }
}

sf::Vector2i worldToHexaCoords(sf::Vector2f const& world, sf::Vector2i const& tileSize, unsigned int hexSideLength, std::string const& axis, std::string const& index)
{
    return sf::Vector2i(); // TODO :M - Conversion
}

sf::Vector2i worldToCoords(std::string const& orientation, sf::Vector2f const& world, sf::Vector2i const& tileSize, std::string const& axis, std::string const& index, unsigned int hexSideLength)
{
    if (orientation == "hexagonal")
    {
        return worldToHexaCoords(world, tileSize, hexSideLength, axis, index);
    }
    else if (orientation == "staggered")
    {
        return worldToStaggerCoords(world, tileSize, axis, index);
    }
    else if (orientation == "isometric")
    {
        return worldToIsoCoords(world, tileSize);
    }
    else if (orientation == "orthogonal")
    {
        return worldToOrthoCoords(world, tileSize);
    }
    else
    {
        detail::log("Incorrect orientation in worldToCoords");
        return sf::Vector2i();
    }
}


namespace detail
{

const unsigned int FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned int FLIPPED_VERTICALLY_FLAG   = 0x40000000;
const unsigned int FLIPPED_DIAGONALLY_FLAG   = 0x20000000;

void log(std::string const& message)
{
    std::cerr << "TMX : " << message << std::endl;
}

void readFlip(unsigned int& gid)
{
    gid &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);
}

void readFlip(unsigned int& gid, bool& horizontal, bool& vertical, bool& diagonal)
{
    horizontal = (gid & FLIPPED_HORIZONTALLY_FLAG);
    vertical = (gid & FLIPPED_VERTICALLY_FLAG);
    diagonal = (gid & FLIPPED_DIAGONALLY_FLAG);
    gid &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);
}

PropertiesHolder::PropertiesHolder()
: mProperites()
{
}

void PropertiesHolder::loadProperties(pugi::xml_node const& node)
{
    pugi::xml_node properties = node.child("properties");
    if (properties)
    {
        for (const pugi::xml_node& property : properties.children("property"))
        {
            mProperites[property.attribute("name").as_string()] = property.attribute("value").as_string();
        }
    }
}

void PropertiesHolder::saveProperties(pugi::xml_node& node)
{
    if (mProperites.size() > 0)
    {
        pugi::xml_node properties = node.append_child("properties");
        for (auto itr = mProperites.begin(); itr != mProperites.end(); itr++)
        {
            pugi::xml_node property = properties.append_child("property");
            property.append_attribute("name") = itr->first.c_str();
            property.append_attribute("value") = itr->second.c_str();
        }
    }
}

Image::Image()
: mData("")
, mFormat("")
, mSource("")
, mTransparent(sf::Color::Transparent)
, mSize({0, 0})
{
}

bool Image::loadFromNode(pugi::xml_node const& image)
{
    for (const pugi::xml_attribute& attr : image.attributes())
    {
        if (attr.name() == std::string("format"))
        {
            mFormat = attr.as_string();
        }
        if (attr.name() == std::string("source"))
        {
            mSource = attr.as_string();
        }
        if (attr.name() == std::string("trans"))
        {
            mTransparent = detail::fromString<sf::Color>(attr.as_string());
        }
        if (attr.name() == std::string("width"))
        {
            mSize.x = attr.as_uint();
        }
        if (attr.name() == std::string("height"))
        {
            mSize.y = attr.as_uint();
        }
    }
    pugi::xml_node data = image.child("data");
    if (data)
    {
        std::stringstream ss(data.text().get());
        ss >> mData;
    }
    return true;
}

void Image::saveToNode(pugi::xml_node& image)
{
    if (mFormat != "")
    {
        image.append_attribute("format") = mFormat.c_str();
    }
    if (mSource != "")
    {
        image.append_attribute("source") = mSource.c_str();
    }
    if (mTransparent != sf::Color::Transparent)
    {
        image.append_attribute("trans") = detail::toString<sf::Color>(mTransparent).c_str();
    }
    if (mSize != sf::Vector2i())
    {
        image.append_attribute("width") = mSize.x;
        image.append_attribute("height") = mSize.y;
    }
    if (mData != "")
    {
        pugi::xml_node data = image.append_child("data");
        data.text().set(mData.data());
    }
}

const std::string& Image::getData() const
{
    return mData;
}

const std::string& Image::getFormat() const
{
    return mFormat;
}

const std::string& Image::getSource() const
{
    return mSource;
}

const sf::Color& Image::getTransparent() const
{
    return mTransparent;
}

const sf::Vector2i& Image::getSize() const
{
    return mSize;
}

void Image::setData(std::string const& data)
{
    mData = data;
}

void Image::setFormat(std::string const& format)
{
    mFormat = format;
}

void Image::setSource(std::string const& source)
{
    mSource = source;
}

void Image::setTransparent(sf::Color const& color)
{
    mTransparent = color;
}

void Image::setSize(sf::Vector2i const& size)
{
    mSize = size;
}

bool Image::loadTexture(sf::Texture& texture, std::string const& additionalPath) const
{
    if (mTransparent != sf::Color::Transparent)
    {
        sf::Image image;
        if (mSource != "")
        {
            if (!image.loadFromFile(additionalPath + mSource))
            {
                detail::log("Unable to load image from file : " + additionalPath + mSource);
                return false;
            }
        }
        else if (mData != "")
        {
            if (!image.loadFromMemory(mData.data(), mData.size()))
            {
                detail::log("Unable to load image from memory");
                return false;
            }
        }
        else
        {
            return false;
        }

        image.createMaskFromColor(mTransparent);

        if (!texture.loadFromImage(image))
        {
            detail::log("Unable to load texture from image");
            return false;
        }
        return true;
    }
    else
    {
        if (mSource != "")
        {
            bool ret = texture.loadFromFile(additionalPath + mSource);
            if (!ret)
            {
                detail::log("Unable to load texture from file : " + additionalPath + mSource);
            }
            return ret;
        }
        else if (mData != "")
        {
            bool ret = texture.loadFromMemory(mData.data(), mData.size());
            if (!ret)
            {
                detail::log("Unable to load texture from memory");
            }
            return ret;
        }
        else
        {
            return false;
        }
    }
}

} // namespace detail

LayerBase::LayerBase()
: mName("")
, mOffset({0.f, 0.f})
, mOpacity(1.f)
, mVisible(true)
{
}

void LayerBase::update()
{
}

bool LayerBase::loadFromNode(pugi::xml_node const& layer)
{
    for (const pugi::xml_attribute& attr : layer.attributes())
    {
        if (attr.name() == std::string("name"))
        {
            mName = attr.as_string();
        }
        if (attr.name() == std::string("offsetx"))
        {
            mOffset.x = attr.as_float();
        }
        if (attr.name() == std::string("offsety"))
        {
            mOffset.y = attr.as_float();
        }
        if (attr.name() == std::string("opacity"))
        {
            mOpacity = attr.as_float();
        }
        if (attr.name() == std::string("visible"))
        {
            mVisible = detail::fromString<bool>(attr.as_string());
        }
    }
    loadProperties(layer);
    return true;
}

void LayerBase::saveToNode(pugi::xml_node& layer)
{
    if (mName != "")
    {
        layer.append_attribute("name") = mName.c_str();
    }
    if (mOffset != sf::Vector2f())
    {
        layer.append_attribute("offsetx") = mOffset.x;
        layer.append_attribute("offsety") = mOffset.y;
    }
    if (mOpacity != 1.f)
    {
        layer.append_attribute("opacity") = mOpacity;
    }
    if (!mVisible)
    {
        layer.append_attribute("visible") = "false";
    }
    saveProperties(layer);
}

const std::string& LayerBase::getName() const
{
    return mName;
}

const sf::Vector2f& LayerBase::getOffset() const
{
    return mOffset;
}

float LayerBase::getOpacity() const
{
    return mOpacity;
}

bool LayerBase::isVisible() const
{
    return mVisible;
}

void LayerBase::setName(std::string const& name)
{
    mName = name;
}

void LayerBase::setOffset(sf::Vector2f const& offset)
{
    mOffset = offset;
    update();
}

void LayerBase::setOpacity(float opacity)
{
    mOpacity = opacity;
    update();
}

void LayerBase::setVisible(bool visible)
{
    mVisible = visible;
}

ImageLayer::ImageLayer(Map& map)
: mMap(map)
{
}

LayerType ImageLayer::getLayerType() const
{
    return tmx::EImageLayer;
}

bool ImageLayer::loadFromNode(pugi::xml_node const& layer)
{
    LayerBase::loadFromNode(layer);
    mImage.loadFromNode(layer.child("image"));
    return loadImage();
}

void ImageLayer::saveToNode(pugi::xml_node& layer)
{
    LayerBase::saveToNode(layer);
    pugi::xml_node image = layer.append_child("image");
    mImage.saveToNode(image);
}

const std::string& ImageLayer::getData() const
{
    return mImage.getData();
}

const std::string& ImageLayer::getSource() const
{
    return mImage.getSource();
}

const std::string& ImageLayer::getFormat() const
{
    return mImage.getFormat();
}

const sf::Color& ImageLayer::getTransparent() const
{
    return mImage.getTransparent();
}

const sf::Vector2i& ImageLayer::getSize() const
{
    return mImage.getSize();
}

void ImageLayer::setData(std::string const& data)
{
    mImage.setData(data);
}

void ImageLayer::setSource(std::string const& source)
{
    mImage.setSource(source);
}

void ImageLayer::setFormat(std::string const& format)
{
    mImage.setFormat(format);
}

void ImageLayer::setTransparent(sf::Color const& transparent)
{
    mImage.setTransparent(transparent);
}

void ImageLayer::setSize(sf::Vector2i const& size)
{
    mImage.setSize(size);
}

bool ImageLayer::loadImage()
{
    bool ret = mImage.loadTexture(mTexture, mMap.getPath());
    update();
    return ret;
}

void ImageLayer::update()
{
    sf::Vector2f textureSize = static_cast<sf::Vector2f>(mTexture.getSize());
    sf::Color color = sf::Color(255, 255, 255, static_cast<unsigned char>(255.f * mOpacity));
    mVertices[0] = sf::Vertex(sf::Vector2f(0.f, 0.f), color, sf::Vector2f(0.f, 0.f));
    mVertices[1] = sf::Vertex(sf::Vector2f(textureSize.x, 0.f), color, sf::Vector2f(textureSize.x, 0.f));
    mVertices[2] = sf::Vertex(textureSize, color, textureSize);
    mVertices[3] = mVertices[2];
    mVertices[4] = sf::Vertex(sf::Vector2f(0.f, textureSize.y), color, sf::Vector2f(0.f, textureSize.y));
    mVertices[5] = mVertices[0];
}

void ImageLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mVisible)
    {
        states.transform.translate(mOffset + mMap.getMapOffset());
        states.texture = &mTexture;
        target.draw(mVertices, 6, sf::Triangles, states);
    }
}

} // namespace tmx
