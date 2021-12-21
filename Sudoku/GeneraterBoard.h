#pragma once

#include "BoardBox.h"

namespace SudokuGenerator {
	using namespace System;

	public ref class Board {
	private:
	protected:
		array<BoardBox^, 2>^ board;
	public:
		Board() {
			board = gcnew array<BoardBox^, 2>(3, 3);
			initBoard();
		}

		Board(Board^ copy) {
			this->Board::Board();
			copyBoard(copy);
		}

		virtual bool canBePlacedAtPosition(int x, int y, int number) {
			int current = getNumber(x, y);
			if (current > 0) {
				return false;
			}

			for (int tableX = 0; tableX < 9; ++tableX) {
				if (tableX == x) {
					continue;
				}
				int tableNumber = getNumber(tableX, y);
				if (tableNumber == number) {
					return false;
				}
			}

			for (int tableY = 0; tableY < 9; ++tableY) {
				if (tableY == y) {
					continue;
				}
				int tableNumber = getNumber(x, tableY);
				if (tableNumber == number) {
					return false;
				}
			}
			return true;
		}

		virtual bool canPlaceAtSubGrid(int subX, int subY, int relativeX, int relativeY, int number) {
			BoardBox^ subGrid = board[subX, subY];
			int checkNumber = subGrid->getNumber(relativeX, relativeY);
			if (checkNumber == 0) {
				bool numberExist = false;
				for (int x = 0; x < 3; ++x) {
					for (int y = 0; y < 3; ++y) {
						if (x == relativeX && y == relativeY) {
							continue;
						}
						checkNumber = subGrid->getNumber(x, y);
						if (checkNumber == number) {
							numberExist = true;
							break;
						}
					}
					if (numberExist) {
						break;
					}
				}
				if (!numberExist) {
					return true;
				}
			}
			return false;
		}

		virtual void copyBoard(Board^ data) {
			for (int y = 0; y < 9; ++y) {
				for (int x = 0; x < 9; ++x) {
					setNumber(x, y, data->getNumber(x, y));
				}
			}
		}

		static Board^ fromString(String^ boardString) {
			Board^ board = gcnew Board();
			int charIndex = 0;
			for (int y = 0; y < 9; ++y) {
				for (int x = 0; x < 9; ++x) {
					int number = Convert::ToInt32(boardString[charIndex++]);
					board->setNumber(x, y, number);
				}
			}
			return board;
		}

		virtual int getNumber(int x, int y) {
			int row = (int)Math::Floor((double)y / 3);
			int col = (int)Math::Floor((double)x / 3);
			BoardBox^ boardBox = board[col, row];
			int minorX = x - col * 3;
			int minorY = y - row * 3;
			return boardBox->getNumber(minorX, minorY);
		}

		virtual void setNumber(int x, int y, int number) {
			int row = (int)Math::Floor((double)y / 3);
			int col = (int)Math::Floor((double)x / 3);
			BoardBox^ boardBox = board[col, row];
			int minorX = x - col * 3;
			int minorY = y - row * 3;
			boardBox->setNumber(minorX, minorY, number);
		}

		virtual String^ toString() {
			return toString(false);
		}

		virtual String^ toString(bool asGrid) {
			String^ output = "";
			for (int y = 0; y < 9; ++y) {
				for (int x = 0; x < 9; ++x) {
					output += getNumber(x, y);
				}
				if (asGrid) {
					output += Environment::NewLine;
				}
			}
			return output;
		}

		virtual void initBoard() {
			for (int x = 0; x < 3; ++x) {
				for (int y = 0; y < 3; ++y) {
					board[x, y] = gcnew BoardBox();
				}
			}
		}
	};
}