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
    if (mGid != 0)
    {
        Tileset* tileset = mGroup.getMap().getTileset(mGid);
        if (tileset == nullptr)
        {
            return;
        }
        mShape.setTexture(&tileset->getTexture());
        mShape.setTextureRect(tileset->toRect(mGid));

        std::string orientation = mGroup.getMap().getOrientation();
        if (orientation == "orthogonal")
        {
            mShape.setOrigin(sf::Vector2f(0.f, mSize.x));
        }
        else
        {
            mShape.setOrigin(mSize.x * 0.5f, mSize.y);
        }
        mShape.setRotation(mRotation);
        mShape.setPosition(mPosition + getLayerOffset());

        // TODO : O - Flip
        /*
        sf::Vector2f scale = sf::Vector2f(1.f, 1.f);
        if (mFlippedHorizontally)
        {
            scale.x = -1.f;
            mShape.move(mSize.x, 0.f);
        }
        if (mFlippedVertically)
        {
            scale.y = -1.f;
            mShape.move(0.f, mSize.y);
        }
        mShape.setScale(scale);
        */

        mShape.setOutlineThickness(0.f);
    }
    else
    {
        mShape.setRotation(mRotation);
        mShape.setOutlineThickness(2.f);
        mShape.setPosition(mPosition + getLayerOffset() + getMapOffset());
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
    else
    {
        sf::Color c = color;
        c.r = 255;
        c.g = 255;
        c.b = 255;
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
