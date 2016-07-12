#include "ObjectGroup.hpp"
#include "Ellipse.h"
#include "Polygon.h"
#include "Polyline.h"
#include "Object.h"
#include "Map.hpp"

namespace tmx
{

ObjectGroup::ObjectGroup(Map& map)
: mMap(map)
, mColor("#a0a0a4")
, mDrawOrder("topdown")
, mObjects()
{
}

LayerType ObjectGroup::getLayerType() const
{
    return tmx::EObjectGroup;
}

bool ObjectGroup::loadFromNode(pugi::xml_node const& layer)
{
    if (!layer)
    {
        return false;
    }
    for (pugi::xml_attribute attr = layer.first_attribute(); attr; attr = attr.next_attribute())
    {
        if (attr.name() == std::string("color"))
        {
            mColor = attr.as_string();
        }
        if (attr.name() == std::string("draworder"))
        {
            mDrawOrder = attr.as_string();
        }
    }
    if (!LayerBase::loadFromNode(layer))
    {
        return false;
    }
    for (pugi::xml_node object = layer.child("object"); object; object = object.next_sibling("object"))
    {
        if (object.child("ellipse"))
        {
            mObjects.push_back(new Ellipse(*this));
        }
        else if (object.child("polygon"))
        {
            mObjects.push_back(new Polygon(*this));
        }
        else if (object.child("polyline"))
        {
            mObjects.push_back(new Polyline(*this));
        }
        else
        {
            mObjects.push_back(new Object(*this));
        }
        mObjects.back()->loadFromNode(object);
    }
    update();
    return true;
}

void ObjectGroup::saveToNode(pugi::xml_node& layer)
{
    if (!layer)
    {
        return;
    }
    if (mColor != "#a0a0a4")
    {
        layer.append_attribute("color") = mColor.c_str();
    }
    if (mDrawOrder != "topdown")
    {
        layer.append_attribute("draworder") = mDrawOrder.c_str();
    }
    LayerBase::saveToNode(layer);
    sort("index");
    for (std::size_t i = 0; i < mObjects.size(); i++)
    {
        pugi::xml_node object = layer.append_child("object");
        mObjects[i]->saveToNode(object);
    }
}

void ObjectGroup::update()
{
    sf::Color color = getColor();
    for (std::size_t i = 0; i < mObjects.size(); i++)
    {
        mObjects[i]->setColor(color); // Apply color and opacity
        mObjects[i]->update();
    }
    sort(mDrawOrder);
}

void ObjectGroup::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mVisible)
    {
        for (std::size_t i = 0; i < mObjects.size(); i++)
        {
            target.draw(*mObjects.at(i), states);
        }
    }
}

sf::Color ObjectGroup::getColor() const
{
    sf::Color color = detail::fromString<sf::Color>(mColor);
    color.a = 255.f * mOpacity;
    return color;
}

void ObjectGroup::setColor(sf::Color const& color)
{
    setOpacity((float)color.a / 255.f);
    mColor = detail::toString<sf::Color>(color);
    for (std::size_t i = 0; i < mObjects.size(); i++)
    {
        mObjects[i]->setColor(color);
    }
}

const std::string& ObjectGroup::getDrawOrder() const
{
    return mDrawOrder;
}

void ObjectGroup::setDrawOrder(std::string const& order)
{
    if (mDrawOrder != order)
    {
        mDrawOrder = order;
        sort(mDrawOrder);
    }
}

void ObjectGroup::sort(std::string const& order)
{
    if (order == "index")
    {
        std::sort(mObjects.begin(),mObjects.end(),[](ObjectBase* a, ObjectBase* b)
        {
            if (a == nullptr || b == nullptr)
            {
                return false;
            }
            return (a->getId() < b->getId());
        });
    }
    else
    {
        std::sort(mObjects.begin(),mObjects.end(),[](ObjectBase* a, ObjectBase* b)
        {
            if (a == nullptr || b == nullptr)
            {
                return false;
            }
            return (a->getPosition().y < b->getPosition().y);
        });
    }
}

std::size_t ObjectGroup::getObjectCount() const
{
    return mObjects.size();
}

ObjectBase* ObjectGroup::getObject(std::size_t index)
{
    return mObjects[index];
}

ObjectType ObjectGroup::getObjectType(std::size_t index)
{
    return mObjects[index]->getObjectType();
}

void ObjectGroup::removeObject(unsigned int id)
{
    mObjects.erase(std::remove_if(mObjects.begin(),mObjects.end(),[&id](ObjectBase* o)->bool{return o->getId() == id;}), mObjects.end());
}

Map& ObjectGroup::getMap()
{
    return mMap;
}

} // namespace tmx
