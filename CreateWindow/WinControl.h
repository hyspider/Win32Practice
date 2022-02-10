#pragma once

#include <windows.h>
#include <commctrl.h>
#include <assert.h>

class TRect : public RECT
{
public:	
	TRect()
	{
		Clear();
	}
	TRect(LONG Left, LONG Top, LONG Right, LONG bottom)
	{
		this->left = Left;
		this->top = Top;
		this->right = Right;
		this->bottom = bottom;
	}
	operator LPRECT() { return this; }
	operator LPCRECT() const { return this; }

	BOOL IsEmpty() const { return ::IsRectEmpty(this); }
	BOOL Clear() { return ::SetRectEmpty(this); }
	BOOL Set(LONG Left, LONG Top, LONG Right, LONG Bottom)
	{
		return ::SetRect(this, Left, Top, Right, Bottom);
	}

	BOOL PointInRect(LONG X, LONG Y)
	{
		POINT pt = {X, Y};
		return ::PtInRect(this, pt);
	}


	LONG Width() const { return right - left; }
	LONG Height() const { return bottom - top; }
};


class TSize : public SIZE
{
public:	
	TSize()
	{
		this->cx = 0;
		this->cy = 0;
	}
	TSize(LONG X, LONG Y)
	{
		this->cx = X;
		this->cy = Y;
	}
};

class TPoint : public POINT
{
public:
	TPoint()
	{
		this->x = 0;
		this->y = 0;
	}

	TPoint(LONG X, LONG Y)
	{
		this->x = X;
		this->y = Y;
	}
};


// CreateStruct: Thin wrapper for CREATESTRUCT structure.
class TCreateStruct : public CREATESTRUCT
{
public:
	TCreateStruct();
	void SetBoundingRect(const TRect &Rect);
};


class TWinControl
{
public:
	TWinControl() : FHandle(nullptr) {}

	void SetHandle(HWND Handle) { FHandle = Handle; }
	HWND Handle() const { return FHandle; }
	static LRESULT CALLBACK DefaultWindowProc(HWND Handle, UINT Msg, WPARAM WParam, LPARAM LParam);

public:
	LRESULT SendMessage(UINT Msg, WPARAM WParam, LPARAM LParam) const
	{
		return ::SendMessage(Handle(), Msg, WParam, LParam);
	}

	BOOL HasStyle(LONG Style) const
	{
		return (GetWindowLong(Handle(), GWL_STYLE) & Style) == Style;
	}

	LONG AddStyle(LONG Style)
	{
		LONG OldStyle = GetWindowLong(FHandle, GWL_STYLE);
		return SetWindowLong(Handle(), GWL_STYLE, OldStyle | Style);
	}

	LONG RemoveStyle(LONG Style)
	{
		LONG OldStyle = GetWindowLong(Handle(), GWL_STYLE);
		OldStyle = OldStyle & (~Style);
		return SetWindowLong(Handle(), GWL_STYLE, OldStyle);
	}

	LONG EnableStyle(BOOL Enable, LONG Style)
	{
		return (Enable ? AddStyle(Style) : RemoveStyle(Style));
	}

	BOOL HasExStyle(LONG ExStyle) const
	{
		return (GetWindowLong(Handle(), GWL_EXSTYLE) & ExStyle) == ExStyle;
	}

	LONG AddExStyle(LONG ExStyle)
	{
		LONG OldExStyle = GetWindowLong(Handle(), GWL_EXSTYLE);
		return SetWindowLong(Handle(), GWL_EXSTYLE, OldExStyle | ExStyle);
	}

	LONG RemoveExStyle(LONG ExStyle)
	{
		LONG OldExStyle = GetWindowLong(Handle(), GWL_EXSTYLE);
		OldExStyle = OldExStyle & (~ExStyle);
		return SetWindowLong(Handle(), GWL_EXSTYLE, OldExStyle);
	}

	LONG EnableExStyle(BOOL Enable, LONG ExStyle)
	{
		return (Enable ? AddExStyle(ExStyle) : RemoveExStyle(ExStyle));
	}

	LONG_PTR GetWindowLongPtr(int Index) const { return ::GetWindowLongPtr(*this, Index); }

	TRect GetWindowRect() const
	{
		TRect Rect;
		::GetWindowRect(*this, &Rect);
		return Rect;
	}

	BOOL IsWindow() { return ::IsWindow(*this); }

	BOOL IsWindowEnable() {	return ::IsWindowEnabled(*this); }

	BOOL IsWindowVisible() { return (this->GetWindowLongPtr(GWL_STYLE) & WS_VISIBLE) != 0; }

	operator HWND() const { return Handle(); }

	BOOL ShowWindow(int CmdShow = SW_SHOWNORMAL) { return ::ShowWindow(FHandle, CmdShow); }

	BOOL UpdateWindow() { return ::UpdateWindow(FHandle); }

protected:

	HWND Create(HWND Parent = nullptr);
	HWND FHandle;

protected:
	virtual LRESULT CALLBACK WindowProc(UINT Msg, WPARAM WParam, LPARAM LParam);
	virtual void PreCreate(TCreateStruct &CS) {};
	virtual void PreRegisterClass(WNDCLASSEX &WC) {};

private:
	BOOL RegisterClass(WNDCLASSEX &WC);
};


/*
* https://github.com/microsoft/Windows-classic-samples/blob/1d363ff4bd17d8e20415b92e2ee989d615cc0d91/Samples/Win7Samples/multimedia/directshow/common/wincontrol.h
*/