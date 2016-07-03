#include "Polyline.h"

namespace tmx
{

Polyline::Polyline(ObjectGroup& group)
: ObjectBase(group)
, mPoints()
{
}

ObjectType Polyline::getObjectType() const
{
    return tmx::EPolyline;
}

void Polyline::loadFromNode(pugi::xml_node const& object)
{
    ObjectBase::loadFromNode(object);
    pugi::xml_node polyline = object.child("polyline");
    if (!polyline)
    {
        std::cerr << "Polyline without polygon node" << std::endl;
        return;
    }
    pugi::xml_attribute attr = polyline.attribute("points");
    if (!attr)
    {
        std::cerr << "Polyline without points attribute" << std::endl;
        return;
    }
    std::string point;
    std::stringstream ss(attr.value());
    while (std::getline(ss, point, ' '))
    {
        mPoints.push_back(detail::fromString<sf::Vector2f>(point));
    }
    update();
}

void Polyline::saveToNode(pugi::xml_node& object)
{
    ObjectBase::saveToNode(object);
    std::string points;
    for (std::size_t i = 0; i < mPoints.size(); i++)
    {
        points += detail::toString(mPoints[i]) + " ";
    }
    object.append_child("polyline").append_attribute("points") = points.c_str();
}

void Polyline::update()
{
    if (mPoints.size() > 1)
    {
        mShapes.resize(mPoints.size()-1);
        for (std::size_t i = 0; i < mShapes.size(); i++)
        {
            sf::Vector2f pos = mPoints[i];
            sf::Vector2f diff = mPoints[i+1] - pos;
            mShapes[i].setPosition(pos + mPosition);
            mShapes[i].setSize(sf::Vector2f(std::sqrt(diff.x * diff.x + diff.y * diff.y), 0.f));
            mShapes[i].setRotation(std::atan2(diff.y,diff.x) * 180.f / 3.141592654f);
            mShapes[i].setOutlineThickness(1.f);
        }
    }
}

void Polyline::addPoint(sf::Vector2f const& point, std::size_t index)
{
    mPoints.insert(mPoints.begin() + index, point);
    update();
}

sf::Vector2f Polyline::getPoint(std::size_t index) const
{
    return mPoints[index];
}

void Polyline::setPoint(std::size_t index, sf::Vector2f const& point)
{
    mPoints[index] = point;
    update();
}

void Polyline::removePoint(std::size_t index)
{
    mPoints.erase(mPoints.begin() + index);
    update();
}

void Polyline::setColor(sf::Color const& color)
{
    sf::Color c = color;
    c.a = 32;
    for (std::size_t i = 0; i < mShapes.size(); i++)
    {
        mShapes[i].setOutlineColor(color);
        mShapes[i].setFillColor(c);
    }
}

void Polyline::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mVisible)
    {
        states.transform.rotate(mRotation);
        for (std::size_t i = 0; i < mShapes.size(); i++)
        {
            target.draw(mShapes[i], states);
        }
    }
}

}
