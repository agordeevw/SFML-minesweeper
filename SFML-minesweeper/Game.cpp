#include "Game.h"

void game::Game::processInput()
{
	mouseArgs.mouseX = sf::Mouse::getPosition(window).x;
	mouseArgs.mouseY = sf::Mouse::getPosition(window).y;

	mouseArgs.leftButtonDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	mouseArgs.rightButtonDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

	mouseArgs.leftClick = false;
	mouseArgs.rightClick = false;
		
	if (mouseArgs.leftButtonHold && !mouseArgs.leftButtonDown)
		mouseArgs.leftClick = true;

	if (mouseArgs.rightButtonHold && !mouseArgs.rightButtonDown)
		mouseArgs.rightClick = true;

	mouseArgs.leftButtonHold = mouseArgs.leftButtonDown;
	mouseArgs.rightButtonHold = mouseArgs.rightButtonDown;
}

void game::Game::update()
{
	uint32_t cellX = (mouseArgs.mouseX - translationX) / (scaleFactor * cellSize);
	uint32_t cellY = (mouseArgs.mouseY - translationY) / (scaleFactor * cellSize);

	if (state == GameState::Menu)
	{
		// wait for click
		// if click happened in the button area, generate field
		// go to active state
		return;
	}

	if (state == GameState::Active)
	{
		if (mouseArgs.leftClick)
		{
			if (field.open(cellX, cellY))
			{
				state = GameState::Loss;

				lostCellX = cellX;
				lostCellY = cellY;

				field.open();
			}
		}

		if (mouseArgs.rightClick)
		{
			field.swapCheck(cellX, cellY);
		}

		if (field.checkWinCondition())
		{
			state = GameState::Win;
		}

		return;
	}

	if (state == GameState::Loss || state == GameState::Win)
	{
		if (mouseArgs.leftClick || mouseArgs.rightClick)
		{
			field = Field(field.getWidth(), field.getHeight());
			firstClick = true;
			state = GameState::Active;
		}

		return;
	}
}

void game::Game::draw()
{
	drawField();
}



void game::Game::init()
{
	auto area = window.getSize();

	scaleFactor = 1.0f / std::max(std::max((float)field.getHeight() * cellSize / area.y, (float)field.getWidth() * cellSize / area.x), 1.0f);

	translationX = 0.5f * (area.x - cellSize * scaleFactor * field.getWidth());
	translationY = 0.5f * (area.y - cellSize * scaleFactor * field.getHeight());
}

void game::Game::setTile(int cellTileCode)
{
	cellSprite.setTextureRect(sf::IntRect((cellTileCode)* cellSize, 0, cellSize, cellSize));
}

void game::Game::drawField()
{
	for (int y = 0; y < field.getHeight(); y++)
	{
		for (int x = 0; x < field.getWidth(); x++)
		{
			cellSprite.setPosition(
				x * cellSize * scaleFactor + translationX,
				y * cellSize * scaleFactor + translationY
			);
			cellSprite.setScale(scaleFactor, scaleFactor);
			cellSprite.setColor(sf::Color(255, 255, 255));

			CellState cellState = field.getCellState(x, y);

			if (cellState == CellState::Closed)
			{
				if (state == GameState::Active && mouseArgs.leftButtonDown &&
					cellSprite.getGlobalBounds().contains(mouseArgs.mouseX, mouseArgs.mouseY)
					)
				{ setTile(CellTiles::SemiClosed); }
				else 
				{ setTile(CellTiles::Closed); }
			}

			if (cellState == CellState::Checked)
			{
				setTile(CellTiles::Checked);
			}

			if (cellState == CellState::OpenMine)
			{
				setTile(CellTiles::Mine);
				if (state == GameState::Loss && x == lostCellX && y == lostCellY)
				{
					cellSprite.setColor(sf::Color(220, 0, 0));
				}
			}

			if (cellState == CellState::OpenEmpty)
			{
				setTile(CellTiles::ZeroMines + field.nearMines(x, y));
			}

			if (state == GameState::Active)
			{
				if (cellState != CellState::OpenEmpty && cellSprite.getGlobalBounds().contains(mouseArgs.mouseX, mouseArgs.mouseY))
					cellSprite.setColor(sf::Color(150, 200, 255));
			}

			window.draw(cellSprite);
		}
	}
}