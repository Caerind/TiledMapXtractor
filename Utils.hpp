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
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Compression.hpp"
#include "pugixml.hpp"

namespace tmx
{

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

template <typename T>
T fromString(std::string const& string)
{
    T value;
    std::istringstream iss(string);
    iss >> value;
    return value;
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

template <> inline std::string fromString<std::string>(std::string const& string)
{
    return string;
}

template <> inline bool fromString<bool>(std::string const& string)
{
    if (string == "true")
        return true;
    if (string == "false")
        return false;
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

sf::Color stringToColor(std::string const& color);

class PropertiesHolder
{
    public:
        PropertiesHolder();

        void loadProperties(pugi::xml_node& node);
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
    mProperites[name] = toString(value);
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

class LayerBase : public PropertiesHolder, public sf::Drawable
{
    public:
        LayerBase();

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const = 0;

        virtual LayerType getType() const = 0;

        virtual bool loadFromNode(pugi::xml_node& layer);
        virtual void saveToNode(pugi::xml_node& layer);

        std::string getName() const;

    protected:
        std::string mName;
        sf::Vector2f mOffset;
        float mOpacity;
        bool mVisible;
};

class Image
{
    public:
        Image();

        bool loadFromNode(pugi::xml_node& image);
        void saveToNode(pugi::xml_node& image);

        std::string getFormat() const;
        std::string getSource() const;
        sf::Color getTransparent() const;
        sf::Vector2u getSize() const;

    protected:
        std::string mFormat;
        std::string mSource;
        std::string mTrans;
        sf::Vector2u mSize;
};

} // namespace detail

} // namespace tmx

#endif // UTILS_HPP
