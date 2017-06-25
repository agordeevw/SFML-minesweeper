#pragma once

#include <SFML/Graphics.hpp>

#include "Field.h"

namespace game
{
	struct MouseArgs
	{
		// mouse position on window screen
		int mouseX, mouseY;
		// are mouse buttons pressed in a current frame
		bool leftButtonDown, rightButtonDown;
		// were mouse buttons held
		// this helps to determine mouse clicks
		bool leftButtonHold, rightButtonHold;
		// was mouse button clicked
		bool leftClick, rightClick;
	};

	class Game
	{
		enum GameState { Active, Menu, Win, Loss };
		enum CellTiles  { Closed, SemiClosed, Checked, ZeroMines, Mine = 12, RedMine };

		GameState state;

		sf::RenderWindow &window;

		sf::Sprite cellSprite;
		const unsigned int cellSize;

		float scaleFactor;
		float translationX;
		float translationY;

		MouseArgs mouseArgs;

		Field field;

		bool firstClick;

		int lostCellX, lostCellY;

	public:
		Game(sf::RenderWindow &window, sf::Texture &cellSheet, int fieldWidth, int fieldHeight) :
			state(GameState::Active),
			window(window),
			cellSprite(cellSheet),
			cellSize(cellSheet.getSize().y),
			field(fieldWidth, fieldHeight),
			firstClick(true),
			lostCellX(-1),
			lostCellY(-1)
		{
			init();
		}

		void processInput();
		void update();
		void draw();

	private:

		void init();

		void setTile(int cellTileCode);
		void drawField();
	};
}
