#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(600,600), "SFML works!");

    tmx::Map map;
    map.loadFromFile("sans-titre.tmx");
    map.setRenderObjects(true);

    sf::View view = window.getView();
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            {
                if (event.mouseWheelScroll.delta < 1)
                {
                    view.zoom(1.2f);
                }
                else
                {
                    view.zoom(0.8f);
                }
            }
        }

        sf::Vector2f mvt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            mvt.y--;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            mvt.y++;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            mvt.x--;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            mvt.x++;
        view.move(mvt * clock.restart().asSeconds() * 100.f);

        window.clear();
        window.setView(view);
        window.draw(map);
        window.display();
    }

    return 0;
}
