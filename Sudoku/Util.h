#pragma once

#include "SudokuLevel.h"
#include "EWException.h"
#include "SudokuTool.h"

namespace Sudoku {
	using namespace System;

	bool showError = true; // 顯示錯誤

	void toggleShowError() { // 顯示錯誤開關
		showError = !showError;
	}

	String^ makeTimeReadable(int time) { // 將秒數轉為較易閱讀時間單位
		int hour = time / 3600;
		int minute = (time - hour * 3600) / 60;
		time = time - (hour * 3600 + minute * 60);

		if (hour > 0) {
			return hour + " 小時 " + minute + " 分 " + time + " 秒";
		}
		if (minute > 0) {
			return minute + " 分 " + time + " 秒";
		}
		return time + " 秒";
	}

	SudokuLevel integerToSudokuLevel(int value) { // 將數值轉為難度列舉
		if (value < EASY || value > HELL) {
			throw gcnew EWException("難度數值錯誤");
		}
		return (SudokuLevel)value;
	}

	int getBlankByLevel(SudokuLevel level) { // 根據難度取得要留下多少空格
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
		throw gcnew EWException("未知的難度");
	}

	String^ getLevelName(SudokuLevel level) { // 將難度列舉轉為中文名稱
		switch (level) {
		case EASY: {
			return "簡單";
		}
		case NORMAL: {
			return "中等";
		}
		case HARD: {
			return "困難";
		}
		case HELL: {
			return "地獄";
		}
		}
		throw gcnew EWException("未知的難度");
	}

	String^ getSudokuToolName(SudokuTool tool) { // 將填寫工具轉為中文名稱
		switch (tool) {
		case PEN: {
			return "原子筆";
		}
		case PENCIL: {
			return "鉛筆";
		}
		}
		throw gcnew EWException("未知的書寫工具");
	}

	void throwFileError() { // 丟出檔案損毀例外狀況
		throw gcnew EWException("檔案損毀");
	}

	bool checkRawIntegerTable(String^ data, bool answer) { // 檢查輸入文字是否符合數獨表規範
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

	bool checkRawBooleanTable(String^ data) { // 檢查輸入文字是否符合狀態表規範
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