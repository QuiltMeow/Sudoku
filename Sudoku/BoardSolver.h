#pragma once

#include "GeneraterBoard.h"

namespace SudokuGenerator {
	using namespace System;
	using namespace System::Collections::Generic;

	public ref class BoardSolver {
	private:
	protected:
		Board^ gameBoard;
		int solutionCount;
		IList<Tuple<int, int>^>^ emptyCell;
	public:
		BoardSolver(Board^ partialBoard) {
			gameBoard = partialBoard;
		}

		virtual bool canBeSolve() {
			return countSolution() > 0;
		}

		virtual int countSolution() {
			solutionCount = 0;
			emptyCell = gcnew List<Tuple<int, int>^>();

			for (int y = 0; y < 9; ++y) {
				for (int x = 0; x < 9; ++x) {
					int number = gameBoard->getNumber(x, y);
					if (number == 0) {
						Tuple<int, int>^ empty = gcnew Tuple<int, int>(x, y);
						emptyCell->Add(empty);
					}
				}
			}

			if (emptyCell->Count <= 0) {
				return solutionCount;
			}
			tryPoint(0);
			return solutionCount;
		}

		virtual void tryPoint(int index) {
			Tuple<int, int>^ point = emptyCell[index];
			int x = point->Item1;
			int y = point->Item2;
			for (int number = 1; number <= 9; ++number) {
				if (gameBoard->canBePlacedAtPosition(x, y, number)) {
					int subX = (int)Math::Floor((double)x / 3);
					int subY = (int)Math::Floor((double)y / 3);
					int relativeX = x - subX * 3;
					int relativeY = y - subY * 3;
					if (gameBoard->canPlaceAtSubGrid(subX, subY, relativeX, relativeY, number)) {
						gameBoard->setNumber(x, y, number);
						if (index == emptyCell->Count - 1) {
							++solutionCount;
						}
						else {
							tryPoint(index + 1);
						}
						gameBoard->setNumber(x, y, 0);
					}
				}
			}
			gameBoard->setNumber(x, y, 0);
		}
	};
}