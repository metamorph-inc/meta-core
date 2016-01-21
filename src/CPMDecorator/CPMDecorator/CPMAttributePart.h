/*
*    Copyright (c) Vanderbilt University,  2011
*    ALL RIGHTS RESERVED
*
*/ 


//################################################################################################
//
// CPM attribute part class (decorator part)
//	CPMAttributePart.h
//
//################################################################################################

#ifndef __CPMATTRIBUTEPART_H_
#define __CPMATTRIBUTEPART_H_


#include "StdAfx.h"
#include "PartBase.h"
#include "TextPart.h"
#include "DecoratorDefs.h"

#include "Decorator.h"

namespace Decor {

class IInternalPart {
public:
	virtual void SetTextRelYPosition (long relYPosition) = 0;
	virtual void SetTextRelXPosition (long relXPosition) = 0;
	virtual CSize GetRelSize(CDC* pDC, Gdiplus::Graphics* gdip) const = 0;
	virtual CPoint GetTextRelPosition() const = 0;
};

class InternalPartSpacer: public IInternalPart {
	CPoint m_pos;
	CSize m_size;
public:
	InternalPartSpacer(long x, long y = 0) { m_size.cx = x; m_size.cy = y; }
	virtual void SetTextRelYPosition (long relYPosition) { m_pos.y = relYPosition; }
	virtual void SetTextRelXPosition (long relXPosition) { m_pos.x = relXPosition; }
	virtual CSize GetRelSize(CDC* pDC, Gdiplus::Graphics* gdip) const { return m_size; }
	virtual CPoint GetTextRelPosition() const { return m_pos; }
};

class CPMInternalTextPart: public DecoratorSDK::TextPart, public IInternalPart
{
protected:
	CComPtr<IMgaFCO> m_spActualFCO;
	long m_textRelYPosition;	// bottom edge of the text because of the bottom alignment
	long m_textRelXPosition;	// left edge of the text

public:
	CPMInternalTextPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents> eventSink,
					 const CString& strText, CComPtr<IMgaFCO>& pFCO, bool bMultiLine,
					 int iFontKey = DecoratorSDK::FONT_LABEL);
	virtual ~CPMInternalTextPart();

	virtual void Draw(CDC* pDC, Gdiplus::Graphics* gdip);
	virtual void SetTextRelYPosition (long relYPosition) { m_textRelYPosition = relYPosition; };
	virtual void SetTextRelXPosition (long relXPosition) { m_textRelXPosition = relXPosition; };
	virtual CPoint GetTextRelPosition() const { CPoint point; point.x = m_textRelXPosition; point.y = m_textRelYPosition; return point; }
	virtual CPoint GetTextPosition(CDC* pDC, Gdiplus::Graphics* gdip) const;
	virtual CRect GetTextLocation(CDC* pDC, Gdiplus::Graphics* gdip) const;
	virtual CSize GetRelSize(CDC* pDC, Gdiplus::Graphics* gdip) const;
	virtual void ExecuteOperation (const CString& newString) = 0;
	virtual void InitializeEx (CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
		CComPtr<IMgaFCO>& pFCO, HWND parentWnd, DecoratorSDK::PreferenceMap& preferences);

	virtual bool MouseLeftButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC) {
		if (_MSC_VER < 1500) {
			return false;
		} else {
			return DecoratorSDK::TextPart::MouseLeftButtonDown(nFlags, point, transformHDC);
		}
	}
	virtual bool MouseLeftButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC) {
		if (_MSC_VER < 1500) {
			return false;
		} else {
			return DecoratorSDK::TextPart::MouseLeftButtonDoubleClick(nFlags, point, transformHDC);
		}
	}

	virtual CSize GetTextSize(Gdiplus::Graphics* gdip, DecoratorSDK::GdipFont* pFont) const
	{
		if (m_bTextEditable == false || m_strText.GetLength() > 0)
			return DecoratorSDK::getFacilities().MeasureText(gdip, pFont, m_strText);

		CSize size = DecoratorSDK::getFacilities().MeasureText(gdip, pFont, L"m");
		size.cx = 20;
		return size;
	}

};

class CPMLabelPart: public CPMInternalTextPart {
public:
	CPMLabelPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents> eventSink,
				 const CString& strText, CComPtr<IMgaFCO>& pFCO,
				 int iFontKey = DecoratorSDK::FONT_LABEL) :
		CPMInternalTextPart(pPart, eventSink, strText, pFCO, false, iFontKey)
	{
		m_bTextEditable = false;
	}
	virtual void ExecuteOperation (const CString& newString) { };
};

class CPMAttributePart: public CPMInternalTextPart {
	CString m_attrName;
public:
	CPMAttributePart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents> eventSink,
		const CString& strText, const CString& attrName, CComPtr<IMgaFCO>& pFCO, bool bMultiLine) :
		CPMInternalTextPart(pPart, eventSink, strText, pFCO, bMultiLine),
			m_attrName(attrName) { }

	virtual void ExecuteOperation(const CString& newString)
	{
	// transaction operation begin
	m_strText = newString;
	DecoratorSDK::getFacilities().setAttribute(m_spActualFCO, m_attrName, m_strText);
	// transaction operation end
	}

};

class CPMIntegerAttributePart: public CPMInternalTextPart {
	CString m_attrName;
public:
	CPMIntegerAttributePart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents> eventSink,
		const CString& strText, const CString& attrName, CComPtr<IMgaFCO>& pFCO) :
		CPMInternalTextPart(pPart, eventSink, strText, pFCO, false),
			m_attrName(attrName) { }

	virtual void ExecuteOperation(const CString& newString)
	{
	// transaction operation begin
	m_strText = newString;
	long newValue = _wtol(newString);
	DecoratorSDK::getFacilities().setAttribute(m_spActualFCO, m_attrName, newValue);
	// transaction operation end
	}

};

class CPMNamePart: public CPMInternalTextPart
{
public:
	CPMNamePart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents> eventSink,
				const CString& strText, CComPtr<IMgaFCO>& pFCO) :
		CPMInternalTextPart(pPart, eventSink, strText, pFCO, false) { }

	virtual void ExecuteOperation(const CString& newString)
	{
		m_strText = newString;
		CComBSTR bstr;
		CopyTo(newString, bstr);
		COMTHROW(m_spFCO->put_Name(bstr));
	}
};

}; // namespace Decor

#endif //__CPMATTRIBUTEPART_H_
