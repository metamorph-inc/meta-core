#pragma once


// UIComboBox

class UIComboBox : public CComboBox
{
	DECLARE_DYNAMIC(UIComboBox)

public:
	UIComboBox();
	virtual ~UIComboBox();
	/// <summary>
	/// Overridden inherited method to include case-sensitivity. 
	/// Returns index at which the string is found.
	/// Returns CB_ERR when string not found or start < -1 or start > number of strings
	/// </summary>
	INT FindStringExact(INT start, LPCTSTR str);	
	/// <summary>
	/// Overridden inherited method to include case-sensitivity. 
	/// Returns index at which the string is found.
	/// Returns CB_ERR when string not found or start < -1 or start > number of strings
	/// </summary>
	INT SelectString(INT start, LPCTSTR str);	
protected:
	DECLARE_MESSAGE_MAP()
};


