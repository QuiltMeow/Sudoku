#pragma once

#include "GeneratorInformation.h"
#include "BoardGenerator.h"
#include "LogEvent.h"

namespace SudokuGenerator {
	using namespace System;

	public ref class SudokuWorker {
	private:
	protected:
		int workerId;
		GeneratorInformation^ info;
	public:
		SudokuWorker(GeneratorInformation^ info) {
			workerId = -1;
			this->info = info;
		}

		SudokuWorker(int workerId, GeneratorInformation^ info) {
			this->workerId = workerId;
			this->info = info;
		}

		virtual void work() {
			bool continueWork = true;
			int max = info->getMaxPuzzle();
			while (continueWork) {
				BoardGenerator^ generator = gcnew BoardGenerator();
				generator->generateSolutionBoard();
				generator->generatePuzzleBoard();

				int count = info->addPuzzle(generator->getSolutionBoard()->toString(), generator->getPuzzleBoard()->toString());
				LogEvent::writeLog("輸出結果 " + count + " : " + generator->getSolutionBoard()->toString());
				if (count >= max) {
					continueWork = false;
				}
			}
			if (workerId != -1) {
				LogEvent::writeLog("執行緒 " + workerId + " 作業完成");
			}
		}
	};
}