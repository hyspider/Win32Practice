#pragma once

#include <Windows.h>
#include <tchar.h>
#include "GDI.h"
#include "WinControl.h"
#include "Application.h"

class TButton : public TWinControl
{
	public:
		TButton()
			: FID(nullptr)
		{
		};
		virtual ~TButton()
		{
			if (FHandle)
			{
				::DestroyWindow(FHandle);
			}
		}

		BOOL Create(
			HMENU ID,
			LPCTSTR Caption,
			TRect &Rect,
			HWND ParentHandle = nullptr,
			DWORD Style = (WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON)
		)
		{
			FCS.lpszClass = _T("BUTTON");
			FCS.SetBoundingRect(Rect);
			FCS.hMenu = ID;
			FCS.lpszName = Caption;
			FCS.style = Style;

			TWinControl::Create(ParentHandle);
			return (Handle() ? TRUE : FALSE);
		};
	public:
		HMENU ID() const {return FID; };
	protected:
		HMENU FID;
		TCreateStruct FCS;
		void PreCreate(TCreateStruct &CS) { CS = FCS; };
		virtual BOOL OnClick();
};

inline BOOL TButton::OnClick()
{
	return TRUE;
}