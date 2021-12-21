#pragma once

typedef unsigned char byte;

// 副產物 未使用
namespace Security {
	using namespace System::IO;
	using namespace System::Security::Cryptography;

	public ref class AES {
	private:
	protected:
	public:
		static array<byte>^ encrypt(array<byte>^ data, array<byte>^ key, array<byte>^ iv) {
			AesManaged^ aes = gcnew AesManaged();
			MemoryStream^ ms = nullptr;
			CryptoStream^ cs = nullptr;
			try {
				ms = gcnew MemoryStream();
				cs = gcnew CryptoStream(ms, aes->CreateEncryptor(key, iv), CryptoStreamMode::Write);
				cs->Write(data, 0, data->Length);
				cs->FlushFinalBlock();
				return ms->ToArray();
			}
			finally {
				if (cs != nullptr) {
					cs->Close();
				}
				if (ms != nullptr) {
					ms->Close();
				}
			}
		}

		static array<byte>^ decrypt(array<byte>^ data, array<byte>^ key, array<byte>^ iv) {
			AesManaged^ aes = gcnew AesManaged();
			MemoryStream^ ms = nullptr;
			CryptoStream^ cs = nullptr;
			try {
				ms = gcnew MemoryStream(data);
				cs = gcnew CryptoStream(ms, aes->CreateDecryptor(key, iv), CryptoStreamMode::Write);
				cs->Write(data, 0, data->Length);
				cs->FlushFinalBlock();
				return ms->ToArray();
			}
			finally {
				if (cs != nullptr) {
					cs->Close();
				}
				if (ms != nullptr) {
					ms->Close();
				}
			}
		}
	};
}