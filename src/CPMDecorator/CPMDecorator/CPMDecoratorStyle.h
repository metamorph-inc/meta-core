/*
*    Copyright (c) Vanderbilt University,  2011
*    ALL RIGHTS RESERVED
*
*/ 



#ifndef __CPMDECORATORSTYLE_H_
#define __CPMDECORATORSTYLE_H_

#include "StdAfx.h"
#include "PartInterface.h"
#include "Decorator.h"
#include "CPMBasePart.h"

namespace Decor {

	class CPMDecoratorStyle
	{
		CComPtr<IMgaCommonDecoratorEvents> m_eventSink;
		CComPtr<IMgaFCO> m_pFCO;
		CPMBasePart* m_pBasePart;
		Type::cpm_type m_cpmType;
		DecoratorSDK::PreferenceMap m_preferences;
		HWND m_parentWnd;
		CComPtr<IMgaMetaPart> m_pPart;
		CComPtr<IMgaProject> m_pProject;

		CString GetAttribute(const CString& attrName) const;
		CPMNamePart* NewCPMNamePart(int iFontKey = FONT_LABEL,  COLORREF cColorKey = COLOR_BLACK);
		void AddCPMUnitsPart(int iFontKey = FONT_LABEL,  COLORREF cColorKey = COLOR_BLACK);
		CPMAttributePart* NewAttributePart(const CString& attrName, bool multiline = false, int iFontKey = FONT_LABEL,  COLORREF cColorKey = COLOR_BLACK);
		CPMIntegerAttributePart* NewIntegerAttributePart(const CString& attrName, int iFontKey = FONT_LABEL,  COLORREF cColorKey = COLOR_BLACK);
		void AddInternalPart(CPMInternalTextPart* attribute);
		void AddAttributePart(const CString& attrName, bool multiline = false, int iFontKey = CPM_FONT_BOLD_INDEX,  COLORREF cColorKey = COLOR_BLUE);
		void AddLabel(const CString& label, int iFontKey = FONT_LABEL,  COLORREF cColorKey = COLOR_BLACK);
		void AddSpacer(long x, long y = 0);
		void RenderParameter();
		void RenderValueUnit();
		
	public:
		CPMDecoratorStyle(CComPtr<IMgaCommonDecoratorEvents> eventSink, CComPtr<IMgaFCO> pFCO,
			CPMBasePart* pBasePart, Type::cpm_type cpmType, HWND parentWnd, CComPtr<IMgaMetaPart>& pPart,
			CComPtr<IMgaProject>& pProject) :
		m_eventSink(eventSink), m_pFCO(pFCO), m_pBasePart(pBasePart), m_cpmType(cpmType),
		m_parentWnd(parentWnd), m_pPart(pPart), m_pProject(pProject) { }


		void AddInternalParts();
	};

};

#endif // __CPMDECORATORSTYLE_H_
