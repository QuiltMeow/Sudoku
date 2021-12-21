#pragma once

namespace SudokuGenerator {
	using namespace System;
	using namespace System::IO;
	using namespace System::Threading;
	using namespace System::Windows::Forms;
	using namespace MapleUI;

	public ref class GeneratorInformation {
	private:
	protected:
		int maxPuzzle;
		String^ outputDirectory;
		int puzzleCount;
	public:
		GeneratorInformation(int max) {
			maxPuzzle = max;
			puzzleCount = 0;
		}

		virtual int addPuzzle(String^ solution, String^ puzzle) {
			Monitor::Enter(this);
			try {
				++puzzleCount;
				File::WriteAllText(outputDirectory + "/" + solution + ".sudoku", solution + "\n" + puzzle);
			}
			catch (Exception ^ ex) {
				(gcnew MapleMessageBox("寫入檔案時發生例外狀況" + Environment::NewLine + ex->Message))->Show();
			}
			finally {
				Monitor::Exit(this);
			}
			return puzzleCount;
		}

		virtual int getMaxPuzzle() {
			return maxPuzzle;
		}

		virtual void setOutputDirectory(String^ directory) {
			outputDirectory = directory;
		}
	};
}