#ifndef UNICODE
#define UNICODE
#endif

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
	GetApp()->Init();

	TMainForm MainForm;
	if (!MainForm.Create(
				_T("Main Form"),
				WS_OVERLAPPEDWINDOW, WS_EX_OVERLAPPEDWINDOW,
				100, 300,
				300, 400,
				hInstance
				))
	{
		MessageBox(NULL,
				_T("Create MainForm failed!"),
				_T("Win32Practice"),
				NULL);

		return 1;
	}
	
	TDesignForm DesignForm;

	DesignForm.Create(
		_T("Design Form"),
		WS_OVERLAPPEDWINDOW, WS_EX_OVERLAPPEDWINDOW,
		600, 300,
		300, 400
	);

	MainForm.ShowWindow(nCmdShow);
	MainForm.UpdateWindow();

	DesignForm.ShowWindow(nCmdShow);
	DesignForm.UpdateWindow();

	SetWindowText(MainForm.Handle(), _T("中文"));

	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return (int)Msg.wParam;
}
