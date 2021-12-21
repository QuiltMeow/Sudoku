#pragma once

namespace SudokuGenerator {
	public ref class BoardBox {
	private:
	protected:
		array<int, 2>^ grid;
	public:
		BoardBox() {
			grid = gcnew array<int, 2>(3, 3);
			wipeGrid();
		}

		virtual int getNumber(int x, int y) {
			return grid[x, y];
		}

		virtual void setNumber(int x, int y, int number) {
			grid[x, y] = number;
		}

		virtual void wipeGrid() {
			for (int x = 0; x < 3; ++x) {
				for (int y = 0; y < 3; ++y) {
					grid[x, y] = 0;
				}
			}
		}
	};
}