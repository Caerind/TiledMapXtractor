#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include "Utils.hpp"

namespace tmx
{

class ObjectGroup;
class ObjectBase : public detail::PropertiesHolder, public sf::Drawable
{
    public:
        ObjectBase(ObjectGroup& group);

        virtual ObjectType getObjectType() const = 0;

        virtual void loadFromNode(pugi::xml_node const& object);
        virtual void saveToNode(pugi::xml_node& object);

        virtual void setColor(sf::Color const& color) = 0;

        unsigned int getId() const;
        unsigned int getGid() const;
        const std::string& getName() const;
        const std::string& getType() const;
        const sf::Vector2f& getPosition() const;
        const sf::Vector2f& getSize() const;
        float getRotation() const;
        bool isVisible() const;

        void setId(unsigned int id);
        void setGid(unsigned int gid);
        void setName(std::string const& name);
        void setType(std::string const& type);
        void setPosition(sf::Vector2f const& position);
        void setSize(sf::Vector2f const& size);
        void setRotation(float rotation);
        void setVisible(bool visible);

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const = 0;

        virtual void update();

    protected:
        ObjectGroup& mGroup;
        unsigned int mId;
        unsigned int mGid;
        std::string mName;
        std::string mType;
        sf::Vector2f mPosition;
        sf::Vector2f mSize;
        float mRotation;
        bool mVisible;
};

}

#endif // OBJECTBASE_H
