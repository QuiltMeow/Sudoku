#pragma once

namespace Sudoku {
	using namespace System;
	using namespace System::Media;
	using namespace System::Runtime::InteropServices;
	using namespace WMPLib;
	using namespace System::Text;

	public ref class SoundManager {
	private:
	protected:
		static SoundManager^ instance; // ��ҼҦ�

		String^ currentPlayBGM; // �ثe���� BGM
		bool muteBGM; // BGM �R�����A

		SoundPlayer^ sp;
		WindowsMediaPlayer^ wmp;

		SoundManager() { // �غc�l
			muteBGM = false;
			sp = gcnew SoundPlayer();

			wmp = gcnew WindowsMediaPlayerClass();
			wmp->settings->volume = 100;
		}
	public:
		static SoundManager^ getInstance() { // ���o���
			if (instance == nullptr) {
				instance = gcnew SoundManager();
			}
			return instance;
		}

		virtual void playBGM(String^ music) { // ������w BGM
			if (currentPlayBGM == music) {
				return;
			}
			try {
				wmp->controls->stop();
				wmp->URL = music;
				wmp->settings->setMode("loop", true);
				wmp->controls->play();
				currentPlayBGM = music;
			}
			catch (Exception ^ ex) {
				Console::WriteLine("BGM ����ɵo�ͨҥ~���p : " + ex->Message);
			}
		}

		static void mciMusic(String^ path) { // Windows API ���񭵼�
			StringBuilder^ sb = gcnew StringBuilder();
			mciSendString("play \"" + path + "\"", sb, 0, IntPtr::Zero);
		}

		virtual SoundPlayer^ getSoundPlayer() { // ���o SoundPlayer ����
			return sp;
		}

		virtual WindowsMediaPlayer^ getWindowsMediaPlayer() { // ���o WindowsMediaPlayer ����
			return wmp;
		}

		virtual bool isMuteBGM() { // �O�_�R��
			return muteBGM;
		}

		virtual void toggleMuteBGM() { // �R���}��
			muteBGM = !muteBGM;
			if (muteBGM) {
				wmp->settings->volume = 0;
			}
			else {
				wmp->settings->volume = 100;
			}
		}

		// P / Invoke �禡
		[DllImport("winmm.dll")]
		static int mciSendString(String^ command, StringBuilder^ buffer, int bufferSize, IntPtr hWndCallback);
	};
}