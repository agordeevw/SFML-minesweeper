#include <SFML/Graphics.hpp>
#include <SOIL.h>

#include "cellSheetData.h"
#include "Game.h"

void main()
{
	sf::Image image;
	image.create(260, 20, pixelData);

	sf::Image icon;
	icon.create(20, 20, sf::Color(255, 255, 255, 255));
	
	sf::Texture cellSheet;
	cellSheet.loadFromImage(image);

	// Initialize window
	sf::RenderWindow window(sf::VideoMode(600, 400), "Minesweep!", sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	window.setIcon(20, 20, icon.getPixelsPtr());

	game::Game game(window, cellSheet, 30, 20);

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