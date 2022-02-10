#include "GDI.h"
#include "Application.h"
#include "WinControl.h"

// Constructs the TGDIObject
TGDIObject::TGDIObject()
{
	FData = new TGDIData;
}

// Note: A copy of a TGDIObject is a clone of the original.
//       Both objects manipulate the one HGDIOBJ.
TGDIObject::TGDIObject(const TGDIObject &RHS)
{
	FData = RHS.FData;
	InterlockedIncrement(&FData->Count);
}

// Deconstructs the TGDIObject
TGDIObject::~TGDIObject()
{
	Release();
}

// Note: A copy of a TGDIObject is a clone of the original.
//       Both objects manipulate the one HGDIOBJ.
TGDIObject &TGDIObject::operator = (const TGDIObject &RHS)
{
	if (this != &RHS)
	{
		InterlockedIncrement(&RHS.FData->Count);
		Release();
		FData = RHS.FData;
	}

	return *this;
}

void TGDIObject::operator = (const HGDIOBJ Object)
{
	Attach(Object);
}

// Store the HDC and CDC pointer in the HDC map
void TGDIObject::AddToMap()
{
	GetApp()->AddGDIData(FData->GDIObject, FData);
}

// Attaches a GDI HANDLE to the TGDIObject.
void TGDIObject::Attach(HGDIOBJ Object)
{
	if (FData && Object != FData->GDIObject)
	{
		// Release any existing GDI Object.
		if (FData->GDIObject != 0)
		{
			Release();
			FData = new TGDIData;
		}

		if (Object)
		{
			// Add the GDI Object to this CCGDIObject.
			TGDIData *GDIDataPtr = GetApp()->GetGDIData(Object);
			if (GDIDataPtr)
			{
				delete FData;
				FData = GDIDataPtr;
				InterlockedIncrement(&FData->Count);
			}
			else
			{
				FData->GDIObject = Object;
				AddToMap();
			}
		}
	}
}

void TGDIObject::DeleteObject()
{
	if (FData && FData->GDIObject != 0)
	{
		RemoveFromMap();

		::DeleteObject(FData->GDIObject);
		FData->GDIObject = 0;
	}
}

// Detaches the HGDIOBJ from this TGDIObject and all its copies.
// The TGDIObject and its copies are returned to the default state.
// Note: We rarely need to detach the HGDIOBJ from TGDIObject.
//       The framework will delete the HGDIOBJ automatically if required
//       when the last copy of the CDC goes out of scope.
//       This also applies to classes inherited from TGDIObject, namely
//       CBitmap, CBrush, TFont, CPalette, CPen and CRgn.
HGDIOBJ TGDIObject::Detach()
{

	HGDIOBJ Object = FData->GDIObject;
	RemoveFromMap();
	FData->GDIObject = 0;
	SetManaged(false);

	if (FData->Count > 0)
	{
		if (::InterlockedDecrement(&FData->Count) == 0)
		{
			delete FData;
		}
	}

	FData = new TGDIData;
	return Object;
}

// Returns the GDI handle (HGDIOBJ) associated with this Object.
HGDIOBJ TGDIObject::Handle() const
{
	return FData ? FData->GDIObject : 0;
}

// Retrieves information for the specified graphics Object.
// Refer to GetObject in the Windows API documentation for more information.
int TGDIObject::GetObject(int Count, LPVOID ObjectPtr) const
{
	return FData ? ::GetObject(FData->GDIObject, Count, ObjectPtr) : 0;
}

// Decrements the reference Count.
// Destroys FData if the reference Count is zero.
void TGDIObject::Release()
{

	if (FData && ::InterlockedDecrement(&FData->Count) == 0)
	{
		if (FData->GDIObject != 0)
		{
			if (FData->IsManagedObject)
			{
				::DeleteObject(FData->GDIObject);
			}

			RemoveFromMap();
		}

		delete FData;
		FData = 0;
	}
}

void TGDIObject::RemoveFromMap()
{
	GetApp()->DelGDIData(FData->GDIObject);
}

TFont::TFont()
{
}

TFont::TFont(HFONT Font)
{
	Attach(Font);
}

TFont::TFont(const LOGFONT &LogFont)
{
	try
	{
		CreateFontIndirect(LogFont);
	}

	catch(...)
	{
		Release();  // Cleanup
		throw;      // Rethrow;
	}
}

TFont::operator HFONT() const
{
	return static_cast<HFONT>(Handle());
}

TFont::~TFont()
{
}

// Creates a logical Font that has the specified characteristics.
// Refer to CreateFontIndirect in the Windows API documentation for more information.
HFONT TFont::CreateFontIndirect(const LOGFONT &LogFont)
{
	HFONT Font = ::CreateFontIndirect(&LogFont);
	if (Font == 0)
			return nullptr;

	Attach(Font);
	SetManaged(true);
	return Font;
}

// Creates a Font of a specified typeface and point size.
// Refer to CreateFontIndirect in the Windows API documentation for more information.
HFONT TFont::CreatePointFont(int PointSize, LPCTSTR FaceName, HDC DC /*= 0*/, BOOL IsBold /*= FALSE*/, BOOL IsItalic /*= FALSE*/)
{
	LOGFONT LogFont;
	ZeroMemory(&LogFont, sizeof(LogFont));
	LogFont.lfCharSet = DEFAULT_CHARSET;
	LogFont.lfHeight = PointSize;

	StrCopy(LogFont.lfFaceName, FaceName, LF_FACESIZE);

	if (IsBold)
		LogFont.lfWeight = FW_BOLD;
	if (IsItalic)
		LogFont.lfItalic = 1;

	return CreatePointFontIndirect(LogFont, DC);
}

// Creates a Font of a specified typeface and point size.
// This function automatically converts the Height in lfHeight to logical
// units using the specified device context.
// Refer to CreateFontIndirect in the Windows API documentation for more information.
HFONT TFont::CreatePointFontIndirect(const LOGFONT &LogFont, HDC DC /* = 0*/)
{
	DC = (DC != 0) ? DC : ::GetDC(HWND_DESKTOP);

	// convert nPointSize to logical units based on hDC
	LOGFONT LogFontTmp = LogFont;

	TPoint Point(0, 0);
	Point.y = ::MulDiv(::GetDeviceCaps(DC, LOGPIXELSY), LogFont.lfHeight, 720);   // 72 points/inch, 10 decipoints/point
	::DPtoLP(DC, &Point, 1);

	TPoint PointOrg = { 0, 0 };
	::DPtoLP(DC, &PointOrg, 1);

	LogFontTmp.lfHeight = -abs(Point.y - PointOrg.y);


	return CreateFontIndirect(LogFontTmp);
}

// Creates a logical Font with the specified characteristics.
// Refer to CreateFont in the Windows API documentation for more information.
HFONT TFont::CreateFont(int Height, int Width, int Escapement,
		int Orientation, int Weight, DWORD Italic, DWORD Underline,
		DWORD StrikeOut, DWORD Charset, DWORD OutPrecision,
		DWORD ClipPrecision, DWORD Quality, DWORD PitchAndFamily,
		LPCTSTR FaceName)
{
	HFONT Font = ::CreateFont(Height, Width, Escapement,
		Orientation, Weight, Italic, Underline, StrikeOut,
		Charset, OutPrecision, ClipPrecision, Quality,
		PitchAndFamily, FaceName);

	if (Font == 0)
		return nullptr;

	Attach(Font);
	SetManaged(true);
	return Font;
}

// Retrieves the Logfont structure that contains Font attributes.
// Refer to GetObject in the Windows API documentation for more information.
LOGFONT TFont::GetLogFont() const
{
	LOGFONT LogFont;
	ZeroMemory(&LogFont, sizeof(LogFont));
	::GetObject(Handle(), sizeof(LogFont), &LogFont);
	return LogFont;
}