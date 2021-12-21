#pragma once

namespace SudokuGenerator {
	using namespace System;

	public delegate void WriteGeneratorLog(String^ message);
	public delegate void AllTaskFinish();

	public ref class LogEvent {
	private:
	protected:
	public:
		static event WriteGeneratorLog^ generatorLogEvent;
		static event AllTaskFinish^ taskFinishEvent;

		static void writeLog(String^ message) {
			generatorLogEvent(message);
		}

		static void taskFinish() {
			taskFinishEvent();
		}
	};
}