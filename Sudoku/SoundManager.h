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
		static SoundManager^ instance; // 單例模式

		String^ currentPlayBGM; // 目前播放 BGM
		bool muteBGM; // BGM 靜音狀態

		SoundPlayer^ sp;
		WindowsMediaPlayer^ wmp;

		SoundManager() { // 建構子
			muteBGM = false;
			sp = gcnew SoundPlayer();

			wmp = gcnew WindowsMediaPlayerClass();
			wmp->settings->volume = 100;
		}
	public:
		static SoundManager^ getInstance() { // 取得實例
			if (instance == nullptr) {
				instance = gcnew SoundManager();
			}
			return instance;
		}

		virtual void playBGM(String^ music) { // 播放指定 BGM
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
				Console::WriteLine("BGM 播放時發生例外狀況 : " + ex->Message);
			}
		}

		static void mciMusic(String^ path) { // Windows API 播放音樂
			StringBuilder^ sb = gcnew StringBuilder();
			mciSendString("play \"" + path + "\"", sb, 0, IntPtr::Zero);
		}

		virtual SoundPlayer^ getSoundPlayer() { // 取得 SoundPlayer 物件
			return sp;
		}

		virtual WindowsMediaPlayer^ getWindowsMediaPlayer() { // 取得 WindowsMediaPlayer 物件
			return wmp;
		}

		virtual bool isMuteBGM() { // 是否靜音
			return muteBGM;
		}

		virtual void toggleMuteBGM() { // 靜音開關
			muteBGM = !muteBGM;
			if (muteBGM) {
				wmp->settings->volume = 0;
			}
			else {
				wmp->settings->volume = 100;
			}
		}

		// P / Invoke 函式
		[DllImport("winmm.dll")]
		static int mciSendString(String^ command, StringBuilder^ buffer, int bufferSize, IntPtr hWndCallback);
	};
}