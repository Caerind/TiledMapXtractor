#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <iostream>
#include <sstream>
#include <unordered_map>

#include "pugixml.hpp"

class Properties
{
    public:
        Properties();

        void loadFromNode(pugi::xml_node& node);
        void saveToNode(pugi::xml_node& node);

        template <typename T>
        void setValue(std::string const& name, T const& value);

        std::string getValue(std::string const& name);
        int getValueInt(std::string const& name);
        float getValueFloat(std::string const& name);
        bool getValueBool(std::string const& name);

    protected:
        std::unordered_map<std::string,std::string> mProperites;
};

template <typename T>
void Properties::setValue(std::string const& name, T const& value)
{
    std::ostringstream oss;
    oss << value;
    mProperites[name] = oss.str();
}

#endif // PROPERTIES_HPP
