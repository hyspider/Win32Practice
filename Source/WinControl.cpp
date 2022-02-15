#include "WinControl.h"
#include "Application.h"


HWND TWinControl::Create(HWND Parent)
{
	WNDCLASSEX WC;
	::ZeroMemory(&WC, sizeof(WC));
	WC.cbSize = sizeof(WC);

	TCreateStruct CS;
	::ZeroMemory(&CS, sizeof(CS));
	CS.hInstance = WC.hInstance;

	PreRegisterClass(WC);
	if (WC.lpszClassName)
	{
		this->RegisterClass(WC);
		CS.lpszClass = WC.lpszClassName;
	}
	else
	{
		CS.lpszClass = _T("GWindow");
	}	

	DWORD OverlappedStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	
	CS.style = WS_VISIBLE | ((Parent) ? WS_CHILD : OverlappedStyle);

	if (Parent == nullptr)
	{
		CS.x = CW_USEDEFAULT;
		CS.cx = CW_USEDEFAULT;
		CS.y = CW_USEDEFAULT;
		CS.cy = CW_USEDEFAULT;
	}

	PreCreate(CS);

	DWORD Style = CS.style & ~WS_VISIBLE;

	HWND Handle = nullptr;

	FHandle = ::CreateWindowEx(CS.dwExStyle, CS.lpszClass, CS.lpszName, Style,
		CS.x, CS.y, CS.cx, CS.cy, Parent,
		CS.hMenu, CS.hInstance, CS.lpCreateParams);

	if (CS.style & WS_VISIBLE)
	{
		if (CS.style & WS_MAXIMIZE) ShowWindow(SW_MAXIMIZE);
		else if (CS.style & WS_MINIMIZE) ShowWindow(SW_MINIMIZE);
		else ShowWindow();
	}

	return FHandle;
};

LRESULT CALLBACK TWinControl::DefaultWindowProc(HWND Handle, UINT Msg, WPARAM WParam, LPARAM LParam)
{
	TWinControl *ThisPtr = nullptr;

	if (Msg == WM_NCCREATE)
	{
		CREATESTRUCT* CreateStruct = (CREATESTRUCT*)LParam;
		ThisPtr = (TWinControl *)CreateStruct->lpCreateParams;
		::SetWindowLongPtr(Handle, GWLP_USERDATA, (LONG_PTR)ThisPtr);

		ThisPtr->SetHandle(Handle);
	}
	else
	{
		ThisPtr = (TWinControl *)::GetWindowLongPtr(Handle, GWLP_USERDATA);
	}
	if (ThisPtr)
	{
		return ThisPtr->WindowProc(Msg, WParam, LParam);
	}
	return ::DefWindowProc(Handle, Msg, WParam, LParam);
}

LRESULT CALLBACK TWinControl::WindowProc(UINT Msg, WPARAM WParam, LPARAM LParam)
{
	return ::DefWindowProc(Handle(), Msg, WParam, LParam);
}

BOOL TWinControl::RegisterClass(WNDCLASSEX &WC)
{
	WNDCLASSEX Info = {};
	Info.cbSize = sizeof(Info);
	BOOL Find = ::GetClassInfoEx(GetApp()->Instance(), WC.lpszClassName, &Info);
	if (Find == FALSE)
	{
		WC.lpfnWndProc = TWinControl::DefaultWindowProc;
		WC.hInstance = GetApp()->Instance();
		Find = ::RegisterClassEx(&WC);
	}
	else
	{
		WC = Info;
	}
	return Find;
}


TCreateStruct::TCreateStruct()
{
	::ZeroMemory(this, sizeof(*this));
}

void TCreateStruct::SetBoundingRect(const TRect &Rect)
{
	x = Rect.left;
	y = Rect.top;
	cx = Rect.right - x;
	cy = Rect.bottom - y;
}
