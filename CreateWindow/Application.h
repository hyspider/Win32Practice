#pragma once

#include <windows.h>
#include <tchar.h>
#include <map>

#include "GDI.h"

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
		int DebugLog(const TCHAR *Fmt, ...);
	protected:
		HINSTANCE FAppInstance;
	private:
		TApplication();
		virtual ~TApplication();
};

TApplication *GetApp();

HINSTANCE GetInstance();

int DebugLog(const TCHAR* Fmt, ...);
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