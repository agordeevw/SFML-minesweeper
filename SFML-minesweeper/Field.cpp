#pragma once

#include <chrono>
#include <random>

#include "Field.h"

namespace
{
	// Cell state can be encoded by 7 bits total.
	// 1-4th bits determine number of neighbouring cells with mines.
	// 5th bit is set when cell has mine in it.
	// 6th bit is set when cell is open.
	// 7th bit is set when cell is checked as having a mine.
	enum CellMasks : uint8_t { hasMine = 0b00010000, isOpen = 0b00100000, isChecked = 0b01000000, nearMines = 0b00001111 };
}

namespace game
{
	CellState Field::getCellState(uint32_t column, uint32_t row)
	{
		uint8_t cell = cells[width * row + column];

		if (!(cell & CellMasks::isOpen))
		{
			if (cell & CellMasks::isChecked)
				return CellState::Checked;
			else
				return CellState::Closed;
		}
		if (cell & CellMasks::hasMine)
			return CellState::OpenMine;
		else
			return CellState::OpenEmpty;
	}

	bool Field::hasMineAt(uint32_t column, uint32_t row) const
	{
		return cells[width * row + column] & CellMasks::hasMine;
	}

	bool Field::isOpen(uint32_t column, uint32_t row) const
	{
		return cells[width * row + column] & CellMasks::isOpen;
	}

	bool Field::isChecked(uint32_t column, uint32_t row) const
	{
		return cells[width * row + column] & CellMasks::isChecked;
	}

	uint32_t Field::nearMines(uint32_t column, uint32_t row) const
	{
		return cells[width * row + column] & CellMasks::nearMines;
	}

	void Field::swapCheck(uint32_t column, uint32_t row)
	{
		if (row < 0 || row > height - 1 || column < 0 || column > width - 1)
			return;

		uint32_t loc = width * row + column;

		// If cell isn't open
		if (!(cells[loc] & CellMasks::isOpen))
		{
			// And if it's checked, uncheck
			if (cells[loc] & CellMasks::isChecked)
			{
				cells[loc] &= ~CellMasks::isChecked;
				totalChecks--;
			}
				
			else
			{
				// otherwise check
				cells[loc] |= CellMasks::isChecked;
				totalChecks++;
			}
				
		}
	}

	void Field::openUncheckedMines()
	{
		for (auto &cell : cells)
		{
			if ((cell & CellMasks::hasMine) && !(cell & CellMasks::isChecked))
				cell |= CellMasks::isOpen;
		}
	}

	bool Field::open(uint32_t column, uint32_t row)
	{
		// If given position is out of bounds, do nothing
		if (row < 0 || row > height - 1 || column < 0 || column > width - 1)
			return false;

		uint32_t loc = width * row + column;

		// If the cell is not open
		if (
			!(cells[loc] & CellMasks::isOpen) &
		// And isn't checked, so we won't try to open possibly mined cell
			!(cells[loc] & CellMasks::isChecked)
			)
		{
			// Set isOpen bit for the cell
			cells[loc] |= CellMasks::isOpen;
			if (cells[loc] & CellMasks::hasMine)
			{
				return true;
			}
			else
			{
				// if no neighbouring cells have mines
				// every such cell can be safely opened
				if ((cells[loc] & CellMasks::nearMines) == 0)
				{
					open(column - 1, row - 1);
					open(column - 1, row);
					open(column - 1, row + 1);
					open(column, row - 1);
					open(column, row + 1);
					open(column + 1, row - 1);
					open(column + 1, row);
					open(column + 1, row + 1);

					return false;
				}
			}
		}
		return false;
	}

	bool Field::checkWinCondition() const
	{
		for (uint32_t row = 0; row < height; row++)
		{
			for (uint32_t col = 0; col < width; col++)
			{
				uint32_t loc = width * row + col;

				// If cell has mine
				if (cells[loc] & CellMasks::hasMine)
				{
					// But isn't checked
					if (!(cells[loc] & CellMasks::isChecked))
					{
						return false;
					}
				}
				else
				// If cell has no mine
				{
					// But isn't open
					if (!(cells[loc] & CellMasks::isOpen))
					{
						return false;
					}
				}
			}
		}

		return true;
	}

	void Field::reset(uint32_t startCol, uint32_t startRow, bool keepChecked)
	{
		totalMines = 0;
		totalChecks = 0;

		// Initialize random generator
		auto seed = std::chrono::system_clock::now().time_since_epoch().count();

		std::default_random_engine generator(seed);
		std::uniform_int_distribution<uint32_t> distr(0, height * width - 1);

		// Clear field while setting back checked flags
		for (uint32_t i = 0; i < height; i++)
		{
			for (uint32_t j = 0; j < width; j++)
			{
				auto loc = width * i + j;

				bool keepCellChecked = keepChecked && (cells[loc] & CellMasks::isChecked);

				// clear
				cells[loc] = 0;

				if (keepCellChecked)
				{
					totalChecks++;
					cells[loc] = CellMasks::isChecked;
				}
				
			}
		}

		// Randomly assign mines to cells
		while (totalMines < 99)
		{
			auto loc = distr(generator);
			auto col = loc % width;
			auto row = loc / width;

			if (col == startCol && row == startRow)
				continue;

			if (cells[loc] & CellMasks::hasMine)
			{
				continue;
			}
			else
			{
				cells[loc] |= CellMasks::hasMine;
				totalMines++;
			}
		}

		calcCellMineCount();
	}

	void Field::calcCellMineCount()
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (cells[width * i + j] & CellMasks::hasMine)
				{
					// Top-Left
					if (i != 0 && j != 0)
						cells[width * (i - 1) + (j - 1)]++;

					// Top
					if (i != 0)
						cells[width * (i - 1) + (j - 0)]++;

					// Top-Right
					if (i != 0 && j != width - 1)
						cells[width * (i - 1) + (j + 1)]++;

					// Left
					if (j != 0)
						cells[width * (i - 0) + (j - 1)]++;

					// Right
					if (j != width - 1)
						cells[width * (i - 0) + (j + 1)]++;

					// Bottom-Left
					if (i != height - 1 && j != 0)
						cells[width * (i + 1) + (j - 1)]++;

					// Bottom
					if (i != height - 1)
						cells[width * (i + 1) + (j - 0)]++;

					// Bottom-Right
					if (i != height - 1 && j != width - 1)
						cells[width * (i + 1) + (j + 1)]++;
				}
			}
		}
	}
}
