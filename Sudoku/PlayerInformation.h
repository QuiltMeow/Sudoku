#pragma once

#include "SudokuLevel.h"

namespace Sudoku {
	using namespace System;

	public ref class PlayerInformation : public IComparable<PlayerInformation^> {
	private:
	protected:
		String^ name; // 名字
		int score; // 分數
		int solveTime; // 解題時間
		SudokuLevel solveLevel; // 解題難度
	public:
		PlayerInformation(String^ name, int score, int solveTime, SudokuLevel solveLevel) { // 建構子
			this->name = name;
			this->score = score;
			this->solveTime = solveTime;
			this->solveLevel = solveLevel;
		}

		virtual String^ getName() { // 取得玩家名字
			return name;
		}

		virtual int getScore() { // 取得玩家分數
			return score;
		}

		virtual int getSolveTime() { // 取得玩家解題時間
			return solveTime;
		}

		virtual SudokuLevel getSolveLevel() { // 取得玩家解題難度
			return solveLevel;
		}

		virtual int CompareTo(PlayerInformation^ another) override { // 比較其他玩家分數
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