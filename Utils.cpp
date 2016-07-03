#include "Utils.hpp"

namespace tmx
{

namespace detail
{

PropertiesHolder::PropertiesHolder()
: mProperites()
{
}

void PropertiesHolder::loadProperties(pugi::xml_node const& node)
{
    if (!node)
    {
        return;
    }
    pugi::xml_node properties = node.child("properties");
    if (properties)
    {
        for (pugi::xml_node property = properties.child("property"); property; property = property.next_sibling("property"))
        {
            std::string name;
            pugi::xml_attribute attr = property.attribute("name");
            if (attr)
            {
                name = attr.as_string();
                attr = property.attribute("value");
                if (attr)
                {
                    mProperites[name] = attr.as_string();
                }
            }
        }
    }
}

void PropertiesHolder::saveProperties(pugi::xml_node& node)
{
    if (!node)
    {
        return;
    }
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

LayerBase::LayerBase()
: mName("")
, mOffset({0.f, 0.f})
, mOpacity(1.f)
, mVisible(true)
{
}

bool LayerBase::loadFromNode(pugi::xml_node const& layer)
{
    if (!layer)
    {
        return false;
    }
    for (pugi::xml_attribute attr = layer.first_attribute(); attr; attr = attr.next_attribute())
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
            mVisible = fromString<bool>(attr.as_string());
        }
    }
    loadProperties(layer);
    return true;
}

void LayerBase::saveToNode(pugi::xml_node& layer)
{
    if (!layer)
    {
        return;
    }
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
}

void LayerBase::setOpacity(float opacity)
{
    mOpacity = opacity;
}

void LayerBase::setVisible(bool visible)
{
    mVisible = visible;
}

Image::Image()
: mFormat("")
, mSource("")
, mTransparent("")
, mSize({0, 0})
{
}

bool Image::loadFromNode(pugi::xml_node const& image)
{
    if (!image)
    {
        return false;
    }
    for (pugi::xml_attribute attr = image.first_attribute(); attr; attr = attr.next_attribute())
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
            mTransparent = attr.as_string();
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
    // TODO : Load Data
    return true;
}

void Image::saveToNode(pugi::xml_node& image)
{
    if (!image)
    {
        return;
    }
    if (mFormat != "")
    {
        image.append_attribute("format") = mFormat.c_str();
    }
    if (mSource != "")
    {
        image.append_attribute("source") = mSource.c_str();
    }
    if (mTransparent != "")
    {
        image.append_attribute("trans") = mTransparent.c_str();
    }
    if (mSize != sf::Vector2u())
    {
        image.append_attribute("width") = mSize.x;
        image.append_attribute("height") = mSize.y;
    }
    // TODO : Save Data
}

const std::string& Image::getFormat() const
{
    return mFormat;
}

const std::string& Image::getSource() const
{
    return mSource;
}

sf::Color Image::getTransparent() const
{
    return fromString<sf::Color>(mTransparent);
}

const sf::Vector2u& Image::getSize() const
{
    return mSize;
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
    mTransparent = detail::toString<sf::Color>(color);
}

void Image::setSize(sf::Vector2u const& size)
{
    mSize = size;
}

} // namespace detail

} // namespace tmx
