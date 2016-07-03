#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 512), "SFML works!");

	tmx::Map map;
	map.loadFromFile("ortho.tmx");
	map.saveToFile("temp.tmx");

	tmx::Map map2;
	map2.loadFromFile("temp.tmx");

	sf::RenderStates states;
	states.transform.translate(512,0);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(map);
        window.draw(map2,states);
        window.display();
    }

    return 0;
}
