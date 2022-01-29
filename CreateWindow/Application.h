#ifndef __APPLICATION__
#define __APPLICATION__

#include <windows.h>
#include <tchar.h>

class TApplication
{
	public:
		static TApplication *GetInstance();
		void Init();
	public:
		int DebugLog(const TCHAR *Fmt, ...);
	private:
		TApplication();
		virtual ~TApplication();
};

TApplication *GetApp();

int DebugLog(const TCHAR* Fmt, ...);
#endif

