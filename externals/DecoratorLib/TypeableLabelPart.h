//################################################################################################
//
// Typeable Label part class (decorator part)
//	TypeableLabelPart.h
//
//################################################################################################

#ifndef __TYPEABLELABELPART_H_
#define __TYPEABLELABELPART_H_


#include "StdAfx.h"
#include "LabelPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : TypeableLabelPart
//
//################################################################################################

class TypeableLabelPart: public LabelPart
{
protected:
	bool				m_bTypeNameEnabled;
	CString				m_strTypeName;
	int					m_iTypeInfo;

public:
	TypeableLabelPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink);
	virtual ~TypeableLabelPart();

	virtual void	Draw						(CDC* pDC, Gdiplus::Graphics* gdip);
	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);

	virtual CPoint	GetTextPosition				(CDC* pDC, Gdiplus::Graphics* gdip) const;
	virtual CRect	GetTextLocation				(CDC* pDC, Gdiplus::Graphics* gdip) const;
};

}; // namespace DecoratorSDK

#endif //__TYPEABLELABELPART_H_
