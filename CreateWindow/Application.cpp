#include "Application.h"

#include <tchar.h>
#include <stdarg.h>

TApplication *TApplication::GetInstance()
{
	static TApplication App;
	return &App;
}

TApplication::TApplication()
{
#ifdef _DEBUG
	AllocConsole();
#endif
}

void TApplication::Init()
{
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
