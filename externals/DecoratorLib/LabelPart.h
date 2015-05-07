//################################################################################################
//
// Label part class (decorator part)
//	LabelPart.h
//
//################################################################################################

#ifndef __LABELPART_H_
#define __LABELPART_H_


#include "StdAfx.h"
#include "TextPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : LabelPart
//
//################################################################################################

class LabelPart: public TextPart
{
protected:
	std::vector<CString>		m_vecText;
public:
	LabelPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink);
	virtual ~LabelPart();

	virtual void	Draw						(CDC* pDC, Gdiplus::Graphics* gdip);

	virtual void InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);
	virtual CPoint	GetTextPosition				(CDC* pDC, Gdiplus::Graphics* gdip) const;
	virtual void	SetTextRelYPosition			(long relYPosition) {};	// This doesn't make sense in case of box label decorator: label text location is computed automatically from box location and text alignment
	virtual CRect	GetTextLocation				(CDC* pDC, Gdiplus::Graphics* gdip) const;
	virtual void	ExecuteOperation			(const CString& newString);
	virtual long	GetLongest				(void) const;
};

}; // namespace DecoratorSDK

#endif //__LABELPART_H_
