#include "StdAfx.h"

#include "Application.h"
#include "MainForm.h"

#include <tchar.h>

typedef struct __TWMCommand__
{
	UINT Msg;
	DWORD ItemID;
	DWORD NotifyCode;
	HWND Ctl;
	LRESULT Result;
	/*
		TWMCommand CmdMsg;
		CmdMsg.Msg = Msg;
		CmdMsg.Ctl = (HWND)LParam;
		CmdMsg.ItemID = LOWORD(WParam);
		CmdMsg.NotifyCode = HIWORD(WParam);
	*/
} TWMCommand;

void TMainForm::InitControls()
{
#define IDB_NEW_PROJECT 101

	RECT NewProjBtnRect = {10, 10, 113, 41};

	FNewProjectButton.Create(
				(HMENU)IDB_NEW_PROJECT,
				_T("New Project"),
				&NewProjBtnRect,
				FHandle
				);
}

LRESULT CALLBACK TMainForm::HandleMessage(UINT Msg, WPARAM WParam, LPARAM LParam)
{

	switch (Msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(FHandle, &ps);
				// TODO: Add any drawing code that uses hdc here...
				EndPaint(FHandle, &ps);
			}
			break;

		case WM_NOTIFY:
			DebugLog(_T("WM_NOTIFY"));
			break;
		case WM_COMMAND:
			{
				/*
				* DEBUG
				*/
				{

					DWORD LowLParam = LOWORD(LParam);
					DWORD HighLParam = HIWORD(LParam);
					DebugLog(_T("WM_COMMAND LParam handle %d low %d high %d"), LParam, LowLParam, HighLParam);
					DWORD LowWParam = LOWORD(WParam);
					DWORD HighWParam = HIWORD(WParam);
					DebugLog(_T("WM_COMMAND WParam handle %d low %d high %d"), WParam, LowWParam, HighWParam);
				}

				TWMCommand CmdMsg = {};
				CmdMsg.Msg = Msg;
				CmdMsg.Ctl = (HWND)LParam;
				CmdMsg.ItemID = LOWORD(WParam);
				CmdMsg.NotifyCode = HIWORD(WParam);

				switch (CmdMsg.ItemID)
				{
					case ID_FILE_EXIT:
						{
							PostQuitMessage(0);
							return 0;
						}
					case IDB_NEW_PROJECT:
						{
							return this->NewProjBtnClick();
						}
					default:
						break;
				}
			}
			break;
		case WM_SIZING:
			{
				LPRECT Rect = (LPRECT)LParam;
				DebugLog(_T("WM_SIZING Height %d"), Rect->bottom);
				if (Rect->bottom != FMaxHeight) Rect->bottom = FMaxHeight;
			}
			break;
		case WM_SIZE:
			DebugLog(_T("WM_SIZE"));
			break;
		case WM_WINDOWPOSCHANGING:
			{
				LPWINDOWPOS Pos = (LPWINDOWPOS)LParam;
				DebugLog(_T("WM_WINDOWPOSCHANGING flag %d"), Pos->flags);
			}
			break;
		case WM_WINDOWPOSCHANGED:
			DebugLog(_T("WM_WINDOWPOSCHANGED"));
			break;
		case WM_SYSCOMMAND:
			{
				DebugLog(_T("WM_SYSCOMMAND Param %d"), WParam);
			}
			return DefWindowProc(FHandle, Msg, WParam, LParam);
		case WM_GETMINMAXINFO:
			{
				LPMINMAXINFO Info = (LPMINMAXINFO)LParam;
				DebugLog(_T("WM_GETMINMAXINFO %d"), Info->ptMaxSize.y);
				Info->ptMaxSize.y = FMaxHeight;

			}
			break;
		default:
			return DefWindowProc(FHandle, Msg, WParam, LParam);
	}
	return 0;
}

BOOL TMainForm::NewProjBtnClick()
{
	DebugLog(_T("New project button click"));
	return true;
}
