#pragma once

#include <cstdint>
#include <vector>

namespace game {
	// Cell object 
	enum CellState {Closed, Checked, OpenMine, OpenEmpty};

	class Field
	{
		uint32_t width;
		uint32_t height;
		uint32_t totalMines;
		uint32_t totalChecks;
		std::vector<uint8_t> cells;
	
	public:
		// This constructor is used only when game is initialized
		// Field is reinitialized with specialized constructor later
		Field() {};

		Field(uint32_t width, uint32_t height) :
			width(width),
			height(height),
			cells(width * height)
		{
			generateMines(0.2f);
			calcCellMineCount();
		}

		Field(const Field& field) = default;

		// Returns width of the field.
		uint32_t getWidth() const { return width; }

		// Returns height of the field.
		uint32_t getHeight() const { return height; }

		// Returns total number of mines on the field.
		uint32_t getTotalMines() const { return totalMines; }

		// Returns total number of checked cells on the field.
		uint32_t getTotalChecks() const { return totalChecks; }

		CellState getCellState(uint32_t column, uint32_t row);

		// Returns true if cell specified by row and column position has mine.
		bool hasMineAt(uint32_t column, uint32_t row) const;

		// Return true if specified cell is open.
		bool isOpen(uint32_t column, uint32_t row) const;

		// Return true is specified cell is checked.
		bool isChecked(uint32_t column, uint32_t row) const;

		// Return number of mines close to specified cell.
		uint32_t nearMines(uint32_t column, uint32_t row) const;

		// Checks/unchecks specified cell.
		// Does nothing if specified cell is open.
		void swapCheck(uint32_t column, uint32_t row);

		// Opens all cells on the field
		void open();

		// Marks specified cell as open if it isn't open or isn't checked.
		// If cell has no mines near it, opens all neighbouring cells.
		// Returns true if specified cell has mine.
		bool open(uint32_t column, uint32_t row);

		// Returns true if all cells containing mines are checked
		// and every cell without mine is open.
		bool checkWinCondition() const;

	private:
		// Generates mines on the field with each mine probability of existence
		// equal to minesProba.
		void generateMines(float_t minesProba);

		// Calculates number of mines in close cells for every cell on field.
		void calcCellMineCount();
	};
}
