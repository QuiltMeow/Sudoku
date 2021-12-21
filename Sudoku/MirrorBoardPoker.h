#pragma once

#include "GeneraterBoard.h"
#include "BoardPoker.h"

namespace SudokuGenerator {
	using namespace System;

	public ref class MirrorBoardPoker : public BoardPoker {
	private:
	protected:
		int toRemove;
	public:
		MirrorBoardPoker(Board^ puzzleBoard) : BoardPoker(puzzleBoard) {
			toRemove = 52;
		}

		virtual void process() override {
			Random^ random = gcnew Random();
			while (toRemove > 0) {
				int randomX = random->Next(0, 9);
				int randomY = random->Next(0, 9);
				int testNumber = puzzleBoard->getNumber(randomX, randomY);
				if (testNumber == 0) {
					continue;
				}

				int mirrorX = 8 - randomX;
				int mirrorY = 8 - randomY;
				puzzleBoard->setNumber(randomX, randomY, 0);
				puzzleBoard->setNumber(mirrorX, mirrorY, 0);
				toRemove -= 2;
			}
		}
	};
}