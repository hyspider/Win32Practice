#pragma once

#include <windows.h>
#include <tchar.h>
#include <map>

#include "GDI.h"

#define TRACE(x, ...) GetApp()->TraceLog(__FILE__, __func__, __LINE__, x, ##__VA_ARGS__);

class TCriticalSection
{
public:
	TCriticalSection();
	~TCriticalSection();

	void Lock();
	void Release();

private:
	TCriticalSection (const TCriticalSection&);
	TCriticalSection &operator = (const TCriticalSection&);

	CRITICAL_SECTION FCS;
	LONG FCount;
};

class TApplication
{
	public:
		static TApplication *GetInstance();
		void Init(HINSTANCE AppInstance);
	public:
		HINSTANCE Instance() const { return FAppInstance; };
		
		//GDI
		TCriticalSection FGDILock;
		std::map<HGDIOBJ, TGDIData*, CompareGDI> FGDIData;
		void AddGDIData(HGDIOBJ Object, TGDIData *Data);
		void DelGDIData(HGDIOBJ Object);
		TGDIData *GetGDIData(HGDIOBJ Object);
	public:
		int TraceLog(const char *File, const char *Func, long Line, const TCHAR *Fmt, ...);
	protected:
		HINSTANCE FAppInstance;
	private:
		TApplication();
		virtual ~TApplication();
};

TApplication *GetApp();

HINSTANCE GetInstance();

void StrCopy(wchar_t *Dst, const wchar_t *Src, size_t DstSize);

class TThreadLock
{
public:
	TThreadLock(TCriticalSection &cs) : FCS(cs) { FCS.Lock(); }
	~TThreadLock() { FCS.Release(); }

private:
	TThreadLock(const TThreadLock&);                // Disable copy construction
	TThreadLock &operator = (const TThreadLock&);    // Disable assignment operator
	TCriticalSection &FCS;
};