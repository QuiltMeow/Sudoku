#pragma once

namespace Sudoku {
	using namespace Microsoft::Win32::SafeHandles;
	using namespace System;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Runtime::InteropServices;
	using namespace System::Text;

	public ref class ConsoleHelper {
	private:
	protected:
		static const int STD_OUTPUT_HANDLE = -11;
		static String^ CMD_ENCODE = "Big5"; // Windows 北跌怠ㄏノ年年絪絏

		static const unsigned int SC_CLOSE = 0xF060;
		static const unsigned int MF_ENABLED = 0x00000000;
		static const unsigned int MF_GRAYED = 0x00000001;
		static const unsigned int MF_DISABLED = 0x00000002;
		static const unsigned int MF_BYCOMMAND = 0x00000000;

		String^ title;
		SafeFileHandle^ sfh;
		FileStream^ fs;
		StreamWriter^ stdOut;
		bool allocate;

		static ConsoleHelper^ instance;

		ConsoleHelper(String^ title) {
			this->title = title;
		}

#pragma region P / Invoke ㄧΑ
		[DllImport("kernel32.dll", SetLastError = true)]
		static bool AllocConsole();

		[DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true)]
		static bool FreeConsole();

		[DllImport("kernel32.dll", EntryPoint = "GetStdHandle", SetLastError = true, CharSet = CharSet::Auto, CallingConvention = CallingConvention::StdCall)]
		static IntPtr GetStdHandle(int nStdHandle);

		[DllImport("kernel32.dll", SetLastError = true)]
		static bool SetConsoleTitle(String^ lpConsoleTitle);

		[DllImport("kernel32.dll")]
		static IntPtr GetConsoleWindow();

		[DllImport("user32.dll")]
		static IntPtr GetSystemMenu(IntPtr hWnd, bool bRevert);

		[DllImport("user32.dll")]
		static bool DeleteMenu(IntPtr hMenu, unsigned int nPosition, unsigned int wFlag);
#pragma endregion
	public:
		static ConsoleHelper^ getInstance(String^ title) {
			if (instance == nullptr) {
				instance = gcnew ConsoleHelper(title);
			}
			return instance;
		}

		virtual bool isAllocate() {
			return allocate;
		}

		virtual void setupConsole() {
			if (allocate) {
				return;
			}
			AllocConsole();
			IntPtr consoleHandle = GetConsoleWindow();
			IntPtr menuHandle = GetSystemMenu(consoleHandle, false);
			DeleteMenu(menuHandle, SC_CLOSE, MF_BYCOMMAND);

			IntPtr stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			sfh = gcnew SafeFileHandle(stdHandle, true);
			fs = gcnew FileStream(sfh, FileAccess::Write);
			Encoding^ encoding = Encoding::GetEncoding(CMD_ENCODE);
			stdOut = gcnew StreamWriter(fs, encoding);
			stdOut->AutoFlush = true;
			Console::SetOut(stdOut);

			SetConsoleTitle(title);
			Console::WriteLine(" _____           _       _          ");
			Console::WriteLine("/  ___|         | |     | |         ");
			Console::WriteLine("\\ `--. _   _  __| | ___ | | ___   _ ");
			Console::WriteLine(" `--. \\ | | |/ _` |/ _ \\| |/ / | | |");
			Console::WriteLine("/\\__/ / |_| | (_| | (_) |   <| |_| |");
			Console::WriteLine("\\____/ \\__,_|\\__,_|\\___/|_|\\_\\\\__,_|");
			Console::WriteLine("北﹍てЧΘ");
			allocate = true;
		}

		virtual void removeConsole() {
			if (!allocate) {
				return;
			}
			stdOut->Close();
			fs->Close();
			sfh->Close();
			FreeConsole();
			allocate = false;
		}

		virtual void toggleConsole() {
			if (!allocate) {
				setupConsole();
			}
			else {
				removeConsole();
			}
		}
	};
}