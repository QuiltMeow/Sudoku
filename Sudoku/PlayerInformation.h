#pragma once

#include "SudokuLevel.h"

namespace Sudoku {
	using namespace System;

	public ref class PlayerInformation : public IComparable<PlayerInformation^> {
	private:
	protected:
		String^ name; // �W�r
		int score; // ����
		int solveTime; // ���D�ɶ�
		SudokuLevel solveLevel; // ���D����
	public:
		PlayerInformation(String^ name, int score, int solveTime, SudokuLevel solveLevel) { // �غc�l
			this->name = name;
			this->score = score;
			this->solveTime = solveTime;
			this->solveLevel = solveLevel;
		}

		virtual String^ getName() { // ���o���a�W�r
			return name;
		}

		virtual int getScore() { // ���o���a����
			return score;
		}

		virtual int getSolveTime() { // ���o���a���D�ɶ�
			return solveTime;
		}

		virtual SudokuLevel getSolveLevel() { // ���o���a���D����
			return solveLevel;
		}

		virtual int CompareTo(PlayerInformation^ another) override { // �����L���a����
			int ret = another->score.CompareTo(score);
			if (ret != 0) {
				return ret;
			}

			ret = ((int)another->solveLevel).CompareTo((int)solveLevel);
			if (ret != 0) {
				return ret;
			}

			ret = solveTime.CompareTo(another->solveTime);
			if (ret != 0) {
				return ret;
			}
			return name->CompareTo(another->name);
		}
	};
}