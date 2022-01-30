#pragma once

#include <Windows.h>
#include <tchar.h>
#include "GDI.h"
#include "WinControl.h"
#include "Application.h"

class TButton
{
	public:
		TButton()
			: FHandle(nullptr), FID(nullptr)
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
			LPCWSTR Caption,
			LPRECT Rect,
			HWND ParentHandle = nullptr,
			HINSTANCE InstanceHandle = nullptr,
			DWORD Style = (WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON)
		)
		{

			FID = ID;
			FHandle = CreateWindow(
					_T("BUTTON"),
					Caption,
					Style,
					Rect->left,
					Rect->top,
					Rect->right,
					Rect->bottom,
					ParentHandle,
					ID,
					InstanceHandle,
					NULL
			);

			if (FHandle)
			{
				FFont.CreatePointFont(90, _T("Tahoma"));
				DebugLog(_T("Font name %s height %d"), FFont.GetLogFont().lfFaceName,
					FFont.GetLogFont().lfHeight);
				//HFONT DefaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
				HFONT DefaultFont = FFont;

				SendMessage(FHandle, WM_SETFONT, WPARAM(DefaultFont), TRUE);
			}

			TRect BtnRect;
			if (FHandle)
				GetWindowRect(FHandle, &BtnRect);
			DebugLog(_T("Button left %d top %d right %d bottom %d width %d height %d"),
				BtnRect.left, BtnRect.top, BtnRect.right, BtnRect.bottom,
				BtnRect.right - BtnRect.left, BtnRect.bottom - BtnRect.top);
			return (FHandle ? TRUE : FALSE);
		};
	public:
		HWND Handle() const { return FHandle; };
		HMENU ID() const {return FID; };
	protected:
		HWND FHandle;
		HMENU FID;
		TFont FFont;

		virtual BOOL OnClick();
};

inline BOOL TButton::OnClick()
{
	return TRUE;
}