#include "Polygon.h"

namespace tmx
{

Polygon::Polygon(ObjectGroup& group)
: ObjectBase(group)
, mShape()
{
}

ObjectType Polygon::getObjectType() const
{
    return tmx::EPolygon;
}

void Polygon::loadFromNode(pugi::xml_node const& object)
{
    if (!object)
    {
        return;
    }
    ObjectBase::loadFromNode(object);
    pugi::xml_node polygon = object.child("polygon");
    if (!polygon)
    {
        std::cerr << "Polygon without polygon node" << std::endl;
        return;
    }
    pugi::xml_attribute attr = polygon.attribute("points");
    if (!attr)
    {
        std::cerr << "Polygon without points attribute" << std::endl;
        return;
    }
    std::string point;
    std::vector<sf::Vector2f> points;
    std::stringstream ss(attr.value());
    while (std::getline(ss, point, ' '))
    {
        points.push_back(detail::fromString<sf::Vector2f>(point));
    }
    mShape.setPointCount(points.size());
    for (std::size_t i = 0; i < points.size(); i++)
    {
        mShape.setPoint(i, points[i]);
    }
    update();
}

void Polygon::saveToNode(pugi::xml_node& object)
{
    if (!object)
    {
        return;
    }
    ObjectBase::saveToNode(object);
    std::string points;
    for (std::size_t i = 0; i < mShape.getPointCount(); i++)
    {
        points += detail::toString(mShape.getPoint(i)) + " ";
    }
    if (points.size() > 0)
    {
        points.pop_back(); // Remove the last space
    }
    object.append_child("polygon").append_attribute("points") = points.c_str();
}

void Polygon::update()
{
    mShape.setOutlineThickness(2.f);
    mShape.setPosition(mPosition);
    mShape.setRotation(mRotation);
}

void Polygon::addPoint(sf::Vector2f const& point)
{
    mShape.setPointCount(mShape.getPointCount() + 1);
    mShape.setPoint(mShape.getPointCount() - 1, point);
}

void Polygon::addPoint(sf::Vector2f const& point, std::size_t index)
{
    mShape.setPointCount(mShape.getPointCount() + 1);
    for (std::size_t i = mShape.getPointCount() - 1; i >= index; i--)
    {
        mShape.setPoint(i, mShape.getPoint(i-1));
    }
    mShape.setPoint(index, point);
}

sf::Vector2f Polygon::getPoint(std::size_t index) const
{
    return mShape.getPoint(index);
}

void Polygon::setPoint(std::size_t index, sf::Vector2f const& point)
{
    mShape.setPoint(index, point);
}

void Polygon::removePoint(std::size_t index)
{
    for (std::size_t i = index; i < mShape.getPointCount()-1; i++)
    {
        mShape.setPoint(i, mShape.getPoint(i+1));
    }
    if (index < mShape.getPointCount())
    {
        mShape.setPointCount(mShape.getPointCount()-1);
    }
}

void Polygon::setColor(sf::Color const& color)
{
    sf::Color c = color;
    c.a = 32;
    mShape.setOutlineColor(color);
    mShape.setFillColor(c);
}

void Polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mVisible)
    {
        target.draw(mShape, states);
    }
}

}
