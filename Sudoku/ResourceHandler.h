#pragma once

#include "EWException.h"

typedef unsigned char byte;

namespace Sudoku {
	using namespace System;
	using namespace System::Resources;
	using namespace System::Reflection;
	using namespace System::Runtime::InteropServices;
	using namespace System::Drawing;
	using namespace System::Drawing::Text;
	using namespace System::ComponentModel;
	using namespace System::Windows::Forms;

	public ref class ResourceHandler {
	private:
	protected:
		static ResourceHandler^ instance;

		ResourceManager^ gameResource;
		FontFamily^ fontFamily;
		byte* fontDataPointer;

		Font^ displayLargeTitle;
		Font^ displayTitle;
		Font^ displayContent;

		ResourceHandler() {
			fontDataPointer = nullptr;

			loadGameResource();
			loadFontFamily();
			loadFont();
		}

		virtual ~ResourceHandler() {
			delete[] fontDataPointer;
		}

		virtual void loadGameResource() {
			Assembly^ assembly = Assembly::GetExecutingAssembly();
			String^ resourceName = assembly->GetName()->Name + ".GameResource";
			gameResource = gcnew ResourceManager(resourceName, assembly);
		}

		virtual void loadFontFamily() {
			InstalledFontCollection^ installFontCollection = gcnew InstalledFontCollection();
			PrivateFontCollection^ privateFontCollection = gcnew PrivateFontCollection();
			array<byte>^ fontData = (array<byte>^)gameResource->GetObject("Font");
			fontDataPointer = manageArrayToNativeArray(fontData);

			size_t installFontCount = 0;
			AddFontMemResourceEx((IntPtr)fontDataPointer, fontData->Length, IntPtr::Zero, &installFontCount);
			privateFontCollection->AddMemoryFont((IntPtr)fontDataPointer, fontData->Length);
			fontFamily = privateFontCollection->Families[0];
		}

		virtual void loadFont() {
			displayLargeTitle = gcnew Font(fontFamily, 15);
			displayTitle = gcnew Font(fontFamily, 12);
			displayContent = gcnew Font(fontFamily, 9);
		}
	public:
		static ResourceHandler^ getInstance() {
			if (instance == nullptr) {
				instance = gcnew ResourceHandler();
			}
			return instance;
		}

		static Cursor^ loadCustomCursor(String^ path) {
			IntPtr cursorHandle = LoadCursorFromFile(path);
			if (cursorHandle == IntPtr::Zero) {
				throw gcnew EWException("無法讀取滑鼠游標");
			}
			Cursor^ cursor = gcnew Cursor(cursorHandle);
			FieldInfo^ fieldInfo = cursor->GetType()->GetField("ownHandle", BindingFlags::NonPublic | BindingFlags::Instance);
			fieldInfo->SetValue(cursor, true);
			return cursor;
		}

		template<typename T>
		static T* manageArrayToNativeArray(array<T>^ data) {
			T* ret = new T[data->Length];
			for (int i = 0; i < data->Length; ++i) {
				ret[i] = data[i];
			}
			return ret;
		}

		virtual FontFamily^ getFontFamily() {
			return fontFamily;
		}

		virtual Font^ getDisplayLargeTitleFont() {
			return displayLargeTitle;
		}

		virtual Font^ getDisplayTitleFont() {
			return displayTitle;
		}

		virtual Font^ getDisplayContentFont() {
			return displayContent;
		}

		virtual ResourceManager^ getGameResource() {
			return gameResource;
		}

#pragma region P / Invoke 函式
		[DllImport("user32.dll", SetLastError = true, CharSet = CharSet::Unicode)]
		static IntPtr LoadCursorFromFile(String^ path);

		[DllImport("gdi32.dll")]
		static IntPtr AddFontMemResourceEx(IntPtr pointerFont, size_t fontSize, IntPtr pointerReserve, size_t* pointerInstallFontCount);
#pragma endregion
	};
}