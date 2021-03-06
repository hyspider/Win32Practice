#pragma once

#include <tchar.h>
#include "BaseForm.h"

class TDesignForm : public TBaseForm<TDesignForm>
{
	public:
		LPCTSTR ClassName() const { return _T("Design Form"); }
		LRESULT HandleMessage(UINT Msg, WPARAM WParam, LPARAM LParam);
};
