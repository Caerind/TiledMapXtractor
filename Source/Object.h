#ifndef OBJECT_H
#define OBJECT_H

#include "Utils.hpp"
#include "ObjectBase.h"

namespace tmx
{

class ObjectGroup;
class Object : public ObjectBase
{
    public:
        Object(ObjectGroup& group);

        ObjectType getObjectType() const;

        void update();

        void setColor(sf::Color const& color);

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

    private:
        sf::RectangleShape mShape;
};

}

#endif // OBJECT_H
