#include "ObjectGroup.hpp"

namespace tmx
{

namespace detail
{

ObjectBase::ObjectBase()
{
    mId = 0;
    mName = "";
    mType = "";
    mPosition = {0.f, 0.f};
    mSize = {0.f, 0.f};
    mGid = 0;
    mVisible = true;
}

void ObjectBase::loadFromNode(pugi::xml_node& object)
{
    if (!object)
    {
        std::cerr << "Invalid object node" << std::endl;
        return;
    }
    for (pugi::xml_attribute attr = object.first_attribute(); attr; attr = attr.next_attribute())
    {
        if (attr.name() == std::string("id")) mId = attr.as_uint();
        if (attr.name() == std::string("name")) mName = attr.as_string();
        if (attr.name() == std::string("type")) mType = attr.as_string();
        if (attr.name() == std::string("x")) mPosition.x = attr.as_float();
        if (attr.name() == std::string("y")) mPosition.y = attr.as_float();
        if (attr.name() == std::string("width")) mSize.x = attr.as_float();
        if (attr.name() == std::string("height")) mSize.y = attr.as_float();
        if (attr.name() == std::string("rotation")) mRotation = attr.as_float();
        if (attr.name() == std::string("gid")) mGid = attr.as_uint();
        if (attr.name() == std::string("visible")) mVisible = detail::fromString<bool>(attr.as_string());
    }
    loadProperties(object);
}

void ObjectBase::saveToNode(pugi::xml_node& object)
{
    if (!object)
    {
        std::cerr << "Invalid object node" << std::endl;
        return;
    }
    if (mId != 0)
        object.append_attribute("id") = mId;
    if (mName != "")
        object.append_attribute("name") = mName.c_str();
    if (mType != "")
        object.append_attribute("type") = mType.c_str();
    if (mPosition.x != 0.f)
        object.append_attribute("x") = mPosition.x;
    if (mPosition.y != 0.f)
        object.append_attribute("y") = mPosition.y;
    if (mSize.x != 0.f)
        object.append_attribute("width") = mSize.x;
    if (mSize.y != 0.f)
        object.append_attribute("height") = mSize.y;
    if (mRotation != 0.f)
        object.append_attribute("rotation") = mRotation;
    if (mGid != 0)
        object.append_attribute("gid") = mGid;
    if (!mVisible)
        object.append_attribute("visible") = false;
    saveProperties(object);
}

void ObjectBase::setColor(sf::Color const& color)
{
}

sf::Vector2f ObjectBase::getPosition() const
{
    return mPosition;
}

unsigned int ObjectBase::getId() const
{
    return mId;
}

} // namespace detail

Object::Object(Map* map) : mMap(map)
{
}

ObjectType Object::getType() const
{
    return tmx::ESimple;
}

void Object::loadFromNode(pugi::xml_node& object)
{
    detail::ObjectBase::loadFromNode(object);

    mShape.setSize(mSize);
    mShape.setRotation(mRotation);
    if (mGid != 0)
    {
        Tileset* tileset = mMap->getTileset(mGid);
        if (tileset == nullptr)
        {
            return;
        }
        mShape.setPosition(mPosition - sf::Vector2f(0.f, mSize.y));
        mShape.setTexture(&tileset->getTexture());
        mShape.setTextureRect(tileset->toRect(mGid));
    }
    else
    {
        mShape.setPosition(mPosition);
        mShape.setOutlineThickness(2.f);
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

Ellipse::Ellipse()
{
}

ObjectType Ellipse::getType() const
{
    return tmx::EEllipse;
}

void Ellipse::loadFromNode(pugi::xml_node& object)
{
    detail::ObjectBase::loadFromNode(object);

    mShape.setOutlineThickness(2.f);
    mShape.setPosition(mPosition + mSize * 0.5f);
    mShape.setRotation(mRotation);
    std::size_t count = 50;
    mShape.setPointCount(count);
    const float pi = 3.141592654f;
    for (std::size_t i = 0; i < count; i++)
    {
        float angle = i * 2 * pi / count - pi / 2;
        float x = std::cos(angle) * mSize.x * 0.5f;
        float y = std::sin(angle) * mSize.y * 0.5f;
        mShape.setPoint(i, sf::Vector2f(x, y));
    }
}

void Ellipse::saveToNode(pugi::xml_node& object)
{
    detail::ObjectBase::saveToNode(object);
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

Polygon::Polygon()
{
}

ObjectType Polygon::getType() const
{
    return tmx::EPolygon;
}

void Polygon::loadFromNode(pugi::xml_node& object)
{
    detail::ObjectBase::loadFromNode(object);
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
    mShape.setOutlineThickness(2.f);
    mShape.setPosition(mPosition);
    mShape.setRotation(mRotation);
}

void Polygon::saveToNode(pugi::xml_node& object)
{
    detail::ObjectBase::saveToNode(object);
    std::string points;
    for (std::size_t i = 0; i < mShape.getPointCount(); i++)
    {
        points += detail::toString(mShape.getPoint(i)) + " ";
    }
    if (points.size() > 0)
    {
        points.pop_back();
    }
    object.append_child("polygon").append_attribute("points") = points.c_str();
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

Polyline::Polyline()
{
}

ObjectType Polyline::getType() const
{
    return tmx::EPolyline;
}

void Polyline::loadFromNode(pugi::xml_node& object)
{
    detail::ObjectBase::loadFromNode(object);
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
    std::vector<sf::Vector2f> points;
    std::stringstream ss(attr.value());
    while (std::getline(ss, point, ' '))
        points.push_back(detail::fromString<sf::Vector2f>(point));
    if (points.size() > 1)
    {
        mShapes.resize(points.size()-1);
        for (std::size_t i = 0; i < mShapes.size(); i++)
        {
            sf::Vector2f pos = points[i];
            sf::Vector2f diff = points[i+1] - pos;
            mShapes[i].setPosition(pos);
            mShapes[i].setSize(sf::Vector2f(std::sqrt(diff.x * diff.x + diff.y * diff.y), 0.f));
            mShapes[i].setRotation(std::atan2(diff.y,diff.x) * 180.f / 3.141592654f);
            mShapes[i].setOutlineThickness(1.f);
        }
    }
}

void Polyline::saveToNode(pugi::xml_node& object)
{
    detail::ObjectBase::saveToNode(object);
    std::string points;
    for (std::size_t i = 0; i < mShapes.size(); i++)
        points += detail::toString(mShapes[i].getPosition()) + " ";
    if (mShapes.size() > 0)
    {
        float r = mShapes.back().getSize().x;
        float angle = mShapes.back().getRotation() * 3.141592654f / 180.f;
        points += detail::toString(mShapes.back().getPosition() + sf::Vector2f(r * std::cos(angle), r * std::sin(angle)));
    }
    object.append_child("polyline").append_attribute("points") = points.c_str();
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
        states.transform.translate(mPosition);
        states.transform.rotate(mRotation);
        for (std::size_t i = 0; i < mShapes.size(); i++)
            target.draw(mShapes[i], states);
    }
}

ObjectGroup::ObjectGroup(Map* map)
{
    mMap = map;
    mColor = "#a0a0a4";
    mDrawOrder = "topdown";
}

LayerType ObjectGroup::getType() const
{
    return tmx::EObjectGroup;
}

bool ObjectGroup::loadFromNode(pugi::xml_node& layer)
{
    if (!detail::LayerBase::loadFromNode(layer))
    {
        return false;
    }
    for (pugi::xml_attribute attr = layer.first_attribute(); attr; attr = attr.next_attribute())
    {
        if (attr.name() == std::string("color")) mColor = attr.as_string();
        if (attr.name() == std::string("draworder")) mDrawOrder = attr.as_string();
    }
    sf::Color color = detail::stringToColor(mColor);
    color.a = 255.f * mOpacity;
    for (pugi::xml_node object = layer.child("object"); object; object = object.next_sibling("object"))
    {
        if (object.child("ellipse"))
        {
            mObjects.push_back(new Ellipse());
        }
        else if (object.child("polygon"))
        {
            mObjects.push_back(new Polygon());
        }
        else if (object.child("polyline"))
        {
            mObjects.push_back(new Polyline());
        }
        else
        {
            mObjects.push_back(new Object(mMap));
        }
        mObjects.back()->loadFromNode(object);
        mObjects.back()->setColor(color);
    }
    sort(mDrawOrder);
    return true;
}

void ObjectGroup::saveToNode(pugi::xml_node& layer)
{
    detail::LayerBase::saveToNode(layer);
    sort("index");
    for (std::size_t i = 0; i < mObjects.size(); i++)
    {
        pugi::xml_node object = layer.append_child("object");
        mObjects[i]->saveToNode(object);
    }
    if (mColor != "#a0a0a4") layer.append_attribute("color") = mColor.c_str();
    if (mDrawOrder != "topdown") layer.append_attribute("draworder") = mDrawOrder.c_str();
}

void ObjectGroup::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mVisible)
    {
        states.transform.translate(mOffset);
        for (std::size_t i = 0; i < mObjects.size(); i++)
        {
            target.draw(*mObjects.at(i), states);
        }
    }
}

void ObjectGroup::sort(std::string const& order)
{
    if (order == "index")
    {
        std::sort(mObjects.begin(),mObjects.end(),[](detail::ObjectBase* a, detail::ObjectBase* b)
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
        std::sort(mObjects.begin(),mObjects.end(),[](detail::ObjectBase* a, detail::ObjectBase* b)
        {
            if (a == nullptr || b == nullptr)
            {
                return false;
            }
            return (a->getPosition().y < b->getPosition().y);
        });
    }
}

} // namespace tmx
