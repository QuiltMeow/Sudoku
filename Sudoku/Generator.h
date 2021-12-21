#pragma once

#include "GeneratorInformation.h"
#include "SudokuWorker.h"
#include "LogEvent.h"

namespace SudokuGenerator {
	using namespace System;
	using namespace System::Threading;
	using namespace System::Collections::Generic;

	public ref class Generator {
	private:
	protected:
		static String^ output = "problem";
		static IList<Thread^>^ threadList = gcnew List<Thread^>();

		Generator() {
		}
	public:
		static void generate(int worker, int puzzle) {
			for (int i = 1; i <= worker; ++i) {
				GeneratorInformation^ info = gcnew GeneratorInformation(puzzle);
				info->setOutputDirectory(output);

				SudokuWorker^ sw = gcnew SudokuWorker(i, info);
				Thread^ swThread = gcnew Thread(gcnew ThreadStart(sw, &SudokuWorker::work));
				threadList->Add(swThread);

				LogEvent::writeLog("執行緒 " + i + " 開始執行");
				swThread->Start();
			}
		}

		static void watchDog() {
			for each (Thread ^ thread in threadList) {
				thread->Join();
			}

			Monitor::Enter(threadList);
			try {
				threadList->Clear();
			}
			finally {
				Monitor::Exit(threadList);
			}

			LogEvent::writeLog("所有執行緒執行完畢");
			LogEvent::taskFinish();
		}

		static void stopAll() {
			Monitor::Enter(threadList);
			try {
				for each (Thread ^ thread in threadList) {
					thread->Abort();
					while (thread->ThreadState != System::Threading::ThreadState::Aborted) {
					}
				}
			}
			finally {
				Monitor::Exit(threadList);
			}
			LogEvent::writeLog("所有工作中止執行");
		}
	};
}