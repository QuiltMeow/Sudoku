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
		static const int MAX_HINT_QUOTA = 5; // 每局最多提示次數
		static String^ DIRECTORY = "problem"; // 題目存放資料夾
		static Random^ random = gcnew Random(); // 隨機數產生器

		int remainingHint; // 剩餘提示次數
		array<int, 2>^ gameTable; // 遊戲中數獨表
		array<int, 2>^ originTable; // 原始數獨表
		array<int, 2>^ solveTable; // 解答數獨表

		array<int, 2>^ markTable; // 標記表
		array<bool, 2>^ hintTable; // 使用提示表
		array<bool, 2>^ errorTable; // 錯誤狀態表

		SudokuLocation^ currentLocation; // 目前選取位置點
		System::Collections::Generic::IList<SudokuLocation^>^ history; // 操作紀錄

		static String^ getIntegerTableString(array<int, 2>^ table) { // 取得數獨表字串
			StringBuilder^ sb = gcnew StringBuilder();
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					sb->Append(table[i, j]);
				}
			}
			return sb->ToString();
		}

		static String^ getBooleanTableString(array<bool, 2>^ table) { // 取得布林狀態字串
			StringBuilder^ sb = gcnew StringBuilder();
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					sb->Append(table[i, j] ? 1 : 0);
				}
			}
			return sb->ToString();
		}

		virtual void initData() { // 初始化變數
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

		static void initIntegerTable(array<int, 2>^ table) { // 初始化整數型別二維陣列
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					table[i, j] = 0;
				}
			}
		}

		static void initBooleanTable(array<bool, 2>^ table) { // 初始化布林型別二維陣列
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					table[i, j] = false;
				}
			}
		}

		virtual void loadRandomFile() { // 取得產生器產出的檔案
			if (!Directory::Exists(DIRECTORY)) {
				throw gcnew EWException("找不到題目目錄");
			}
			array<String^>^ file = Directory::GetFiles(DIRECTORY);
			if (file->Length <= 0) {
				throw gcnew EWException("找不到題目檔案");
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

		virtual void swapRow(int left, int right) { // 交換兩行 (Row) 數值
			for (int i = 0, temp; i < 9; ++i) {
				temp = originTable[left, i];
				originTable[left, i] = originTable[right, i];
				originTable[right, i] = temp;

				temp = solveTable[left, i];
				solveTable[left, i] = solveTable[right, i];
				solveTable[right, i] = temp;
			}
		}

		virtual void swapColumn(int left, int right) { // 交換兩列 (Column) 數值
			for (int i = 0, temp; i < 9; ++i) {
				temp = originTable[i, left];
				originTable[i, left] = originTable[i, right];
				originTable[i, right] = temp;

				temp = solveTable[i, left];
				solveTable[i, left] = solveTable[i, right];
				solveTable[i, right] = temp;
			}
		}

		virtual void changeSymbol() { // 打亂數值
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

		virtual void shuffleTable() { // 打亂數獨表
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

		virtual void fillHole(SudokuLevel level) { // 根據難度填充數字
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

		virtual void initTable() { // 初始化數獨表
			shuffleTable();
			initGameTable();
		}
	public:
		SudokuTable() { // 建構子
			initData();
		}

		SudokuTable(SudokuLevel level) { // 難度建構子
			initData();
			loadRandomFile();
			fillHole(level);
			initTable();
		}

		virtual array<int, 2>^ getGameTable() { // 取得遊戲中數獨表
			return gameTable;
		}

		virtual array<int, 2>^ getOriginTable() { // 取得原始數獨表
			return originTable;
		}

		virtual array<int, 2>^ getSolveTable() { // 取得解答數獨表
			return solveTable;
		}

		virtual array<int, 2>^ getMarkTable() { // 取得標記表
			return markTable;
		}

		virtual array<bool, 2>^ getHintTable() { // 取得使用提示表
			return hintTable;
		}

		virtual array<bool, 2>^ getErrorTable() { // 取得錯誤狀態表
			return errorTable;
		}

		virtual String^ getGameTableString() { // 取得遊戲中數獨表字串
			return getIntegerTableString(gameTable);
		}

		virtual String^ getOriginTableString() { // 取得原始數獨表字串
			return getIntegerTableString(originTable);
		}

		virtual String^ getSolveTableString() { // 取得解答數獨表字串
			return getIntegerTableString(solveTable);
		}

		virtual String^ getMarkTableString() { // 取得標記表字串
			return getIntegerTableString(markTable);
		}

		virtual String^ getHintTableString() { // 取得使用提示表字串
			return getBooleanTableString(hintTable);
		}

		virtual String^ getErrorTableString() { // 取得錯誤狀態表字串
			return getBooleanTableString(errorTable);
		}

		virtual System::Collections::Generic::IList<SudokuLocation^>^ getHistory() { // 取得操作紀錄
			return history;
		}

		virtual int getRemainingHint() { // 取得剩餘提示次數
			return remainingHint;
		}

		virtual int requestHint() { // 提示目前所選位置
			--remainingHint;
			int ret = solveTable[currentLocation->getRow(), currentLocation->getColumn()];
			gameTable[currentLocation->getRow(), currentLocation->getColumn()] = ret;
			hintTable[currentLocation->getRow(), currentLocation->getColumn()] = true;
			return ret;
		}

		virtual bool checkCurrentLocation() { // 檢查目前所在位置是否輸入錯誤
			if (gameTable[currentLocation->getRow(), currentLocation->getColumn()] == 0) {
				return true;
			}
			bool error = gameTable[currentLocation->getRow(), currentLocation->getColumn()] != solveTable[currentLocation->getRow(), currentLocation->getColumn()];
			errorTable[currentLocation->getRow(), currentLocation->getColumn()] = error;
			return !error;
		}

		virtual int checkTable() { // 檢查輸入錯誤
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

		virtual bool hasError() { // 是否有輸入錯誤
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					if (errorTable[i, j]) {
						return true;
					}
				}
			}
			return false;
		}

		virtual void addHistory(int row, int column, int value) { // 加入操作紀錄
			history->Add(gcnew SudokuLocation(row, column, value));
		}

		virtual bool set(int value) { // 設定目前位置數值
			if (gameTable[currentLocation->getRow(), currentLocation->getColumn()] == value) {
				return false;
			}
			gameTable[currentLocation->getRow(), currentLocation->getColumn()] = value;
			addHistory(currentLocation->getRow(), currentLocation->getColumn(), value);
			return true;
		}

		virtual void pencil(int value) { // 標記不確定數字
			markTable[currentLocation->getRow(), currentLocation->getColumn()] = value;
		}

		virtual void clearAllPencil() { // 清除標記數字
			initIntegerTable(markTable);
		}

		virtual void initGameTable() { // 初始化遊戲數獨表
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					gameTable[i, j] = originTable[i, j];
				}
			}
		}

		virtual void clearFillNumber() { // 清除填入數字
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

		virtual bool undo() { // 回復上一操作
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

		virtual SudokuLocation^ getCurrentLocation() { // 取得目前選取位置
			return currentLocation;
		}

		virtual bool isSelect() { // 是否選取位置
			return !(currentLocation->getRow() == -1 && currentLocation->getColumn() == -1);
		}

		virtual void setNoSelect() { // 設定為不選取
			currentLocation = gcnew SudokuLocation(-1, -1);
		}

		virtual void copySolve() { // 顯示解答
			initBooleanTable(errorTable);
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					gameTable[i, j] = solveTable[i, j];
				}
			}
		}

		virtual void setRemainingHint(int hint) { // 設定剩餘可用提示
			remainingHint = hint;
		}

		static void fillIntegerTable(array<int, 2>^ table, String^ data) { // 填滿指定數獨表
			for (int y = 0; y < 9; ++y) {
				for (int x = 0; x < 9; ++x) {
					table[y, x] = data[y * 9 + x] - '0';
				}
			}
		}

		static void fillBooleanTable(array<bool, 2>^ table, String^ data) { // 填滿指定布林狀態表
			for (int y = 0; y < 9; ++y) {
				for (int x = 0; x < 9; ++x) {
					table[y, x] = data[y * 9 + x] == '1';
				}
			}
		}

		virtual void setCurrentLocation(int row, int column) { // 設定目前選取位置
			currentLocation = gcnew SudokuLocation(row, column);
		}

		static int getMaxHintQuota() { // 取得每局最大提示次數
			return MAX_HINT_QUOTA;
		}
	protected:
#pragma region 圖形處理函式
		// 筆刷
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

		// 繪製座標
		float realX, realY, realWidth;

		virtual void initDrawComponent() { // 初始化繪圖工具
			background1 = (gcnew Pen(Color::Khaki))->Brush;
			background2 = (gcnew Pen(Color::LightGoldenrodYellow))->Brush;
			errorPen = gcnew Pen(Color::Red, 3);
			border1 = gcnew Pen(Color::Black, 3);
			border2 = gcnew Pen(Color::Black, 1);
		}
	public:
		virtual void setCurrentLocation(Point^ location) { // [UI 座標交互] 設定目前選取位置
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

		virtual void drawTable(Graphics^ graphic) { // 繪製目前數獨表
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
								graphic->DrawString(mark, smallFont, smallFontColor, rectangleNumber); // 以相對大字體位置進行繪製
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