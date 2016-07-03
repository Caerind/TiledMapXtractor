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

        typedef std::shared_ptr<ObjectBase> Ptr;

        virtual ObjectType getObjectType() const = 0;

        virtual void loadFromNode(pugi::xml_node& object);
        virtual void saveToNode(pugi::xml_node& object);

        virtual void setColor(sf::Color const& color) = 0;

        virtual unsigned int getId() const;
        virtual unsigned int getGid() const;
        virtual std::string getName() const;
        virtual std::string getType() const;
        virtual sf::Vector2f getPosition() const;
        virtual sf::Vector2f getSize() const;
        virtual float getRotation() const;
        virtual bool isVisible() const;

        virtual void setId(unsigned int id);
        virtual void setGid(unsigned int gid);
        virtual void setName(std::string const& name);
        virtual void setType(std::string const& type);
        virtual void setPosition(sf::Vector2f const& position);
        virtual void setSize(sf::Vector2f const& size);
        virtual void setRotation(float rotation);
        virtual void setVisible(bool visible);

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const = 0;

    protected:
        unsigned int mId;
        unsigned int mGid;
        std::string mName;
        std::string mType;
        sf::Vector2f mPosition;
        sf::Vector2f mSize;
        float mRotation;
        bool mVisible;
};

} // namespace detail

class Object : public detail::ObjectBase
{
    public:
        Object(Map& map);

        typedef std::shared_ptr<Object> Ptr;

        ObjectType getObjectType() const;

        void loadFromNode(pugi::xml_node& object);

        void setColor(sf::Color const& color);

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

    private:
        Map& mMap;
        sf::RectangleShape mShape;
};

class Ellipse : public detail::ObjectBase
{
    public:
        Ellipse();

        typedef std::shared_ptr<Ellipse> Ptr;

        ObjectType getObjectType() const;

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

        typedef std::shared_ptr<Polygon> Ptr;

        ObjectType getObjectType() const;

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

        typedef std::shared_ptr<Polyline> Ptr;

        ObjectType getObjectType() const;

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
        ObjectGroup(Map& map);

        LayerType getType() const;

        bool loadFromNode(pugi::xml_node& layer);
        void saveToNode(pugi::xml_node& layer);

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

        sf::Color getColor() const;
        void setColor(sf::Color const& color);

        std::string getDrawOrder() const;
        void setDrawOrder(std::string const& order);

        void sort(std::string const& order = "topdown");

        std::size_t getObjectCount() const;
        detail::ObjectBase::Ptr getObject(std::size_t index);
        void addObject(detail::ObjectBase::Ptr object);

    protected:
        Map& mMap;
        std::string mColor;
        std::string mDrawOrder;
        std::vector<detail::ObjectBase::Ptr> mObjects;
};

} // namespace tmx

#endif // TMX_OBJECTGROUP_HPP
