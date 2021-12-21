#pragma once

typedef unsigned char byte;

// 副產物 未使用
namespace Security {
	using namespace System::Security::Cryptography;

	public ref class AESKey {
	private:
	protected:
		array<byte>^ key;
		array<byte>^ iv;
	public:
		AESKey() {
			generateKey(256);
		}

		AESKey(int keySize) {
			generateKey(keySize);
		}

		AESKey(array<byte>^ key, array<byte>^ iv) {
			setAESKey(key, iv);
		}

		virtual array<byte>^ getKey() {
			return key;
		}

		virtual array<byte>^ getIV() {
			return iv;
		}

		virtual void setAESKey(array<byte>^ key, array<byte>^ iv) {
			this->key = key;
			this->iv = iv;
		}

		virtual void generateKey(int keySize) {
			RijndaelManaged^ generator = gcnew RijndaelManaged();
			generator->KeySize = keySize;

			generator->GenerateKey();
			generator->GenerateIV();
			key = generator->Key;
			iv = generator->IV;
		}
	};
}