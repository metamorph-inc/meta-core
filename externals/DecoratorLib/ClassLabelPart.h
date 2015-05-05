//################################################################################################
//
// Class label part class (decorator part)
//	ClassLabelPart.h
//
//################################################################################################

#ifndef __CLASSLABELPART_H_
#define __CLASSLABELPART_H_


#include "StdAfx.h"
#include "TextPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ClassLabelPart
//
//################################################################################################

class ClassLabelPart: public TextPart
{
protected:
	long		m_labelRelYPosition;	// bottom of the text because of the bottom alignment
	bool		m_bIsAbstract;

public:
	ClassLabelPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, bool bIsAbstract);
	virtual ~ClassLabelPart();

	virtual void	Draw						(CDC* pDC, Gdiplus::Graphics* gdip);
	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);

	virtual CPoint	GetTextPosition				(CDC* pDC, Gdiplus::Graphics* gdip) const;
	virtual void	SetTextRelYPosition			(long relYPosition) { m_labelRelYPosition = relYPosition; };
	virtual CRect	GetTextLocation				(CDC* pDC, Gdiplus::Graphics* gdip) const;
	virtual void	ExecuteOperation			(const CString& newString);
};

}; // namespace DecoratorSDK

#endif //__CLASSLABELPART_H_
