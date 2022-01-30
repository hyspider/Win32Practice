#pragma once

#include <Windows.h>

struct CompareGDI
{
	bool operator()(const HGDIOBJ A, const HGDIOBJ B) const
		{return (reinterpret_cast<DWORD_PTR>(A) < reinterpret_cast<DWORD_PTR>(B));}
};

struct TGDIData
{
	// Constructor
	TGDIData() : GDIObject(0), Count(1L), IsManagedObject(false) {}

	HGDIOBJ GDIObject;
	LONG Count;
	BOOL IsManagedObject;
};

class TGDIObject
{
public:
	TGDIObject();
	TGDIObject(const TGDIObject &RHS);
	virtual ~TGDIObject();
	TGDIObject &operator = (const TGDIObject &RHS);
	void operator = (const HGDIOBJ Object);

	void    Attach(HGDIOBJ Object);
	void    DeleteObject();
	HGDIOBJ Detach();
	HGDIOBJ Handle() const;
	int     GetObject(int Count, LPVOID ObjectPtr) const;

protected:
	void    Release();
	void    SetManaged(BOOL IsManaged) const { FData->IsManagedObject = IsManaged; }

private:
	void    AddToMap();
	void    RemoveFromMap();

	TGDIData *FData;
};


class TFont : public TGDIObject
{
public:
	TFont();
	TFont(HFONT Font);
	TFont(const LOGFONT &LogFont);
	operator HFONT() const;
	virtual ~TFont();

	// Create methods
	HFONT CreateFontIndirect(const LOGFONT &LogFont);
	HFONT CreatePointFont(int PointSize, LPCTSTR FaceName, HDC DC = nullptr, BOOL IsBold = FALSE, BOOL IsItalic = FALSE);
	HFONT CreatePointFontIndirect(const LOGFONT &LogFont, HDC DC = nullptr);

	HFONT CreateFont(int Height, int Width, int Escapement,
			int Orientation, int Weight, DWORD Italic, DWORD Underline,
			DWORD StrikeOut, DWORD Charset, DWORD OutPrecision,
			DWORD ClipPrecision, DWORD Quality, DWORD PitchAndFamily,
			LPCTSTR FaceName);

	LOGFONT GetLogFont() const;
};