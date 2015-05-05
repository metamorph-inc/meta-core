// DialogTemplate.cpp : implementation file
//

#include "stdafx.h"
#include "DialogTemplate.h"

CDialogTemplate::CDialogTemplate(LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y, int w, int h,
								 LPCTSTR font, WORD fontSize)
{
	usedBufferLength = sizeof(DLGTEMPLATE);
	totalBufferLength = 8 * usedBufferLength; // FIXME: this appears to be a guess

	dialogTemplate = (DLGTEMPLATE*)(new char[totalBufferLength]);

	dialogTemplate->style = style;

	if (font != NULL)
	{
		dialogTemplate->style |= DS_SETFONT;
	}

	dialogTemplate->x     = x;
	dialogTemplate->y     = y;
	dialogTemplate->cx    = w;
	dialogTemplate->cy    = h;
	dialogTemplate->cdit  = 0;

	dialogTemplate->dwExtendedStyle = exStyle;

	// The dialog box doesn't have a menu or a special class

	AppendData("\0", 2);
	AppendData("\0", 2);

	// Add the dialog's caption to the template

	AppendString(caption);

	if (font != NULL)
	{
		AppendData(&fontSize, sizeof(WORD));
		AppendString(font);
	}
}

//
// Returns a pointer to the Win32 dialog template which the object
// represents. This pointer may become invalid if additional
// components are added to the template.
//

CDialogTemplate::operator const DLGTEMPLATE*() const
{
	return dialogTemplate;
}

CDialogTemplate::~CDialogTemplate()
{
	delete [] ((char*)dialogTemplate);
}

void CDialogTemplate::AddButton(LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y,
								int w, int h, WORD id)
{
	AddStandardComponent(0x0080, caption, style, exStyle, x, y, w, h, id);

	WORD creationDataLength = 0;
	AppendData(&creationDataLength, sizeof(WORD));
}

void CDialogTemplate::AddEditBox(LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y,
								 int w, int h, WORD id)
{
	AddStandardComponent(0x0081, caption, style, exStyle, x, y, w, h, id);

	WORD creationDataLength = 0;
	AppendData(&creationDataLength, sizeof(WORD));
}

void CDialogTemplate::AddStatic(LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y,
								int w, int h, WORD id)
{
	AddStandardComponent(0x0082, caption, style, exStyle, x, y, w, h, id);

	WORD creationDataLength = 0;
	AppendData(&creationDataLength, sizeof(WORD));
}

void CDialogTemplate::AddListBox(LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y,
								 int w, int h, WORD id)
{
	AddStandardComponent(0x0083, caption, style, exStyle, x, y, w, h, id);

	WORD creationDataLength = 0;
	AppendData(&creationDataLength, sizeof(WORD));
}

void CDialogTemplate::AddScrollBar(LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y,
								   int w, int h, WORD id)
{
	AddStandardComponent(0x0084, caption, style, exStyle, x, y, w, h, id);

	WORD creationDataLength = 0;
	AppendData(&creationDataLength, sizeof(WORD));
}

void CDialogTemplate::AddComboBox(LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y,
								  int w, int h, WORD id)
{
	AddStandardComponent(0x0085, caption, style, exStyle, x, y, w, h, id);

	WORD creationDataLength = 0;
	AppendData(&creationDataLength, sizeof(WORD));
}

void CDialogTemplate::AddRichEdit(LPCTSTR caption, DWORD style, DWORD exStyle, int x, int y,
								  int w, int h, WORD id)
{
	AddComponent(0x0081, RICHEDIT_CLASS, caption, style, exStyle, x, y, w, h, id);

	WORD creationDataLength = 0;
	AppendData(&creationDataLength, sizeof(WORD));
}

void CDialogTemplate::AddStandardComponent(WORD type, LPCTSTR caption, DWORD style,
										   DWORD exStyle, int x, int y, int w, int h, WORD id)
{
	AddComponent(type, NULL, caption, style, exStyle, x, y, w, h, id);
}

void CDialogTemplate::AddComponent(WORD type, LPCTSTR classId, LPCTSTR caption, DWORD style,
								   DWORD exStyle, int x, int y, int w, int h, WORD id)
{
	DLGITEMTEMPLATE item;

	// DWORD algin the beginning of the component data

	AlignData(sizeof(DWORD));

	item.style = style;
	item.x     = x;
	item.y     = y;
	item.cx    = w;
	item.cy    = h;
	item.id    = id;

	item.dwExtendedStyle = exStyle;

	AppendData(&item, sizeof(DLGITEMTEMPLATE));

	if (classId == NULL) {
		WORD preType = 0xFFFF;

		AppendData(&preType, sizeof(WORD));
		AppendData(&type, sizeof(WORD));
	} else {
		AppendString(classId);
	}

	AppendString(caption);

	// Increment the component count

	dialogTemplate->cdit++;
}

void CDialogTemplate::AlignData(int size)
{
	int paddingSize = usedBufferLength % size;

	if (paddingSize != 0)
	{
		EnsureSpace(paddingSize);
		usedBufferLength += paddingSize;
	}
}

void CDialogTemplate::AppendString(LPCWSTR string)
{
	AppendData(string, (wcslen(string) + 1) * sizeof(wchar_t));
}

void CDialogTemplate::AppendString(LPCSTR string)
{
	int length = MultiByteToWideChar(CP_ACP, 0, string, -1, NULL, 0);

	WCHAR* wideString = (WCHAR*)malloc(sizeof(WCHAR) * length);
	MultiByteToWideChar(CP_ACP, 0, string, -1, wideString, length);

	AppendData(wideString, length * sizeof(WCHAR));
	free(wideString);
}

void CDialogTemplate::AppendData(const void* data, int dataLength)
{
	EnsureSpace(dataLength);

	memcpy((char*)dialogTemplate + usedBufferLength, data, dataLength);
	usedBufferLength += dataLength;
}

void CDialogTemplate::EnsureSpace(int length)
{

	if (length + usedBufferLength > totalBufferLength)
	{
		totalBufferLength += length * 2;

		char* newBuffer = new char[totalBufferLength];
		memcpy(newBuffer, dialogTemplate, usedBufferLength);

		delete [] ((char*)dialogTemplate);
		dialogTemplate = (DLGTEMPLATE*)newBuffer;
	}
}
