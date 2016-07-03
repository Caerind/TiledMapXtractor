#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sstream>
#include <unordered_map>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
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

        virtual LayerType getLayerType() const = 0;

        virtual bool loadFromNode(pugi::xml_node& layer);
        virtual void saveToNode(pugi::xml_node& layer);

        std::string getName() const;

    protected:
        std::string mName;
        sf::Vector2i mOffset;
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
