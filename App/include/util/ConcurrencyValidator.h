#pragma once

#include "rbx/Debug.h"
#include "rbx/atomic.h"


#ifdef __ARL_NOT_RELEASE
	#define ARL_USE_CONCURRENCY_VALIDATOR(expr) (expr)
#else
	#define ARL_USE_CONCURRENCY_VALIDATOR(expr) ((void)0)
#endif



namespace ARL { 

class ConcurrencyValidator
{
private:
    rbx::atomic<int> writing;
	std::string writeLocation;
    mutable rbx::atomic<int> reading;

public:
	ConcurrencyValidator() : writing(0), reading(0)
	{
	}

	~ConcurrencyValidator() {
		ARLASSERT(writing == 0);
		ARLASSERT(reading == 0);
	}

private:
	friend class WriteValidator;
	friend class ReadOnlyValidator;

	bool preRead() const {
		++reading;

		long wasWriting = writing;
		if (wasWriting != 0) {ARLASSERT(false && "writing check failed in preRead");}
		return true;
	}

	bool postRead() const {
		long wasWriting = writing;
		if (wasWriting != 0)	{ARLASSERT(false && "wasWriting check failed in postRead");}
		if (writing != 0)		{ARLASSERT(false && "writing check failed in postRead");}

		--reading;
		return true;
	}

	bool preWrite() {
		long wasWriting = writing;
		long wasReading = reading;

		if (wasReading != 0)	{ARLASSERT(false && "wasReading check failed in preWrite");}
		if (wasWriting != 0)	{ARLASSERT(false && "wasWriting check failed in preWrite");}
		if (writing != 0)		{ARLASSERT(false && "writing check failed in preWrite");}

		long result = ++writing;

		if (result != 1)		{ARLASSERT(false && "InterlocedIncrement returned not -1 in preWrite");}
		return true;
	}

	bool preWrite(const std::string& writeWhere) {
		bool okWrite = preWrite();
		if (okWrite) {
			writeLocation = writeWhere;
		}
		return okWrite;
	}

	bool postWrite() {
		long wasWriting = writing;
		long wasReading = reading;

		if (wasWriting != 1)	{ARLASSERT(false && "wasWriting check failed in postWrite");}
		if (writing != 1)		{ARLASSERT(false && "writing check failed in postWrite");}
		if (wasReading != 0)	{ARLASSERT(false && "wasReading check failed in postWrite");}
		if (reading != 0)		{ARLASSERT(false && "reading check failed in postWrite");}

		long result = --writing;
		if (result != 0)		{ARLASSERT(false && "InterlocedIncrement returned not 0 in postWrite");}
		if (reading != 0)		{ARLASSERT(false && "reading check failed in postWrite");}
		return true;
	}
};

class ReadOnlyValidator
{
private:
	const ConcurrencyValidator& concurrencyValidator;

public:
	ReadOnlyValidator(const ConcurrencyValidator& c) : concurrencyValidator(c) {
		ARL_USE_CONCURRENCY_VALIDATOR(concurrencyValidator.preRead());
	}

	~ReadOnlyValidator() {
		ARL_USE_CONCURRENCY_VALIDATOR(concurrencyValidator.postRead());
	}
};


class WriteValidator
{
private:
	ConcurrencyValidator& concurrencyValidator;

public:
	WriteValidator(ConcurrencyValidator& c) : concurrencyValidator(c) {
		ARL_USE_CONCURRENCY_VALIDATOR(concurrencyValidator.preWrite());
	}

	WriteValidator(ConcurrencyValidator& c, const std::string& writeWhere) : concurrencyValidator(c) {
		ARL_USE_CONCURRENCY_VALIDATOR(concurrencyValidator.preWrite(writeWhere));
	}

	WriteValidator(ConcurrencyValidator& c, const char* writeWhere) : concurrencyValidator(c) {
		ARL_USE_CONCURRENCY_VALIDATOR(concurrencyValidator.preWrite(writeWhere));
	}

	~WriteValidator() {
		ARL_USE_CONCURRENCY_VALIDATOR(concurrencyValidator.postWrite());
	}
};


}	// namespace

