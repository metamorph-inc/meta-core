// UIComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "UIComboBox.h"


// UIComboBox

IMPLEMENT_DYNAMIC(UIComboBox, CComboBox)

UIComboBox::UIComboBox()
{

}

UIComboBox::~UIComboBox()
{
}


BEGIN_MESSAGE_MAP(UIComboBox, CComboBox)
END_MESSAGE_MAP()



// UIComboBox message handlers
INT UIComboBox::FindStringExact(INT start, LPCTSTR str)
{
	if(start < -1 || start > this->GetCount()-1) return CB_ERR;

	for(INT i= start==-1 ? 0: start; i<this->GetCount(); ++i)
	{
		CString itemText;
		this->GetLBText(i, itemText);

		if(!itemText.Compare(str))
			return i;
	}

	return CB_ERR;
}

INT UIComboBox::SelectString(INT start, LPCTSTR str)
{
	return this->SetCurSel((this->FindStringExact(start, str)));
}

