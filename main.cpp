#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML works!");

	tmx::Map map;
	map.loadFromFile("loaded.tmx");
	map.saveToFile("saved.tmx");

	tmx::Map map2;
	map2.loadFromFile("saved.tmx");

	tmx::Map map3;
    tmx::ImageLayer::Ptr img = map3.createLayer<tmx::ImageLayer>("CalquePerso");
    img->setOffset({20.f, 20.f});
    img->setSource("ortho.png");
    img->loadImage();
	map3.saveToFile("created.tmx");


	sf::RenderStates states2;
	states2.transform.translate(512,0);

	sf::RenderStates states3;
	states3.transform.translate(256,256);

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
        window.draw(map2,states2);
        window.draw(map3,states3);
        window.display();
    }

    return 0;
}
