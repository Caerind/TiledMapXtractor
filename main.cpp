#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(512, 512), "SFML works!");

	Map map;
	map.loadFromFile("ortho.tmx");
	map.saveToFile("temp.tmx");
	map.loadFromFile("temp.tmx");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        map.render(window);
        window.display();
    }

    return 0;
}
