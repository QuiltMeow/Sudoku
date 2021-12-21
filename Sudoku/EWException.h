#pragma once

using namespace System;
using namespace System::Runtime::Serialization;

[Serializable]
public ref class EWException : public Exception, ISerializable {
private:
protected:
	static const int EW = 6987;
	int type;

	EWException(SerializationInfo^ info, StreamingContext context) : Exception(info, context) {
	}
public:
	EWException() : Exception("µo¥Í¨Ò¥~ª¬ªp") {
	}

	EWException(String^ message) : Exception(message) {
	}

	EWException(String^ message, Exception^ inner) : Exception(message, inner) {
	}

	EWException(String^ message, int type) : Exception(message) {
		this->type = type;
	}

	EWException(String^ message, Exception^ inner, int type) : Exception(message, inner) {
		this->type = type;
	}

	EWException(SerializationInfo^ info, StreamingContext context, int type) : Exception(info, context) {
		this->type = type;
	}

	virtual int getFirstRef() {
		return 80923; // SX
	}

	virtual double getSecondRef() {
		return 221; // RX
	}

	virtual double defaultDouble() {
		return 0;
	}

	virtual int defaultInteger() {
		return 0;
	}

	virtual String^ defaultString() {
		return "";
	}

	virtual int errorInteger() {
		return -1;
	}

	virtual void forceExit() {
		Environment::Exit(EW);
	}

	virtual int getEW() {
		return EW;
	}

	virtual int getExceptionType() {
		return type;
	}

	virtual Object^ getNull() {
		return nullptr;
	}
};