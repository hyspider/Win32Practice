#include "MainForm.h"

#include <tchar.h>

LRESULT TMainForm::HandleMessage(UINT Msg, WPARAM WParam, LPARAM LParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, Windows desktop!");

	switch (Msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_PAINT:
			hdc = BeginPaint(FHandle, &ps);

			// Here your application is laid out.
			// For this introduction, we just print out "Hello, Windows desktop!"
			// in the top left corner.
			TextOut(hdc, 5, 5, greeting, lstrlen(greeting));
			// End application specific layout section.

			EndPaint(FHandle, &ps);
			break;

		default:
			return DefWindowProc(FHandle, Msg, WParam, LParam);
	}
	return TRUE;
}
