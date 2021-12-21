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
		static GameManager^ instance; // ��ҼҦ�
		static String^ GAME_SAVE_FILE = "record.ew"; // �C�������ɮ�
		int solveTime, solveStep; // ���D�ɶ� / �B��

		SudokuTable^ table; // �ƿW��
		GameStatus status; // �C�����A
		SudokuLevel level; // ����

		GameManager() { // �غc�l
			cleanUp();
			level = EASY;
		}
	public:
		static GameManager^ getInstance() { // ���o���
			if (instance == nullptr) {
				instance = gcnew GameManager();
			}
			return instance;
		}

		static String^ getRecordFileName() { // ���o�C�������ɮצW��
			return GAME_SAVE_FILE;
		}

		virtual void newGame() { // �s�C��
			cleanUp();
			try {
				table = gcnew SudokuTable(level);
				status = PLAYING;
			}
			catch (Exception ^ ex) {
				(gcnew MapleMessageBox("�o�ͨҥ~���p" + Environment::NewLine + ex->Message))->Show();
			}
		}

		virtual void togglePause() { // �~�� / �Ȱ�
			if (status == PLAYING) {
				status = PAUSE;
			}
			else {
				status = PLAYING;
			}
		}

		virtual void giveUp() { // ���
			table->copySolve();
			status = GAME_OVER;
		}

		virtual void cleanUp() { // �M���C�����A
			solveTime = solveStep = 0;
			status = GAME_OVER;
		}

		virtual void complete() { // ����
			status = GAME_OVER;
		}

		static bool hasSave() { // �O�_�s�b���e����
			return File::Exists(GAME_SAVE_FILE);
		}

		virtual void loadGame() { // Ū���������C��
			FileStream^ fs = nullptr;
			BinaryReader^ br = nullptr;
			try {
				fs = gcnew FileStream(GAME_SAVE_FILE, FileMode::Open, FileAccess::Read);
				br = gcnew BinaryReader(fs);

				// Ū�����q
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

				// ��J���q
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
				(gcnew MapleMessageBox("Ū���C�������ɵo�ͨҥ~���p" + Environment::NewLine + ex->Message))->Show();
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

		virtual void saveGame() { // �x�s�C��
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
				(gcnew MapleMessageBox("�x�s�C�������ɵo�ͨҥ~���p : " + ex->Message))->Show();
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

		virtual int getPlayingTime() { // ���o���a���D�ɶ�
			return solveTime;
		}

		virtual void gameTimerTick() { // �p�ɾ��p�ɨ禡
			++solveTime;
		}

		virtual SudokuTable^ getTable() { // ���o�ثe�ƿW��
			return table;
		}

		virtual GameStatus getStatus() { // ���o�C�����A
			return status;
		}

		virtual int getScore() { // ���o�ثe����
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

		static int getMinScore() { // ���o�̤p����
			return 1000;
		}

		static int getMaxScore() { // ���o�̤j����
			return 4000 + 5000 + 5000 + SudokuTable::getMaxHintQuota() * 100;
		}

		virtual int getSolveStep() { // ���o���D�B��
			return solveStep;
		}

		virtual void addSolveStep() { // �������D�B��
			++solveStep;
		}

		virtual SudokuLevel getLevel() { // ���o����
			return level;
		}

		virtual void changeLevel(SudokuLevel level) { // �������׫�C�����m
			this->level = level;
			newGame();
		}
	};
}