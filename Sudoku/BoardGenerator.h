#pragma once

#include "GeneraterBoard.h"
#include "BoardPoker.h"
#include "MirrorBoardPoker.h"
#include "BoardSolver.h"
#include "GeneratorType.h"
#include "EWException.h"

namespace SudokuGenerator {
	using namespace System;
	using namespace System::Threading;

	public ref class BoardGenerator {
	private:
	protected:
		static const int MAX_EMPTY_SEARCH_ATTEMPT = 40;
		static Random^ random = gcnew Random();

		Board^ solutionBoard;
		Board^ puzzleBoard;

		virtual bool trySolutionGeneration() {
			for (int number = 1; number <= 9; ++number) {
				for (int x = 0; x < 3; ++x) {
					for (int y = 0; y < 3; ++y) {
						int tryTime = 0;
						bool findEmpty = false;
						int targetX, targetY;

						while (!findEmpty) {
							int randomX, randomY;
							Monitor::Enter(random);
							try
							{
								randomX = random->Next(0, 3);
								randomY = random->Next(0, 3);
							}
							finally
							{
								Monitor::Exit(random);
							}

							targetX = x * 3 + randomX;
							targetY = y * 3 + randomY;
							if (solutionBoard->canPlaceAtSubGrid(x, y, randomX, randomY, number)) {
								if (solutionBoard->canBePlacedAtPosition(targetX, targetY, number)) {
									findEmpty = true;
								}
							}
							++tryTime;
							if (!findEmpty && tryTime >= MAX_EMPTY_SEARCH_ATTEMPT) {
								return false;
							}
						}
						solutionBoard->setNumber(targetX, targetY, number);
					}
				}
			}
			return true;
		}
	public:
		BoardGenerator() {
		}

		virtual void generatePuzzleBoard() {
			generatePuzzleBoard(MIRROR);
		}

		virtual void generatePuzzleBoard(GeneratorType type) {
			bool solvable = false;
			while (!solvable) {
				Board^ testBoard = gcnew Board();
				testBoard->copyBoard(solutionBoard);
				if (type == MIRROR) {
					BoardPoker^ poker = gcnew MirrorBoardPoker(testBoard);
					poker->process();
				}
				else {
					throw gcnew EWException("無效的生成器");
				}

				BoardSolver^ solver = gcnew BoardSolver(testBoard);
				int solutionCount = solver->countSolution();
				if (solutionCount == 1) {
					solvable = true;
				}

				if (solvable) {
					puzzleBoard = testBoard;
				}
			}
		}

		virtual void generateSolutionBoard() {
			solutionBoard = gcnew Board();
			while (!trySolutionGeneration()) {
				solutionBoard->initBoard();
			}
		}

		virtual Board^ getPuzzleBoard() {
			return puzzleBoard;
		}

		virtual Board^ getSolutionBoard() {
			return solutionBoard;
		}
	};
}