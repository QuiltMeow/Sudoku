#pragma once

#include "GeneraterBoard.h"

namespace SudokuGenerator {
	public ref class BoardPoker abstract {
	private:
	protected:
		Board^ puzzleBoard;
	public:
		BoardPoker(Board^ puzzleBoard) {
			this->puzzleBoard = puzzleBoard;
		}

		virtual void process() abstract;
	};
}