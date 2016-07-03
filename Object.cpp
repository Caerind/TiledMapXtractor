#include "Object.h"
#include "Tileset.hpp"
#include "ObjectGroup.hpp"
#include "Map.hpp"

namespace tmx
{

Object::Object(ObjectGroup& group)
: ObjectBase(group)
, mShape()
{
}

ObjectType Object::getObjectType() const
{
    return tmx::ESimple;
}

void Object::update()
{
    mShape.setSize(mSize);
    mShape.setRotation(mRotation);
    if (mGid != 0)
    {
        Tileset* tileset = mGroup.getMap().getTileset(mGid);
        if (tileset == nullptr)
        {
            return;
        }
        mShape.setTexture(&tileset->getTexture());
        mShape.setTextureRect(tileset->toRect(mGid));
        // TODO : Tile Flipping
        // TODO : Map Orientation
        std::string orientation = mGroup.getMap().getOrientation();
        if (orientation == "orthogonal")
        {
            mShape.setPosition(mPosition - sf::Vector2f(0.f, mSize.y));
        }
        else if (orientation == "isometric")
        {
            mShape.setPosition(mPosition - sf::Vector2f(mSize.x * 0.5f, mSize.y));
        }
        mShape.setOutlineThickness(0.f);
    }
    else
    {
        mShape.setOutlineThickness(2.f);
        mShape.setPosition(mPosition);
    }
}

void Object::setColor(sf::Color const& color)
{
    if (mGid == 0)
    {
        sf::Color c = color;
        c.a = 32;
        mShape.setOutlineColor(color);
        mShape.setFillColor(c);
    }
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mVisible)
    {
        target.draw(mShape, states);
    }
}

}
