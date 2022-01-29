#ifndef __BASE_FORM__
#define __BASE_FORM__

#include <Windows.h>

template <class BASEFORM_DERIVED_TYPE>
class TBaseForm
{
	public:
		static LRESULT CALLBACK WindowProc(HWND Handle, UINT Msg, WPARAM WParam, LPARAM LParam)
		{
			BASEFORM_DERIVED_TYPE *ThisPtr = nullptr;

			if (Msg == WM_NCCREATE)
			{
				CREATESTRUCT *CreateStruct = (CREATESTRUCT *)LParam;
				ThisPtr = (BASEFORM_DERIVED_TYPE *)CreateStruct->lpCreateParams;
				SetWindowLongPtr(Handle, GWLP_USERDATA, (LONG_PTR)ThisPtr);

				ThisPtr->FHandle = Handle;
			}
			else
			{
				ThisPtr = (BASEFORM_DERIVED_TYPE *)GetWindowLongPtr(Handle, GWLP_USERDATA);
			}
			if (ThisPtr)
			{
				return ThisPtr->HandleMessage(Msg, WParam, LParam);
			}
			return DefWindowProc(Handle, Msg, WParam, LParam);
		}

		TBaseForm() : FHandle(nullptr) { }

		BOOL Create(
				PCWSTR WindowName,
				DWORD Style,
				DWORD ExStyle = 0,
				int X = CW_USEDEFAULT,
				int Y = CW_USEDEFAULT,
				int Width = CW_USEDEFAULT,
				int Height = CW_USEDEFAULT,
				HINSTANCE InstanceHandle = nullptr,
				HWND ParentHandle = nullptr,
				HMENU MenuHandle = nullptr
				)
		{
			WNDCLASSEX WndClass = {0};

			WndClass.cbSize = sizeof(WndClass);
			WndClass.hbrBackground = reinterpret_cast<HBRUSH>(::GetStockObject(COLOR_WINDOW));
			WndClass.lpfnWndProc = BASEFORM_DERIVED_TYPE::WindowProc;
			WndClass.hInstance = InstanceHandle;
			WndClass.lpszClassName = ClassName();

			RegisterClassEx(&WndClass);

			FHandle = ::CreateWindowEx(
					ExStyle, ClassName(), WindowName, Style, X, Y,
					Width, Height, ParentHandle, MenuHandle, WndClass.hInstance, this
					);

			return (FHandle ? TRUE : FALSE);
		}

		HWND Handle() const { return FHandle; }

	protected:

		virtual PCWSTR  ClassName() const = 0;
		virtual LRESULT HandleMessage(UINT Msg, WPARAM WParam, LPARAM LParam) = 0;

		HWND FHandle;

	/* api wrap */
	public:
		BOOL WINAPI ShowWindow(int CmdShow) { return ::ShowWindow(FHandle, CmdShow); }
		BOOL WINAPI UpdateWindow() { return ::UpdateWindow(FHandle); }
};

#endif

