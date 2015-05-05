#if !defined(AFX_DIALOGTEMPLATE_H__38B753AD_BDF3_4f9b_8224_1AB10FA65608__INCLUDED_)
#define AFX_DIALOGTEMPLATE_H__38B753AD_BDF3_4f9b_8224_1AB10FA65608__INCLUDED_

// from Max McGuire
// http://www.flipcode.com/archives/Dialog_Template.shtml

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTemplate.h : header file
//

#include "stdafx.h"

// CDialogTemplate class

class CDialogTemplate
{
public:
	CDialogTemplate(LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h,
					LPCTSTR font = NULL, WORD fontSize = 8);
	virtual ~CDialogTemplate();
	operator const DLGTEMPLATE*() const;

	void AddButton		(LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id);
	void AddEditBox		(LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id);
	void AddStatic		(LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id);
	void AddListBox		(LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id);
	void AddScrollBar	(LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id);
	void AddComboBox	(LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id);
	void AddRichEdit	(LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id);

protected:
	void AddStandardComponent(WORD type, LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id);
	void AddComponent(WORD type, LPCTSTR classId, LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h, WORD id);
	void AlignData(int size);
	void AppendString(LPCWSTR string);
	void AppendString(LPCSTR string);
	void AppendData(const void* data, int dataLength);
	void EnsureSpace(int length);

private:
	DLGTEMPLATE* dialogTemplate;

	int totalBufferLength;
	int usedBufferLength;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTEMPLATE_H__38B753AD_BDF3_4f9b_8224_1AB10FA65608__INCLUDED_)
