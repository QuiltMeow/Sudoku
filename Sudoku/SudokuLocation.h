#pragma once

namespace Sudoku {
	public ref class SudokuLocation {
	private:
	protected:
		int row; // 行
		int column; // 列
		int value; // 數值
	public:
		SudokuLocation(int row, int column) { // 建構子
			this->row = row;
			this->column = column;
			value = 0;
		}

		SudokuLocation(int row, int column, int value) { // 建構子 (包含數值)
			this->row = row;
			this->column = column;
			this->value = value;
		}

		virtual int getRow() { // 取得行的位置
			return row;
		}

		virtual int getColumn() { // 取得列的位置
			return column;
		}

		virtual int getValue() { // 取得數值
			return value;
		}
	};
}