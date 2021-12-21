#pragma once

namespace Sudoku {
	public ref class SudokuLocation {
	private:
	protected:
		int row; // ��
		int column; // �C
		int value; // �ƭ�
	public:
		SudokuLocation(int row, int column) { // �غc�l
			this->row = row;
			this->column = column;
			value = 0;
		}

		SudokuLocation(int row, int column, int value) { // �غc�l (�]�t�ƭ�)
			this->row = row;
			this->column = column;
			this->value = value;
		}

		virtual int getRow() { // ���o�檺��m
			return row;
		}

		virtual int getColumn() { // ���o�C����m
			return column;
		}

		virtual int getValue() { // ���o�ƭ�
			return value;
		}
	};
}