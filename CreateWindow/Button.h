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
			FCS.SetBoundingRect(Rect);
			FCS.hMenu = ID;
			FCS.lpszName = Caption;
			FCS.style = Style;

			//FHandle = TWinControl::Create(ParentHandle);
			FHandle = ::CreateWindow(
				_T("BUTTON"),
				Caption,
				Style,
				Rect.left,
				Rect.top,
				Rect.right,
				Rect.bottom,
				ParentHandle,
				ID,
				GetApp()->Instance(),
				nullptr
			);
			return (FHandle ? TRUE : FALSE);
		};
	public:
		HMENU ID() const {return FID; };
	protected:
		HMENU FID;
		TCreateStruct FCS;
		void PreRegisterClass(WNDCLASSEX &WC) { WC.lpszClassName = _T("BUTTON"); };
		void PreCreate(TCreateStruct &CS) { CS = FCS; };
		virtual BOOL OnClick();
};

inline BOOL TButton::OnClick()
{
	return TRUE;
}