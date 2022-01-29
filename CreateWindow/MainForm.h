#ifndef __MAIN_FORM__
#define __MAIN_FORM__
#include <tchar.h>
#include "BaseForm.h"

class TMainForm : public TBaseForm<TMainForm>
{
	public:
		PCWSTR ClassName() const { return _T("Sample Form Class"); }
		LRESULT HandleMessage(UINT Msg, WPARAM WParam, LPARAM LParam);
};

#endif

