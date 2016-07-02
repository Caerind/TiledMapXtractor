#include "Properties.hpp"

Properties::Properties()
{
}

void Properties::loadFromNode(pugi::xml_node& node)
{
    if (!node)
    {
        std::cerr << "Invalid properties node" << std::endl;
        return;
    }
    for (pugi::xml_node property = node.child("property"); property; property = property.next_sibling("property"))
    {
        for (pugi::xml_attribute attr = property.first_attribute(); attr; attr = attr.next_attribute())
        {
            setValue<std::string>(attr.name(), attr.value());
        }
    }
}

void Properties::saveToNode(pugi::xml_node& node)
{
    if (!node)
    {
        std::cerr << "Invalid properites node" << std::endl;
    }
    if (mProperites.size() > 0)
    {
        pugi::xml_node properties = node.append_child("properties");
        for (auto itr = mProperites.begin(); itr != mProperites.end(); itr++)
        {
            pugi::xml_node property = properties.append_child("property");
            property.append_attribute("name") = itr->first.c_str();
            property.append_attribute("value") = itr->second.c_str();
        }
    }
}

std::string Properties::getValue(std::string const& name)
{
    auto itr = mProperites.find(name);
    if (itr != mProperites.end())
    {
        return itr->second;
    }
    return "";
}

int Properties::getValueInt(std::string const& name)
{
    int value;
    auto itr = mProperites.find(name);
    if (itr != mProperites.end())
    {
        std::istringstream iss(itr->second);
        iss >> value;
    }
    return value;
}

float Properties::getValueFloat(std::string const& name)
{
    float value;
    auto itr = mProperites.find(name);
    if (itr != mProperites.end())
    {
        std::istringstream iss(itr->second);
        iss >> value;
    }
    return value;
}

bool Properties::getValueBool(std::string const& name)
{
    bool value;
    auto itr = mProperites.find(name);
    if (itr != mProperites.end())
    {
        if (itr->second == "true")
            return true;
        if (itr->first == "false")
            return false;
        std::istringstream iss(itr->second);
        iss >> value;
    }
    return value;
}

