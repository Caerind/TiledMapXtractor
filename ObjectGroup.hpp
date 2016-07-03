#ifndef TMX_OBJECTGROUP_HPP
#define TMX_OBJECTGROUP_HPP

#include "Map.hpp"
#include "Utils.hpp"

namespace tmx
{

class Map;

namespace detail
{

class ObjectBase : public PropertiesHolder, public sf::Drawable
{
    public:
        ObjectBase();

        virtual ObjectType getType() const = 0;

        virtual void loadFromNode(pugi::xml_node& object);
        virtual void saveToNode(pugi::xml_node& object);

        virtual void setColor(sf::Color const& color);

        sf::Vector2f getPosition() const;
        unsigned int getId() const;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const = 0;

    protected:
        unsigned int mId;
        std::string mName;
        std::string mType;
        sf::Vector2f mPosition;
        sf::Vector2f mSize;
        float mRotation;
        unsigned int mGid;
        bool mVisible;
};

} // namespace detail

class Object : public detail::ObjectBase
{
    public:
        Object(Map* map);

        ObjectType getType() const;

        void loadFromNode(pugi::xml_node& object);

        void setColor(sf::Color const& color);

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

    private:
        Map* mMap;
        sf::RectangleShape mShape;
};

class Ellipse : public detail::ObjectBase
{
    public:
        Ellipse();

        ObjectType getType() const;

        void loadFromNode(pugi::xml_node& object);
        void saveToNode(pugi::xml_node& object);

        void setColor(sf::Color const& color);

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

    private:
        sf::ConvexShape mShape;
};

class Polygon : public detail::ObjectBase
{
    public:
        Polygon();

        ObjectType getType() const;

        void loadFromNode(pugi::xml_node& object);
        void saveToNode(pugi::xml_node& object);

        void setColor(sf::Color const& color);

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

    private:
        sf::ConvexShape mShape;
};

class Polyline : public detail::ObjectBase
{
    public:
        Polyline();

        ObjectType getType() const;

        void loadFromNode(pugi::xml_node& object);
        void saveToNode(pugi::xml_node& object);

        void setColor(sf::Color const& color);

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

    private:
        std::vector<sf::RectangleShape> mShapes;
};

class ObjectGroup : public detail::LayerBase
{
    public:
        ObjectGroup(Map* map);

        LayerType getType() const;

        bool loadFromNode(pugi::xml_node& layer);
        void saveToNode(pugi::xml_node& layer);

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

        void sort(std::string const& order = "topdown");

    protected:
        Map* mMap;
        std::string mColor;
        std::string mDrawOrder;
        std::vector<detail::ObjectBase*> mObjects;
};

} // namespace tmx

#endif // TMX_OBJECTGROUP_HPP
