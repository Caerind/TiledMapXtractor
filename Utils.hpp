#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sstream>
#include <unordered_map>

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "Compression.hpp"
#include "pugixml.hpp"

namespace tmx
{

class Map;

enum LayerType
{
    ELayer,
    EObjectGroup,
    EImageLayer
};

enum ObjectType
{
    ESimple,
    EEllipse,
    EPolygon,
    EPolyline
};

namespace detail
{

template <typename T>
std::string toString(T const& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template <> inline std::string toString<std::string>(std::string const& value)
{
    return value;
}

template <> inline std::string toString<bool>(bool const& value)
{
    return (value) ? "true" : "false";
}

template <> inline std::string toString<sf::Vector2f>(sf::Vector2f const& value)
{
    std::ostringstream oss;
    oss << value.x << "," << value.y;
    return oss.str();
}

template <> inline std::string toString<sf::Color>(sf::Color const& value)
{
    std::ostringstream oss;
    oss << std::hex << value.toInteger();
    return oss.str(); // TODO : Update with # when fully supported by Tiled
}

template <typename T>
T fromString(std::string const& string)
{
    T value;
    std::istringstream iss(string);
    iss >> value;
    return value;
}

template <> inline std::string fromString<std::string>(std::string const& string)
{
    return string;
}

template <> inline bool fromString<bool>(std::string const& string)
{
    if (string == "true")
    {
        return true;
    }
    if (string == "false")
    {
        return false;
    }
    bool value;
    std::istringstream iss(string);
    iss >> value;
    return value;
}

template <> inline sf::Vector2f fromString<sf::Vector2f>(std::string const& string)
{
    sf::Vector2f vector;
    std::size_t found = string.find_first_of(',');
    if (found != std::string::npos)
    {
        std::istringstream iss(string.substr(0, found));
        iss >> vector.x;
        iss.clear();
        iss.str(string.substr(found + 1));
        iss >> vector.y;
    }
    return vector;
}

template <> inline sf::Color fromString<sf::Color>(std::string const& string)
{
    std::string color = string;
    if (color != "")
    {
        if (color[0] == '#')
        {
            color.erase(color.begin());
        }
        int hexTrans;
        std::stringstream ss(color);
        ss >> std::hex >> hexTrans;
        if (hexTrans >= 0)
        {
            unsigned char red, green, blue;
            red = hexTrans >> 16;
            green = (hexTrans >> 8) & 0xff;
            blue = hexTrans & 0xff;
            return sf::Color(red, green, blue);
        }
    }
    return sf::Color::Transparent;
}

class PropertiesHolder
{
    public:
        PropertiesHolder();

        void loadProperties(pugi::xml_node const& node);
        void saveProperties(pugi::xml_node& node);

        template <typename T>
        void setProperty(std::string const& name, T const& value);

        template <typename T>
        T getProperty(std::string const& name);

    protected:
        std::unordered_map<std::string,std::string> mProperites;
};

template <typename T>
void PropertiesHolder::setProperty(std::string const& name, T const& value)
{
    mProperites[name] = toString<T>(value);
}

template <typename T>
T PropertiesHolder::getProperty(std::string const& name)
{
    T value;
    auto itr = mProperites.find(name);
    if (itr != mProperites.end())
    {
        value = fromString<T>(itr->second);
    }
    return value;
}

class Image
{
    public:
        Image();

        bool loadFromNode(pugi::xml_node const& image);
        void saveToNode(pugi::xml_node& image);

        const std::string& getFormat() const;
        const std::string& getSource() const;
        sf::Color getTransparent() const;
        const sf::Vector2u& getSize() const;

        void setFormat(std::string const& format);
        void setSource(std::string const& image);
        void setTransparent(sf::Color const& color);
        void setSize(sf::Vector2u const& size);

    protected:
        std::string mFormat;
        std::string mSource;
        std::string mTransparent;
        sf::Vector2u mSize;
};

} // namespace detail

class LayerBase : public detail::PropertiesHolder, public sf::Drawable
{
    public:
        LayerBase();

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const = 0;

        virtual LayerType getLayerType() const = 0;

        virtual bool loadFromNode(pugi::xml_node const& layer);
        virtual void saveToNode(pugi::xml_node& layer);

        const std::string& getName() const;
        const sf::Vector2f& getOffset() const;
        float getOpacity() const;
        bool isVisible() const;

        virtual void setName(std::string const& name);
        virtual void setOffset(sf::Vector2f const& offset);
        virtual void setOpacity(float opacity);
        virtual void setVisible(bool visible);

    protected:
        std::string mName;
        sf::Vector2f mOffset;
        float mOpacity;
        bool mVisible;
};

class ImageLayer : public LayerBase
{
    public:
        ImageLayer(Map& map);

        LayerType getLayerType() const;

        bool loadFromNode(pugi::xml_node const& layer);
        void saveToNode(pugi::xml_node& layer);

        const std::string& getSource() const;
        const std::string& getFormat() const;
        sf::Color getTransparent() const;
        const sf::Vector2u& getSize() const;

        void setSource(std::string const& source);
        void setFormat(std::string const& format);
        void setTransparent(sf::Color const& transparent);
        void setSize(sf::Vector2u const& size);

        bool loadImage();

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

    protected:
        Map& mMap;
        detail::Image mImage;
        sf::Texture mTexture;
        sf::Vertex mVertices[6];
};

} // namespace tmx

#endif // UTILS_HPP
