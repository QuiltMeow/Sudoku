#pragma once

#include "PlayerInformation.h"
#include "Util.h"

namespace Sudoku {
	using namespace System::Collections::Generic;
	using namespace System::IO;
	using namespace System::Windows::Forms;
	using namespace MapleUI;

	public ref class ScoreBoard {
	private:
	protected:
		static ScoreBoard^ instance; // 單例模式
		static const int RANK_LIMIT = 20; // 記分板上限
		static const int MAX_NAME_LENGTH = 20; // 名稱長度上限
		static String^ FILE_NAME = "score.ew"; // 記分板檔案
		List<PlayerInformation^>^ playerData; // 玩家資料

		ScoreBoard() { // 建構子
			playerData = gcnew List<PlayerInformation^>();
			load();
		}

		virtual void load() { // 從硬碟讀取記分板的資料
			if (!File::Exists(FILE_NAME)) {
				return;
			}
			FileStream^ fs = nullptr;
			BinaryReader^ br = nullptr;
			try {
				fs = gcnew FileStream(FILE_NAME, FileMode::Open, FileAccess::Read);
				br = gcnew BinaryReader(fs);

				System::Collections::Generic::IList<PlayerInformation^>^ readInfo = gcnew List<PlayerInformation^>();
				while (fs->Position != fs->Length) {
					String^ name = br->ReadString();
					if (name->Length > MAX_NAME_LENGTH) {
						throwFileError();
						return;
					}

					int score = br->ReadInt32();
					if (score < GameManager::getMinScore() || score > GameManager::getMaxScore()) {
						throwFileError();
						return;
					}

					int solveTime = br->ReadInt32();
					if (solveTime < 0) {
						throwFileError();
						return;
					}

					SudokuLevel solveLevel = integerToSudokuLevel(br->ReadInt32());
					readInfo->Add(gcnew PlayerInformation(name, score, solveTime, solveLevel));
				}

				for each (PlayerInformation ^ info in readInfo) {
					playerData->Add(info);
				}
			}
			catch (Exception ^ ex) {
				(gcnew MapleMessageBox("讀取記分板時發生例外狀況" + Environment::NewLine + ex->Message))->Show();
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

		virtual void save() { // 儲存記分板的資料
			FileStream^ fs = nullptr;
			BinaryWriter^ bw = nullptr;
			try {
				fs = gcnew FileStream(FILE_NAME, FileMode::Create, FileAccess::Write);
				bw = gcnew BinaryWriter(fs);
				for each (PlayerInformation ^ info in playerData) {
					bw->Write(info->getName());
					bw->Write(info->getScore());
					bw->Write(info->getSolveTime());
					bw->Write((int)info->getSolveLevel());
				}
			}
			catch (Exception ^ ex) {
				(gcnew MapleMessageBox("儲存記分板時發生例外狀況" + Environment::NewLine + ex->Message))->Show();
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

		virtual void rank() { // 進行排名
			playerData->Sort();
			if (playerData->Count > RANK_LIMIT) {
				playerData->RemoveRange(RANK_LIMIT, playerData->Count - RANK_LIMIT);
			}
		}
	public:
		static ScoreBoard^ getInstance() { // 取得實例
			if (instance == nullptr) {
				instance = gcnew ScoreBoard();
			}
			return instance;
		}

		static int getMaxNameLength() { // 取得名稱長度上限
			return MAX_NAME_LENGTH;
		}

		virtual bool canAdd(int score, int solveTime, SudokuLevel solveLevel) { // 確認成績是否可加入記分板
			if (playerData->Count < RANK_LIMIT) {
				return true;
			}
			PlayerInformation^ last = playerData[playerData->Count - 1];
			if (score > last->getScore()) {
				return true;
			}
			else if (score == last->getScore()) {
				if (solveTime < last->getSolveTime()) {
					return true;
				}
				else if (solveTime == last->getSolveTime()) {
					return solveLevel > last->getSolveLevel();
				}
			}
			return false;
		}

		virtual void addPlayer(PlayerInformation^ player) { // 新增一個玩家到記分板
			playerData->Add(player);
			rank();
			save();
		}

		virtual int removePlayer(String^ name) { // 從記分板移除一個玩家所有資料
			int ret = 0;
			for each (PlayerInformation ^ info in playerData) {
				if (info->getName()->Equals(name, StringComparison::OrdinalIgnoreCase)) {
					playerData->Remove(info);
					++ret;
				}
			}
			save();
			return ret;
		}

		virtual void clear() { // 清除記分板所有玩家
			playerData->Clear();
			save();
		}

		virtual System::Collections::Generic::IList<PlayerInformation^>^ getPlayerData() { // 列出記分板上所有玩家
			return playerData;
		}
	};
}