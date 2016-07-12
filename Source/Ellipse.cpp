#include "Ellipse.h"
#include "ObjectGroup.hpp"

namespace tmx
{

Ellipse::Ellipse(ObjectGroup& group)
: ObjectBase(group)
, mShape()
, mPointCount(50)
{
}

ObjectType Ellipse::getObjectType() const
{
    return tmx::EEllipse;
}

void Ellipse::setPointCount(std::size_t count)
{
    mPointCount = count;
    update();
    mGroup.sort(mGroup.getDrawOrder());
}

void Ellipse::update()
{
    mShape.setPosition(mPosition + mSize * 0.5f + getLayerOffset() + getMapOffset());
    mShape.setRotation(mRotation);
    mShape.setPointCount(mPointCount);
    mShape.setOutlineThickness(2.f);
    const float pi = 3.141592654f;
    for (std::size_t i = 0; i < mShape.getPointCount(); i++)
    {
        float angle = i * 2 * pi / mShape.getPointCount() - pi / 2;
        float x = std::cos(angle) * mSize.x * 0.5f;
        float y = std::sin(angle) * mSize.y * 0.5f;
        mShape.setPoint(i, sf::Vector2f(x, y));
    }
}

void Ellipse::saveToNode(pugi::xml_node& object)
{
    if (!object) return;
    ObjectBase::saveToNode(object);
    object.append_child("ellipse");
}

void Ellipse::setColor(sf::Color const& color)
{
    sf::Color c = color;
    c.a = 32;
    mShape.setOutlineColor(color);
    mShape.setFillColor(c);
}

void Ellipse::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mVisible)
    {
        target.draw(mShape, states);
    }
}

}
