#include "Application.h"

#include <tchar.h>
#include <stdarg.h>

TApplication *TApplication::GetInstance()
{
	static TApplication App;
	return &App;
}

TApplication::TApplication()
	: FAppInstance(nullptr)
{
#ifdef _DEBUG
	AllocConsole();
#endif
}

void TApplication::Init(HINSTANCE AppInstance)
{
	FAppInstance = AppInstance;
	return;
}

TApplication::~TApplication()
{
#ifdef _DEBUG
	FreeConsole();
#endif
}

int TApplication::DebugLog(const TCHAR *Fmt, ...)
{
#ifdef _DEBUG
	TCHAR Buffer[4096] = {};
	va_list ArgPtr;
	va_start(ArgPtr, Fmt);
	int Cnt = wvsprintfW(Buffer, Fmt, ArgPtr);
	Buffer[Cnt++] = _T('\n');
	va_end(ArgPtr);
	DWORD Size = 0;
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), Buffer, lstrlen(Buffer), &Size, NULL);
#endif
	return Cnt;
}

TApplication *GetApp()
{
	return TApplication::GetInstance();
}

TGDIData *TApplication::GetGDIData(HGDIOBJ Object)
{
	TGDIData *GDIData = nullptr;
	TThreadLock Lock(FGDILock);
	auto Iter = FGDIData.find(Object);

	if (Iter != FGDIData.end())
		GDIData = Iter->second;

	return GDIData;
}

void TApplication::AddGDIData(HGDIOBJ Object, TGDIData *Data)
{
	TThreadLock Lock(FGDILock);
	FGDIData.insert(std::make_pair(Object, Data));
}

void TApplication::DelGDIData(HGDIOBJ Object)
{
	TThreadLock Lock(FGDILock);
	auto Iter = FGDIData.find(Object);

	if (Iter != FGDIData.end())
		FGDIData.erase(Iter);
}

int DebugLog(const TCHAR* Fmt, ...)
{
#ifdef _DEBUG
	TCHAR Buffer[4096] = {};
	va_list ArgPtr;
	va_start(ArgPtr, Fmt);
	int Cnt = wvsprintfW(Buffer, Fmt, ArgPtr);
	Buffer[Cnt++] = _T('\n');
	va_end(ArgPtr);
	DWORD Size = 0;
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), Buffer, lstrlen(Buffer), &Size, NULL);
#endif
	return Cnt;
}

HINSTANCE GetInstance()
{
	return (HINSTANCE)::GetModuleHandle(NULL);
}

TCriticalSection::TCriticalSection() : FCount(0)
{
	::InitializeCriticalSection(&FCS);
}

TCriticalSection::~TCriticalSection()
{
	while (FCount > 0)
	{
		Release();
	}

	::DeleteCriticalSection(&FCS);
}

void TCriticalSection::Lock()
{
	::EnterCriticalSection(&FCS);
	::InterlockedIncrement(&FCount);
}

void TCriticalSection::Release()
{
	if (FCount > 0)
	{
		::LeaveCriticalSection(&FCS);
		::InterlockedDecrement(&FCount);
	}
}

void StrCopy(wchar_t *Dst, const wchar_t *Src, size_t DstSize)
{
	if (Dst && Src && DstSize != 0)
	{
		size_t Index;

		// Copy each character.
		for (Index = 0; Index < DstSize - 1; ++Index)
		{
			Dst[Index] = Src[Index];
			if (Src[Index] == '\0')
				break;
		}

		// Add null termination if required.
		if (Dst[Index] != '\0')
			Dst[DstSize - 1] = '\0';
	}
}