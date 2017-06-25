#include <iostream>
#include <map>
#include <string>

#include <SFML/Graphics.hpp>

#include "Game.h"

void main()
{
	std::map<std::string, sf::Texture> textures;

	// Load all necessary textures
	textures["cellSheet"].loadFromFile("Resources/Textures/cells.png");
	textures["cellSheet"].generateMipmap();

	// Initialize window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweep!", sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	game::Game game(window, textures["cellSheet"], 10, 10);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();

		game.processInput();
		game.update();
		game.draw();

		window.display();
	}
}