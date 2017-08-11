#pragma once

#include "afxcmn.h"

// DisplayEdit

class DisplayEdit : public CEdit
{
	DECLARE_DYNAMIC(DisplayEdit)

public:
	DisplayEdit();
	virtual ~DisplayEdit();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point); 

protected:
	DECLARE_MESSAGE_MAP()
};


