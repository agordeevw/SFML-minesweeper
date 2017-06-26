#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <fstream>
#include <iomanip>

#include <SFML/Graphics.hpp>
#include <SOIL.h>

#include "cellsData.h"
#include "iconData.h"

#include "Game.h"

void main()
{
	if (cellsData == nullptr || iconData == nullptr)
	{
		throw std::exception("Image Data Headers aren't initialized! Use imageDataGen to do so.");

		return;
	}

	sf::Image cells;
	cells.create(20, 280, cellsData);

	sf::Image icon;
	icon.create(20, 20, iconData);
	
	sf::Texture cellSheet;
	cellSheet.loadFromImage(cells);


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