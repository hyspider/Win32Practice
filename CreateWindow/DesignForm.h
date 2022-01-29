#ifndef __DESIGN_FORM__
#define __DESIGN_FORM__

#include <tchar.h>
#include "BaseForm.h"

class TDesignForm : public TBaseForm<TDesignForm>
{
	public:
		PCWSTR ClassName() const { return _T("Design Form"); }
		LRESULT HandleMessage(UINT Msg, WPARAM WParam, LPARAM LParam);
};

#endif

