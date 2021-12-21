#pragma once

#include "SudokuLocation.h"
#include "SudokuLevel.h"
#include "EWException.h"
#include "Util.h"

namespace Sudoku {
	using namespace System;
	using namespace System::Collections::Generic;
	using namespace System::IO;
	using namespace System::Windows::Forms;
	using namespace System::Text;

	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;
	using namespace System::Drawing::Text;

	public ref class SudokuTable {
	private:
	protected:
		static const int MAX_HINT_QUOTA = 5; // �C���̦h���ܦ���
		static String^ DIRECTORY = "problem"; // �D�ئs���Ƨ�
		static Random^ random = gcnew Random(); // �H���Ʋ��;�

		int remainingHint; // �Ѿl���ܦ���
		array<int, 2>^ gameTable; // �C�����ƿW��
		array<int, 2>^ originTable; // ��l�ƿW��
		array<int, 2>^ solveTable; // �ѵ��ƿW��

		array<int, 2>^ markTable; // �аO��
		array<bool, 2>^ hintTable; // �ϥδ��ܪ�
		array<bool, 2>^ errorTable; // ���~���A��

		SudokuLocation^ currentLocation; // �ثe�����m�I
		System::Collections::Generic::IList<SudokuLocation^>^ history; // �ާ@����

		static String^ getIntegerTableString(array<int, 2>^ table) { // ���o�ƿW��r��
			StringBuilder^ sb = gcnew StringBuilder();
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					sb->Append(table[i, j]);
				}
			}
			return sb->ToString();
		}

		static String^ getBooleanTableString(array<bool, 2>^ table) { // ���o���L���A�r��
			StringBuilder^ sb = gcnew StringBuilder();
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					sb->Append(table[i, j] ? 1 : 0);
				}
			}
			return sb->ToString();
		}

		virtual void initData() { // ��l���ܼ�
			remainingHint = MAX_HINT_QUOTA;
			currentLocation = gcnew SudokuLocation(-1, -1);
			history = gcnew List<SudokuLocation^>();
			initDrawComponent();

			gameTable = gcnew array<int, 2>(9, 9);
			originTable = gcnew array<int, 2>(9, 9);
			solveTable = gcnew array<int, 2>(9, 9);
			initIntegerTable(gameTable);
			initIntegerTable(originTable);
			initIntegerTable(solveTable);

			markTable = gcnew array<int, 2>(9, 9);
			initIntegerTable(markTable);

			errorTable = gcnew array<bool, 2>(9, 9);
			hintTable = gcnew array<bool, 2>(9, 9);
			initBooleanTable(errorTable);
			initBooleanTable(hintTable);
		}

		static void initIntegerTable(array<int, 2>^ table) { // ��l�ƾ�ƫ��O�G���}�C
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					table[i, j] = 0;
				}
			}
		}

		static void initBooleanTable(array<bool, 2>^ table) { // ��l�ƥ��L���O�G���}�C
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					table[i, j] = false;
				}
			}
		}

		virtual void loadRandomFile() { // ���o���;����X���ɮ�
			if (!Directory::Exists(DIRECTORY)) {
				throw gcnew EWException("�䤣���D�إؿ�");
			}
			array<String^>^ file = Directory::GetFiles(DIRECTORY);
			if (file->Length <= 0) {
				throw gcnew EWException("�䤣���D���ɮ�");
			}
			String^ select = file[random->Next(file->Length)];

			FileStream^ fs = nullptr;
			StreamReader^ sr = nullptr;
			try {
				fs = gcnew FileStream(select, FileMode::Open, FileAccess::Read);
				sr = gcnew StreamReader(fs);

				String^ answer = sr->ReadLine();
				String^ problem = sr->ReadLine();
				if (!checkRawIntegerTable(answer, true) || !checkRawIntegerTable(problem, false)) {
					throwFileError();
					return;
				}

				fillIntegerTable(originTable, problem);
				fillIntegerTable(solveTable, answer);
			}
			finally {
				if (sr != nullptr) {
					sr->Close();
				}
				if (fs != nullptr) {
					fs->Close();
				}
			}
		}

		virtual void swapRow(int left, int right) { // �洫��� (Row) �ƭ�
			for (int i = 0, temp; i < 9; ++i) {
				temp = originTable[left, i];
				originTable[left, i] = originTable[right, i];
				originTable[right, i] = temp;

				temp = solveTable[left, i];
				solveTable[left, i] = solveTable[right, i];
				solveTable[right, i] = temp;
			}
		}

		virtual void swapColumn(int left, int right) { // �洫��C (Column) �ƭ�
			for (int i = 0, temp; i < 9; ++i) {
				temp = originTable[i, left];
				originTable[i, left] = originTable[i, right];
				originTable[i, right] = temp;

				temp = solveTable[i, left];
				solveTable[i, left] = solveTable[i, right];
				solveTable[i, right] = temp;
			}
		}

		virtual void changeSymbol() { // ���üƭ�
			array<int>^ symbol = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			int round = 10;
			for (int i = 0; i < round; ++i) {
				int source = random->Next(9) + 1;
				int target = random->Next(9) + 1;

				int temp = symbol[source];
				symbol[source] = symbol[target];
				symbol[target] = temp;
			}

			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					originTable[i, j] = symbol[originTable[i, j]];
					solveTable[i, j] = symbol[solveTable[i, j]];
				}
			}
		}

		virtual void shuffleTable() { // ���üƿW��
			// (0, 1), (1, 2), (3, 4), (4, 5), (6, 7), (7, 8)
			array<int>^ shuffleLine = { 0, 1, 3, 4, 6, 7 };

			int round = 10;
			for (int i = 0; i < round; ++i) {
				int index = random->Next(6);
				int target = shuffleLine[index];
				swapRow(target, target + 1);
			}

			for (int i = 0; i < round; ++i) {
				int index = random->Next(6);
				int target = shuffleLine[index];
				swapColumn(target, target + 1);
			}
			changeSymbol();
		}

		virtual void fillHole(SudokuLevel level) { // �ھ����׶�R�Ʀr
			if (level == HELL) {
				return;
			}
			int currentNumber = 0;
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					if (originTable[i, j] != 0) {
						++currentNumber;
					}
				}
			}

			int goalNumber = 81 - getBlankByLevel(level);
			while (currentNumber < goalNumber) {
				int row = random->Next(9);
				int column = random->Next(9);
				if (originTable[row, column] == 0) {
					originTable[row, column] = solveTable[row, column];
					++currentNumber;
				}
			}
		}

		virtual void initTable() { // ��l�ƼƿW��
			shuffleTable();
			initGameTable();
		}
	public:
		SudokuTable() { // �غc�l
			initData();
		}

		SudokuTable(SudokuLevel level) { // ���׫غc�l
			initData();
			loadRandomFile();
			fillHole(level);
			initTable();
		}

		virtual array<int, 2>^ getGameTable() { // ���o�C�����ƿW��
			return gameTable;
		}

		virtual array<int, 2>^ getOriginTable() { // ���o��l�ƿW��
			return originTable;
		}

		virtual array<int, 2>^ getSolveTable() { // ���o�ѵ��ƿW��
			return solveTable;
		}

		virtual array<int, 2>^ getMarkTable() { // ���o�аO��
			return markTable;
		}

		virtual array<bool, 2>^ getHintTable() { // ���o�ϥδ��ܪ�
			return hintTable;
		}

		virtual array<bool, 2>^ getErrorTable() { // ���o���~���A��
			return errorTable;
		}

		virtual String^ getGameTableString() { // ���o�C�����ƿW��r��
			return getIntegerTableString(gameTable);
		}

		virtual String^ getOriginTableString() { // ���o��l�ƿW��r��
			return getIntegerTableString(originTable);
		}

		virtual String^ getSolveTableString() { // ���o�ѵ��ƿW��r��
			return getIntegerTableString(solveTable);
		}

		virtual String^ getMarkTableString() { // ���o�аO��r��
			return getIntegerTableString(markTable);
		}

		virtual String^ getHintTableString() { // ���o�ϥδ��ܪ�r��
			return getBooleanTableString(hintTable);
		}

		virtual String^ getErrorTableString() { // ���o���~���A��r��
			return getBooleanTableString(errorTable);
		}

		virtual System::Collections::Generic::IList<SudokuLocation^>^ getHistory() { // ���o�ާ@����
			return history;
		}

		virtual int getRemainingHint() { // ���o�Ѿl���ܦ���
			return remainingHint;
		}

		virtual int requestHint() { // ���ܥثe�ҿ��m
			--remainingHint;
			int ret = solveTable[currentLocation->getRow(), currentLocation->getColumn()];
			gameTable[currentLocation->getRow(), currentLocation->getColumn()] = ret;
			hintTable[currentLocation->getRow(), currentLocation->getColumn()] = true;
			return ret;
		}

		virtual bool checkCurrentLocation() { // �ˬd�ثe�Ҧb��m�O�_��J���~
			if (gameTable[currentLocation->getRow(), currentLocation->getColumn()] == 0) {
				return true;
			}
			bool error = gameTable[currentLocation->getRow(), currentLocation->getColumn()] != solveTable[currentLocation->getRow(), currentLocation->getColumn()];
			errorTable[currentLocation->getRow(), currentLocation->getColumn()] = error;
			return !error;
		}

		virtual int checkTable() { // �ˬd��J���~
			int empty = 0;
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					if (gameTable[i, j] == 0) {
						errorTable[i, j] = false;
						++empty;
						continue;
					}
					bool error = gameTable[i, j] != solveTable[i, j];
					errorTable[i, j] = error;
				}
			}
			return empty;
		}

		virtual bool hasError() { // �O�_����J���~
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					if (errorTable[i, j]) {
						return true;
					}
				}
			}
			return false;
		}

		virtual void addHistory(int row, int column, int value) { // �[�J�ާ@����
			history->Add(gcnew SudokuLocation(row, column, value));
		}

		virtual bool set(int value) { // �]�w�ثe��m�ƭ�
			if (gameTable[currentLocation->getRow(), currentLocation->getColumn()] == value) {
				return false;
			}
			gameTable[currentLocation->getRow(), currentLocation->getColumn()] = value;
			addHistory(currentLocation->getRow(), currentLocation->getColumn(), value);
			return true;
		}

		virtual void pencil(int value) { // �аO���T�w�Ʀr
			markTable[currentLocation->getRow(), currentLocation->getColumn()] = value;
		}

		virtual void clearAllPencil() { // �M���аO�Ʀr
			initIntegerTable(markTable);
		}

		virtual void initGameTable() { // ��l�ƹC���ƿW��
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					gameTable[i, j] = originTable[i, j];
				}
			}
		}

		virtual void clearFillNumber() { // �M����J�Ʀr
			clearAllPencil();
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					if (hintTable[i, j]) {
						continue;
					}
					gameTable[i, j] = originTable[i, j];
				}
			}
			initBooleanTable(errorTable);
			history->Clear();
		}

		virtual bool undo() { // �^�_�W�@�ާ@
			if (history->Count <= 0) {
				return false;
			}
			SudokuLocation^ last = history[history->Count - 1];
			SudokuLocation^ before;
			for (int i = history->Count - 2; i >= 0; --i) {
				SudokuLocation^ location = history[i];
				if (last->getRow() == location->getRow() && last->getColumn() == location->getColumn()) {
					before = location;
					break;
				}
			}
			if (before == nullptr) {
				gameTable[last->getRow(), last->getColumn()] = originTable[last->getRow(), last->getColumn()];
			}
			else {
				gameTable[last->getRow(), last->getColumn()] = before->getValue();
			}
			history->Remove(last);
			return true;
		}

		virtual SudokuLocation^ getCurrentLocation() { // ���o�ثe�����m
			return currentLocation;
		}

		virtual bool isSelect() { // �O�_�����m
			return !(currentLocation->getRow() == -1 && currentLocation->getColumn() == -1);
		}

		virtual void setNoSelect() { // �]�w�������
			currentLocation = gcnew SudokuLocation(-1, -1);
		}

		virtual void copySolve() { // ��ܸѵ�
			initBooleanTable(errorTable);
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					gameTable[i, j] = solveTable[i, j];
				}
			}
		}

		virtual void setRemainingHint(int hint) { // �]�w�Ѿl�i�δ���
			remainingHint = hint;
		}

		static void fillIntegerTable(array<int, 2>^ table, String^ data) { // �񺡫��w�ƿW��
			for (int y = 0; y < 9; ++y) {
				for (int x = 0; x < 9; ++x) {
					table[y, x] = data[y * 9 + x] - '0';
				}
			}
		}

		static void fillBooleanTable(array<bool, 2>^ table, String^ data) { // �񺡫��w���L���A��
			for (int y = 0; y < 9; ++y) {
				for (int x = 0; x < 9; ++x) {
					table[y, x] = data[y * 9 + x] == '1';
				}
			}
		}

		virtual void setCurrentLocation(int row, int column) { // �]�w�ثe�����m
			currentLocation = gcnew SudokuLocation(row, column);
		}

		static int getMaxHintQuota() { // ���o�C���̤j���ܦ���
			return MAX_HINT_QUOTA;
		}
	protected:
#pragma region �ϧγB�z�禡
		// ����
		static Brush^ staticFontColor = Brushes::Black;
		static Brush^ answerFontColor = Brushes::RoyalBlue;
		static Brush^ hintFontColor = Brushes::DeepPink;
		static Brush^ selectBrush = gcnew SolidBrush(Color::FromArgb(64, Color::RoyalBlue));
		static Brush^ smallFontColor = gcnew SolidBrush(Color::FromArgb(235, Color::Black));

		Brush^ background1;
		Brush^ background2;
		Pen^ errorPen;
		Pen^ border1;
		Pen^ border2;

		// ø�s�y��
		float realX, realY, realWidth;

		virtual void initDrawComponent() { // ��l��ø�Ϥu��
			background1 = (gcnew Pen(Color::Khaki))->Brush;
			background2 = (gcnew Pen(Color::LightGoldenrodYellow))->Brush;
			errorPen = gcnew Pen(Color::Red, 3);
			border1 = gcnew Pen(Color::Black, 3);
			border2 = gcnew Pen(Color::Black, 1);
		}
	public:
		virtual void setCurrentLocation(Point^ location) { // [UI �y�Х椬] �]�w�ثe�����m
			int newX = (int)Math::Floor((location->X - realX) / realWidth * 9.0);
			int newY = (int)Math::Floor((location->Y - realY) / realWidth * 9.0);
			if (newX < 0 || newX > 8 || newY < 0 || newY > 8) {
				currentLocation = gcnew SudokuLocation(-1, -1);
				return;
			}

			if (newX == currentLocation->getColumn() && newY == currentLocation->getRow()) {
				currentLocation = gcnew SudokuLocation(-1, -1);
			}
			else if (originTable[newY, newX] == 0 && !hintTable[newY, newX]) {
				currentLocation = gcnew SudokuLocation(newY, newX);
			}
		}

		virtual void drawTable(Graphics^ graphic) { // ø�s�ثe�ƿW��
			Brush^ backgroundBrush;
			Brush^ fontColor = staticFontColor;

			graphic->SmoothingMode = SmoothingMode::HighQuality;
			graphic->CompositingQuality = CompositingQuality::HighQuality;
			graphic->TextRenderingHint = TextRenderingHint::AntiAlias;

			float width = graphic->VisibleClipBounds.Width;
			float height = graphic->VisibleClipBounds.Height;
			float border = Math::Min(width, height);

			float centerX = width / 2;
			float centerY = height / 2;

			float startX = centerX - border / 2;
			float startY = centerY - border / 2;

			float offset = 20;
			realX = startX + offset;
			realY = startY + offset;
			realWidth = border - 2 * offset;
			if (realWidth <= 0) {
				return;
			}

			Font^ font = gcnew Font("Arial", realWidth / 20);
			Font^ smallFont = gcnew Font("Arial", realWidth / 72);
			float errorCircleOffset = realWidth / 9 * 0.95F;

			for (float i = 0; i < 3; ++i) {
				for (float j = 0; j < 3; ++j) {
					float drawBorderX = realX + realWidth * i / 3;
					float drawBorderY = realY + realWidth * j / 3;
					float divBorder = realWidth / 3;

					graphic->DrawRectangle(border1, drawBorderX, drawBorderY, divBorder, divBorder);
					if ((int)(i + j) % 2 == 0) {
						backgroundBrush = background1;
					}
					else {
						backgroundBrush = background2;
					}
					graphic->FillRectangle(backgroundBrush, drawBorderX, drawBorderY, divBorder, divBorder);

					for (float iInner = 0; iInner < 3; ++iInner) {
						for (float jInner = 0; jInner < 3; ++jInner) {
							int iIndex = Convert::ToInt32(i * 3 + iInner);
							int jIndex = Convert::ToInt32(j * 3 + jInner);

							float drawInnerX = realX + realWidth * (i / 3 + iInner / 9);
							float drawInnerY = realY + realWidth * (j / 3 + jInner / 9);
							float divInner = realWidth / 9;
							if (iIndex == currentLocation->getColumn() && jIndex == currentLocation->getRow()) {
								graphic->FillRectangle(selectBrush, drawInnerX, drawInnerY, divInner, divInner);
							}
							graphic->DrawRectangle(border2, drawInnerX, drawInnerY, divInner, divInner);

							String^ number = gameTable[jIndex, iIndex].ToString();
							String^ mark = markTable[jIndex, iIndex].ToString();

							if (originTable[jIndex, iIndex] != 0) {
								fontColor = staticFontColor;
							}
							else if (hintTable[jIndex, iIndex]) {
								fontColor = hintFontColor;
							}
							else {
								fontColor = answerFontColor;
							}

							SizeF numberSize = graphic->MeasureString(number, font);
							float numberX = drawInnerX + (divInner - numberSize.Width) / 2;
							float numberY = drawInnerY + (divInner - numberSize.Height) / 2;
							RectangleF rectangleNumber = RectangleF(numberX, numberY, numberSize.Width, numberSize.Height);

							float drawCircle = divInner - 2 * errorCircleOffset;
							RectangleF rectangleErrorCircle = RectangleF(drawInnerX + errorCircleOffset, drawInnerY + errorCircleOffset, drawCircle, drawCircle);

							if (mark != "0") {
								graphic->DrawString(mark, smallFont, smallFontColor, rectangleNumber); // �H�۹�j�r���m�i��ø�s
							}
							if (number != "0") {
								graphic->DrawString(number, font, fontColor, rectangleNumber);
							}
							if (showError && errorTable[jIndex, iIndex]) {
								graphic->DrawEllipse(errorPen, rectangleErrorCircle);
							}
						}
					}
				}
			}
		}
#pragma endregion
	};
}