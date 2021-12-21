#pragma once

#include "SudokuLevel.h"
#include "EWException.h"
#include "SudokuTool.h"

namespace Sudoku {
	using namespace System;

	bool showError = true; // ��ܿ��~

	void toggleShowError() { // ��ܿ��~�}��
		showError = !showError;
	}

	String^ makeTimeReadable(int time) { // �N����ର�����\Ū�ɶ����
		int hour = time / 3600;
		int minute = (time - hour * 3600) / 60;
		time = time - (hour * 3600 + minute * 60);

		if (hour > 0) {
			return hour + " �p�� " + minute + " �� " + time + " ��";
		}
		if (minute > 0) {
			return minute + " �� " + time + " ��";
		}
		return time + " ��";
	}

	SudokuLevel integerToSudokuLevel(int value) { // �N�ƭ��ର���צC�|
		if (value < EASY || value > HELL) {
			throw gcnew EWException("���׼ƭȿ��~");
		}
		return (SudokuLevel)value;
	}

	int getBlankByLevel(SudokuLevel level) { // �ھ����ר��o�n�d�U�h�֪Ů�
		switch (level) {
		case EASY: {
			return 40;
		}
		case NORMAL: {
			return 45;
		}
		case HARD: {
			return 50;
		}
		case HELL: {
			return 81;
		}
		}
		throw gcnew EWException("����������");
	}

	String^ getLevelName(SudokuLevel level) { // �N���צC�|�ର����W��
		switch (level) {
		case EASY: {
			return "²��";
		}
		case NORMAL: {
			return "����";
		}
		case HARD: {
			return "�x��";
		}
		case HELL: {
			return "�a��";
		}
		}
		throw gcnew EWException("����������");
	}

	String^ getSudokuToolName(SudokuTool tool) { // �N��g�u���ର����W��
		switch (tool) {
		case PEN: {
			return "��l��";
		}
		case PENCIL: {
			return "�]��";
		}
		}
		throw gcnew EWException("�������Ѽg�u��");
	}

	void throwFileError() { // ��X�ɮ׷l���ҥ~���p
		throw gcnew EWException("�ɮ׷l��");
	}

	bool checkRawIntegerTable(String^ data, bool answer) { // �ˬd��J��r�O�_�ŦX�ƿW��W�d
		if (data->Length != 81) {
			return false;
		}
		for (int i = 0; i < 81; ++i) {
			int value = data[i] - '0';
			if (answer && (value <= 0 || value > 9)) {
				return false;
			}
			else if (value < 0 || value > 9) {
				return false;
			}
		}
		return true;
	}

	bool checkRawBooleanTable(String^ data) { // �ˬd��J��r�O�_�ŦX���A��W�d
		if (data->Length != 81) {
			return false;
		}
		for (int i = 0; i < 81; ++i) {
			int value = data[i] - '0';
			if (value < 0 || value > 1) {
				return false;
			}
		}
		return true;
	}
}