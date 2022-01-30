/*
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
*/

#include "StdAfx.h"

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "Application.h"
#include "MainForm.h"
#include "DesignForm.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/* 中文utf-8 */
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPreInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	GetApp()->Init(hInstance);

	RECT Rect = {};
	SystemParametersInfo(SPI_GETWORKAREA, 0, &Rect, 0);  
	TRACE(_T("WindowRect left: %d top: %d right: %d bottom: %d"),
			Rect.left,
			Rect.top,
			Rect.right,
			Rect.bottom
			);
	HMENU MainMenu = ::LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MAIN_MENU));
	TMainForm MainForm;
	if (!MainForm.Create(
				_T("Main Form"),
				//WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_CLIPCHILDREN | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
				WS_OVERLAPPEDWINDOW,
				WS_EX_OVERLAPPEDWINDOW,
				0, 0,
				Rect.right, MainForm.MaxHeight(),
				//320, 240,
				hInstance,
				nullptr//,
				//MainMenu
				))
	{
		MessageBox(nullptr,
				_T("Create MainForm failed!"),
				_T("Win32Practice"),
				0);

		return 1;
	}
	
	TDesignForm DesignForm;

	DesignForm.Create(
		_T("Design Form"),
		WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_CLIPCHILDREN,
		WS_EX_TOOLWINDOW,
		0, MainForm.MaxHeight(),
		300, Rect.bottom - MainForm.MaxHeight(),
		hInstance,
		MainForm.Handle()
	);

	MainForm.InitControls();
	/*
#define IDB_NEW_PROJECT 101
	::CreateWindow(
		_T("BUTTON"),
		_T("TEST"),
		(WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON),
		10,
		10,
		100,
		100,
		MainForm.Handle(),
		(HMENU)IDB_NEW_PROJECT,
		GetApp()->Instance(),
		nullptr
	);
	*/

	DesignForm.ShowWindow(nCmdShow);
	DesignForm.UpdateWindow();

	MainForm.ShowWindow(nCmdShow);
	MainForm.UpdateWindow();

	SetWindowText(MainForm.Handle(), _T("中文"));

	MSG Msg;
	while (GetMessage(&Msg, nullptr, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return (int)Msg.wParam;
}
