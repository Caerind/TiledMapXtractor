#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "ObjectBase.h"

namespace tmx
{

class ObjectGroup;
class Ellipse : public ObjectBase
{
    public:
        Ellipse(ObjectGroup& group);

        typedef std::shared_ptr<Ellipse> Ptr;

        ObjectType getObjectType() const;

        void setPointCount(std::size_t count);

        void update();

        void saveToNode(pugi::xml_node& object);

        void setColor(sf::Color const& color);

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

    private:
        sf::ConvexShape mShape;
        std::size_t mPointCount;
};

}

#endif // ELLIPSE_H
