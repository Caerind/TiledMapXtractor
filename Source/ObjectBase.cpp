#include "ObjectBase.h"
#include "ObjectGroup.hpp"

namespace tmx
{

ObjectBase::ObjectBase(ObjectGroup& group)
: mGroup(group)
, mId(0)
, mGid(0)
, mName("")
, mType("")
, mPosition({0.f, 0.f})
, mSize({0.f, 0.f})
, mRotation(0.f)
, mVisible(true)
, mFlippedHorizontally(false)
, mFlippedVertically(false)
, mFlippedDiagonally(false)
{
}

void ObjectBase::loadFromNode(pugi::xml_node const& object)
{
    if (!object)
    {
        return;
    }
    for (pugi::xml_attribute attr = object.first_attribute(); attr; attr = attr.next_attribute())
    {
        if (attr.name() == std::string("id"))
        {
            mId = attr.as_uint();
        }
        if (attr.name() == std::string("gid"))
        {
            mGid = attr.as_uint();
            detail::readFlip(mGid, mFlippedHorizontally, mFlippedVertically, mFlippedDiagonally);
        }
        if (attr.name() == std::string("name"))
        {
            mName = attr.as_string();
        }
        if (attr.name() == std::string("type"))
        {
            mType = attr.as_string();
        }
        if (attr.name() == std::string("x"))
        {
            mPosition.x = attr.as_float();
        }
        if (attr.name() == std::string("y"))
        {
            mPosition.y = attr.as_float();
        }
        if (attr.name() == std::string("width"))
        {
            mSize.x = attr.as_float();
        }
        if (attr.name() == std::string("height"))
        {
            mSize.y = attr.as_float();
        }
        if (attr.name() == std::string("rotation"))
        {
            mRotation = attr.as_float();
        }
        if (attr.name() == std::string("visible"))
        {
            mVisible = detail::fromString<bool>(attr.as_string());
        }
    }
    update();
    mGroup.sort(mGroup.getDrawOrder());
    loadProperties(object);
}

void ObjectBase::saveToNode(pugi::xml_node& object)
{
    if (!object)
    {
        return;
    }
    if (mId != 0)
    {
        object.append_attribute("id") = mId;
    }
    if (mGid != 0)
    {
        object.append_attribute("gid") = mGid;
        // TODO : O - Save Flip
    }
    if (mName != "")
    {
        object.append_attribute("name") = mName.c_str();
    }
    if (mType != "")
    {
        object.append_attribute("type") = mType.c_str();
    }
    object.append_attribute("x") = mPosition.x;
    object.append_attribute("y") = mPosition.y;
    if (mSize != sf::Vector2f())
    {
        object.append_attribute("width") = mSize.x;
        object.append_attribute("height") = mSize.y;
    }
    if (mRotation != 0.f)
    {
        object.append_attribute("rotation") = mRotation;
    }
    if (!mVisible)
    {
        object.append_attribute("visible") = "false";
    }
    saveProperties(object);
}

unsigned int ObjectBase::getId() const
{
    return mId;
}

unsigned int ObjectBase::getGid() const
{
    return mGid;
}

const std::string& ObjectBase::getName() const
{
    return mName;
}

const std::string& ObjectBase::getType() const
{
    return mType;
}

const sf::Vector2f& ObjectBase::getPosition() const
{
    return mPosition;
}

const sf::Vector2f& ObjectBase::getSize() const
{
    return mSize;
}

float ObjectBase::getRotation() const
{
    return mRotation;
}

bool ObjectBase::isVisible() const
{
    return mVisible;
}

void ObjectBase::setId(unsigned int id)
{
    mId = id;
    mGroup.sort(mGroup.getDrawOrder());
}

void ObjectBase::setGid(unsigned int gid)
{
    mGid = gid;
    update();
    mGroup.sort(mGroup.getDrawOrder());
}

void ObjectBase::setName(std::string const& name)
{
    mName = name;
}

void ObjectBase::setType(std::string const& type)
{
    mType = type;
}

void ObjectBase::setPosition(sf::Vector2f const& position)
{
    mPosition = position;
    update();
    mGroup.sort(mGroup.getDrawOrder());
}

void ObjectBase::setSize(sf::Vector2f const& size)
{
    mSize = size;
    update();
    mGroup.sort(mGroup.getDrawOrder());
}

void ObjectBase::setRotation(float rotation)
{
    mRotation = rotation;
    update();
    mGroup.sort(mGroup.getDrawOrder());
}

void ObjectBase::setVisible(bool visible)
{
    mVisible = visible;
}

const sf::Vector2f& ObjectBase::getLayerOffset() const
{
    return mGroup.getOffset();
}

void ObjectBase::setLayerOffset(sf::Vector2f const& offset)
{
    mGroup.setOffset(offset);
}

const sf::Vector2f& ObjectBase::getMapOffset() const
{
    return mGroup.getMap().getMapOffset();
}

void ObjectBase::setMapOffset(sf::Vector2f const& offset)
{
    mGroup.getMap().setMapOffset(offset);
}

void ObjectBase::update()
{
}

}
