#ifndef POLYGON_H
#define POLYGON_H

#include "ObjectBase.h"

namespace tmx
{

class ObjectGroup;
class Polygon : public ObjectBase
{
    public:
        Polygon(ObjectGroup& group);

        ObjectType getObjectType() const;

        void loadFromNode(pugi::xml_node const& object);
        void saveToNode(pugi::xml_node& object);

        void update();

        void addPoint(sf::Vector2f const& point);
        void addPoint(sf::Vector2f const& point, std::size_t index);
        sf::Vector2f getPoint(std::size_t index) const;
        void setPoint(std::size_t index, sf::Vector2f const& point);
        void removePoint(std::size_t index);
        std::size_t getPointCount() const;

        void setColor(sf::Color const& color);

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

    private:
        thor::ConcaveShape mShape;
};

}

#endif // POLYGON_H
