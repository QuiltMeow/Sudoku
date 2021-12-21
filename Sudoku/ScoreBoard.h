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
		static ScoreBoard^ instance; // ��ҼҦ�
		static const int RANK_LIMIT = 20; // �O���O�W��
		static const int MAX_NAME_LENGTH = 20; // �W�٪��פW��
		static String^ FILE_NAME = "score.ew"; // �O���O�ɮ�
		List<PlayerInformation^>^ playerData; // ���a���

		ScoreBoard() { // �غc�l
			playerData = gcnew List<PlayerInformation^>();
			load();
		}

		virtual void load() { // �q�w��Ū���O���O�����
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
				(gcnew MapleMessageBox("Ū���O���O�ɵo�ͨҥ~���p" + Environment::NewLine + ex->Message))->Show();
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

		virtual void save() { // �x�s�O���O�����
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
				(gcnew MapleMessageBox("�x�s�O���O�ɵo�ͨҥ~���p" + Environment::NewLine + ex->Message))->Show();
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

		virtual void rank() { // �i��ƦW
			playerData->Sort();
			if (playerData->Count > RANK_LIMIT) {
				playerData->RemoveRange(RANK_LIMIT, playerData->Count - RANK_LIMIT);
			}
		}
	public:
		static ScoreBoard^ getInstance() { // ���o���
			if (instance == nullptr) {
				instance = gcnew ScoreBoard();
			}
			return instance;
		}

		static int getMaxNameLength() { // ���o�W�٪��פW��
			return MAX_NAME_LENGTH;
		}

		virtual bool canAdd(int score, int solveTime, SudokuLevel solveLevel) { // �T�{���Z�O�_�i�[�J�O���O
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

		virtual void addPlayer(PlayerInformation^ player) { // �s�W�@�Ӫ��a��O���O
			playerData->Add(player);
			rank();
			save();
		}

		virtual int removePlayer(String^ name) { // �q�O���O�����@�Ӫ��a�Ҧ����
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

		virtual void clear() { // �M���O���O�Ҧ����a
			playerData->Clear();
			save();
		}

		virtual System::Collections::Generic::IList<PlayerInformation^>^ getPlayerData() { // �C�X�O���O�W�Ҧ����a
			return playerData;
		}
	};
}