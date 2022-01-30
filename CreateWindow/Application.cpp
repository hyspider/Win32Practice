#include "Application.h"

#include <tchar.h>
#include <stdarg.h>
#include <string>

TApplication *GetApp()
{
	return TApplication::GetInstance();
}

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

int TApplication::TraceLog(const char *File, const char *Func, long Line, const TCHAR *Fmt, ...)
{
#ifdef _DEBUG
	TCHAR Buffer[4096] = {};
	va_list ArgPtr;
	va_start(ArgPtr, Fmt);
	int Cnt = wvsprintfW(Buffer, Fmt, ArgPtr);
	Buffer[Cnt] = _T('\n');
	va_end(ArgPtr);
	DWORD Size = 0;

	std::string FileStr = File;
	auto Pos = FileStr.find_last_of("\\");
	if (Pos != std::string::npos)
	{
		FileStr = FileStr.substr(Pos + 1, FileStr.size());
	}
	char Header[1024] = {0};
	snprintf(&Header[0], sizeof(Header), "<%s>[%s:%d]: ", FileStr.c_str(), Func, Line);
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), Header, (DWORD)strlen(Header), &Size, nullptr);
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), Buffer, lstrlen(Buffer), &Size, nullptr);
#endif
	return 0;
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

HINSTANCE GetInstance()
{
	return (HINSTANCE)::GetModuleHandle(nullptr);
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