#include "MainForm.h"

using namespace SkinSharp;
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Diagnostics;

[STAThreadAttribute]
void Main(array<String^>^ args) {
	Application::SetCompatibleTextRenderingDefault(false);
	try {
		SkinH_Net^ skin = gcnew SkinH_Net();
		skin->AttachEx("Skin.she", "");
	}
	catch (...) {
		Application::EnableVisualStyles();
	}

	Process^ currentProcess = Process::GetCurrentProcess();
	String^ processName = currentProcess->ProcessName;
	if (Process::GetProcessesByName(processName)->Length > 1) {
		MessageBox::Show("�w�������L�ƿW�C�����b����", "��T", MessageBoxButtons::OK, MessageBoxIcon::Information);
		return;
	}

	Sudoku::MainForm form;
	Application::Run(% form);
}