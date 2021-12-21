#pragma once

#include "GameStatus.h"
#include "SudokuLevel.h"
#include "SudokuTable.h"
#include "SudokuLocation.h"
#include "Util.h"

namespace Sudoku {
	using namespace System::Collections::Generic;
	using namespace MapleUI;

	public ref class GameManager {
	private:
	protected:
		static GameManager^ instance; // 單例模式
		static String^ GAME_SAVE_FILE = "record.ew"; // 遊戲紀錄檔案
		int solveTime, solveStep; // 解題時間 / 步數

		SudokuTable^ table; // 數獨表
		GameStatus status; // 遊戲狀態
		SudokuLevel level; // 難度

		GameManager() { // 建構子
			cleanUp();
			level = EASY;
		}
	public:
		static GameManager^ getInstance() { // 取得實例
			if (instance == nullptr) {
				instance = gcnew GameManager();
			}
			return instance;
		}

		static String^ getRecordFileName() { // 取得遊戲紀錄檔案名稱
			return GAME_SAVE_FILE;
		}

		virtual void newGame() { // 新遊戲
			cleanUp();
			try {
				table = gcnew SudokuTable(level);
				status = PLAYING;
			}
			catch (Exception ^ ex) {
				(gcnew MapleMessageBox("發生例外狀況" + Environment::NewLine + ex->Message))->Show();
			}
		}

		virtual void togglePause() { // 繼續 / 暫停
			if (status == PLAYING) {
				status = PAUSE;
			}
			else {
				status = PLAYING;
			}
		}

		virtual void giveUp() { // 放棄
			table->copySolve();
			status = GAME_OVER;
		}

		virtual void cleanUp() { // 清除遊戲狀態
			solveTime = solveStep = 0;
			status = GAME_OVER;
		}

		virtual void complete() { // 完成
			status = GAME_OVER;
		}

		static bool hasSave() { // 是否存在之前紀錄
			return File::Exists(GAME_SAVE_FILE);
		}

		virtual void loadGame() { // 讀取未完成遊戲
			FileStream^ fs = nullptr;
			BinaryReader^ br = nullptr;
			try {
				fs = gcnew FileStream(GAME_SAVE_FILE, FileMode::Open, FileAccess::Read);
				br = gcnew BinaryReader(fs);

				// 讀取階段
				int readSolveTime = br->ReadInt32();
				int readSolveStep = br->ReadInt32();
				if (readSolveTime < 0 || readSolveStep < 0) {
					throwFileError();
					return;
				}

				SudokuLevel readLevel = integerToSudokuLevel(br->ReadInt32());
				int readHint = br->ReadInt32();
				if (readHint < 0 || readHint > SudokuTable::getMaxHintQuota()) {
					throwFileError();
					return;
				}

				bool readShowError = br->ReadBoolean();
				String^ readGameTable = br->ReadString();
				String^ readOriginTable = br->ReadString();
				String^ readSolveTable = br->ReadString();
				String^ readMarkTable = br->ReadString();
				String^ readHintTable = br->ReadString();
				String^ readErrorTable = br->ReadString();
				if (!checkRawIntegerTable(readGameTable, false)
					|| !checkRawIntegerTable(readOriginTable, false)
					|| !checkRawIntegerTable(readSolveTable, true)
					|| !checkRawIntegerTable(readMarkTable, false)
					|| !checkRawBooleanTable(readHintTable)
					|| !checkRawBooleanTable(readErrorTable)) {
					throwFileError();
					return;
				}

				int row = br->ReadInt32();
				int column = br->ReadInt32();
				if (!(row == -1 && column == -1) && (row < 0 || row > 8 || column < 0 || column > 8)) {
					throwFileError();
					return;
				}

				System::Collections::Generic::IList<SudokuLocation^>^ readHistory = gcnew List<SudokuLocation^>();
				while (fs->Position != fs->Length) {
					row = br->ReadInt32();
					column = br->ReadInt32();
					int value = br->ReadInt32();
					if (row < 0 || row > 8 || column < 0 || column > 8 || value < 0 || value > 9) {
						throwFileError();
						return;
					}
				}

				// 填入階段
				solveTime = readSolveTime;
				solveStep = readSolveStep;
				status = PAUSE;
				level = readLevel;

				table = gcnew SudokuTable();
				table->setRemainingHint(readHint);
				showError = readShowError;

				SudokuTable::fillIntegerTable(table->getGameTable(), readGameTable);
				SudokuTable::fillIntegerTable(table->getOriginTable(), readOriginTable);
				SudokuTable::fillIntegerTable(table->getSolveTable(), readSolveTable);
				SudokuTable::fillIntegerTable(table->getMarkTable(), readMarkTable);
				SudokuTable::fillBooleanTable(table->getHintTable(), readHintTable);
				SudokuTable::fillBooleanTable(table->getErrorTable(), readErrorTable);

				table->setCurrentLocation(row, column);
				for each (SudokuLocation ^ location in readHistory) {
					table->addHistory(location->getRow(), location->getColumn(), location->getValue());
				}
			}
			catch (Exception ^ ex) {
				(gcnew MapleMessageBox("讀取遊戲紀錄時發生例外狀況" + Environment::NewLine + ex->Message))->Show();
			}
			finally {
				if (br != nullptr) {
					br->Close();
				}
				if (fs != nullptr) {
					fs->Close();
				}
			}
		}

		virtual void saveGame() { // 儲存遊戲
			FileStream^ fs = nullptr;
			BinaryWriter^ bw = nullptr;
			try {
				fs = gcnew FileStream(GAME_SAVE_FILE, FileMode::Create, FileAccess::Write);
				bw = gcnew BinaryWriter(fs);

				bw->Write(solveTime);
				bw->Write(solveStep);
				bw->Write((int)level);

				bw->Write(table->getRemainingHint());
				bw->Write(showError);

				bw->Write(table->getGameTableString());
				bw->Write(table->getOriginTableString());
				bw->Write(table->getSolveTableString());
				bw->Write(table->getMarkTableString());
				bw->Write(table->getHintTableString());
				bw->Write(table->getErrorTableString());

				SudokuLocation^ location = table->getCurrentLocation();
				bw->Write(location->getRow());
				bw->Write(location->getColumn());

				System::Collections::Generic::IList<SudokuLocation^>^ history = table->getHistory();
				for each (SudokuLocation ^ log in history) {
					bw->Write(log->getRow());
					bw->Write(log->getColumn());
					bw->Write(log->getValue());
				}
			}
			catch (Exception ^ ex) {
				(gcnew MapleMessageBox("儲存遊戲紀錄時發生例外狀況 : " + ex->Message))->Show();
			}
			finally {
				if (bw != nullptr) {
					bw->Close();
				}
				if (fs != nullptr) {
					fs->Close();
				}
			}
		}

		virtual int getPlayingTime() { // 取得玩家解題時間
			return solveTime;
		}

		virtual void gameTimerTick() { // 計時器計時函式
			++solveTime;
		}

		virtual SudokuTable^ getTable() { // 取得目前數獨表
			return table;
		}

		virtual GameStatus getStatus() { // 取得遊戲狀態
			return status;
		}

		virtual int getScore() { // 取得目前分數
			int solveScore = 5000 - solveTime * 3;
			if (solveScore < 0) {
				solveScore = 0;
			}

			int stepScore = 5000 - (solveStep - getBlankByLevel(level)) * 35;
			if (stepScore < 0) {
				stepScore = 0;
			}
			return ((int)level + 1) * 1000 + solveScore + stepScore + table->getRemainingHint() * 100;
		}

		static int getMinScore() { // 取得最小分數
			return 1000;
		}

		static int getMaxScore() { // 取得最大分數
			return 4000 + 5000 + 5000 + SudokuTable::getMaxHintQuota() * 100;
		}

		virtual int getSolveStep() { // 取得解題步數
			return solveStep;
		}

		virtual void addSolveStep() { // 紀錄解題步數
			++solveStep;
		}

		virtual SudokuLevel getLevel() { // 取得難度
			return level;
		}

		virtual void changeLevel(SudokuLevel level) { // 改變難度後遊戲重置
			this->level = level;
			newGame();
		}
	};
}