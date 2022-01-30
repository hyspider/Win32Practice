#pragma once

#include <tchar.h>
#include "BaseForm.h"

class TMainForm : public TBaseForm<TMainForm>
{
	public:
		TMainForm() : FMaxHeight(200) {}
		void InitControls();
	public:
		LPCTSTR ClassName() const { return _T("Sample Form Class"); }
		LRESULT CALLBACK HandleMessage(UINT Msg, WPARAM WParam, LPARAM LParam);
	public:
		DWORD MaxHeight () const { return FMaxHeight; };
	protected:
		BOOL NewProjBtnClick();
	private:
		TButton FNewProjectButton;
		DWORD FMaxHeight;
};
