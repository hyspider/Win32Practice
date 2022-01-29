#include "DesignForm.h"

LRESULT TDesignForm::HandleMessage(UINT Msg, WPARAM WParam, LPARAM LParam)
{

	switch (Msg)
	{
		case WM_PAINT:
			return DefWindowProc(FHandle, Msg, WParam, LParam);
		default:
			return DefWindowProc(FHandle, Msg, WParam, LParam);
	}
	return TRUE;
}
