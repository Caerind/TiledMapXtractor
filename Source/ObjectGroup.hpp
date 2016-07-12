#ifndef TMX_OBJECTGROUP_HPP
#define TMX_OBJECTGROUP_HPP

#include "Map.hpp"
#include "Utils.hpp"
#include "ObjectBase.h"

namespace tmx
{

class Map;
class ObjectGroup : public LayerBase
{
    public:
        ObjectGroup(Map& map);

        LayerType getLayerType() const;

        bool loadFromNode(pugi::xml_node const& layer);
        void saveToNode(pugi::xml_node& layer);

        void update();

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

        sf::Color getColor() const;
        void setColor(sf::Color const& color);

        const std::string& getDrawOrder() const;
        void setDrawOrder(std::string const& order);

        void sort(std::string const& order = "topdown");

        std::size_t getObjectCount() const;
        ObjectBase* getObject(std::size_t index);
        ObjectType getObjectType(std::size_t index);
        template <typename T>
        T* getObject(std::size_t index);
        template <typename T>
        T* createObject(unsigned int id);
        void removeObject(unsigned int id);

        Map& getMap();

    protected:
        Map& mMap;
        std::string mColor;
        std::string mDrawOrder;
        std::vector<ObjectBase*> mObjects;
};

template <typename T>
T* ObjectGroup::getObject(std::size_t index)
{
    return static_cast<T*>(mObjects[index]);
}

template <typename T>
T* ObjectGroup::createObject(unsigned int id)
{
    if (id != 0)
    {
        if (std::find_if(mObjects.begin(),mObjects.end(),[&id](ObjectBase* obj) -> bool { return (obj->getId() >= id);}) == mObjects.end())
        {
            T* p = new T(*this);
            p->setId(id);
            p->setColor(getColor());
            mObjects.push_back(p);
            sort(mDrawOrder);
            return p;
        }
    }
    return nullptr;
}

} // namespace tmx

#endif // TMX_OBJECTGROUP_HPP
