/*
*    Copyright (c) Vanderbilt University,  2011
*    ALL RIGHTS RESERVED
*
*/ 



#include "StdAfx.h"

#include <vector>
#include <algorithm>

#include "DecoratorSmart.h"
#include "Decorator.h"
#include "CPMDecoratorStyle.h"
#include "CPMAttributePart.h"

namespace Decor {


void CPMDecoratorStyle::AddInternalPart(CPMInternalTextPart* attribute)
{
	m_pBasePart->AddInternalPart(attribute, attribute);
	attribute->InitializeEx(m_pProject, m_pPart, m_pFCO, m_parentWnd, m_preferences);
}

static CString getAttribute(const CComPtr<IMgaFCO> pFCO, const CString& attrName)
{
	CString attrStr;
	DecoratorSDK::getFacilities().getAttribute(pFCO, attrName, attrStr);
	return attrStr;
}


CString CPMDecoratorStyle::GetAttribute(const CString& attrName) const
{
	return getAttribute(m_pFCO, attrName);
}

CPMNamePart* CPMDecoratorStyle::NewCPMNamePart(int iFontKey,  COLORREF cColorKey)
{
	CComBSTR bName;
	COMTHROW(m_pFCO->get_Name(&bName));
    CString cName = bName;
	m_preferences[DecoratorSDK::PREF_LABELFONT] = PreferenceVariant((long)iFontKey);
	m_preferences[DecoratorSDK::PREF_LABELCOLOR] = PreferenceVariant(cColorKey);
	return new CPMNamePart(m_pBasePart, m_eventSink, cName, m_pFCO);
}

void CPMDecoratorStyle::AddCPMUnitsPart(int iFontKey,  COLORREF cColorKey)
{
	CComPtr<IMgaFCO> spReferenced;
	CComQIPtr<IMgaReference> spReference = m_pFCO;
	if (spReference)
	{
		COMTHROW(spReference->get_Referred(&spReferenced));
		if (spReferenced)
		{
			_bstr_t kind = spReferenced->Meta->Name;
			if (wcsstr(static_cast<const wchar_t*>(kind), L"_unit"))
			{
				CString label = getAttribute(spReferenced, "Abbreviation");
				if (label == "")
					label = getAttribute(spReferenced, "Symbol");
				if (label == "")
					label = static_cast<const TCHAR*>(spReferenced->Name);
				//return new CPMNamePart(m_pBasePart, m_eventSink, cName, m_pFCO);
				AddSpacer(5);
				AddInternalPart(new CPMLabelPart(m_pBasePart, m_eventSink, label, m_pFCO));
			}
		}
	}
}

CPMAttributePart* CPMDecoratorStyle::NewAttributePart(const CString& attrName, bool multiline, int iFontKey,  COLORREF cColorKey)
{
	m_preferences[DecoratorSDK::PREF_LABELFONT] = PreferenceVariant((long)iFontKey);
	m_preferences[DecoratorSDK::PREF_LABELCOLOR] = PreferenceVariant(cColorKey);
	return new CPMAttributePart(m_pBasePart, m_eventSink, GetAttribute(attrName), attrName, m_pFCO, multiline);
}

static CString getIntegerAttribute(const CComPtr<IMgaFCO> pFCO, const CString& attrName)
{
	long attrValue;
	DecoratorSDK::getFacilities().getAttribute(pFCO, attrName, attrValue);
	CString attrString;
	attrString.Format(L"%ld", attrValue);
	return attrString;
}


CPMIntegerAttributePart* CPMDecoratorStyle::NewIntegerAttributePart(const CString& attrName, int iFontKey,  COLORREF cColorKey)
{
	m_preferences[DecoratorSDK::PREF_LABELFONT] = PreferenceVariant((long)iFontKey);
	m_preferences[DecoratorSDK::PREF_LABELCOLOR] = PreferenceVariant(cColorKey);
	CString attrString = getIntegerAttribute(m_pFCO, attrName);
	return new CPMIntegerAttributePart(m_pBasePart, m_eventSink, attrString, attrName, m_pFCO);
}

void CPMDecoratorStyle::AddAttributePart(const CString& attrName, bool multiline, int iFontKey,  COLORREF cColorKey) 
{
	AddInternalPart(NewAttributePart(attrName, multiline, iFontKey,cColorKey));
}

void CPMDecoratorStyle::AddLabel(const CString& label, int iFontKey,  COLORREF cColorKey)
{
	m_preferences[DecoratorSDK::PREF_LABELFONT] = PreferenceVariant((long)iFontKey);
	m_preferences[DecoratorSDK::PREF_LABELCOLOR] = PreferenceVariant(cColorKey);
	AddInternalPart(new CPMLabelPart(m_pBasePart, m_eventSink, label, m_pFCO));
}

void CPMDecoratorStyle::AddSpacer(long x, long y)
{
	m_pBasePart->AddInternalPart(new InternalPartSpacer(x, y));
}

void CPMDecoratorStyle::RenderParameter()
{
	AddLabel("Param", 0, PARAM_TEXT_COLOR);
	AddSpacer(5);
	AddInternalPart(NewCPMNamePart(CPM_FONT_BOLD_INDEX));
	RenderValueUnit();

	CString range = GetAttribute(CPM_PARAMETER_RANGE_ATTR);
	if (range != "-inf..inf") {
		m_pBasePart->NewInternalPartRow();
		AddSpacer(10);
		if (range.Find('[')==-1)
			AddLabel("Range: [ ");
		else
			AddLabel("Range: ");
		AddSpacer(5);
		AddAttributePart(CPM_PARAMETER_RANGE_ATTR);				
		AddSpacer(5);
		if (range.Find(']')==-1)
			AddLabel("]");

	}
}

void CPMDecoratorStyle::RenderValueUnit()
{
	CString val = GetAttribute(CPM_PARAMETER_VALUE_ATTR);
	if (val != "") {
		AddLabel(": ");
		AddSpacer(5);
		AddAttributePart(CPM_PARAMETER_VALUE_ATTR);
	}		
	AddCPMUnitsPart(CPM_FONT_ITALICS_INDEX);
}

void CPMDecoratorStyle::AddInternalParts() {
	// This preference tells TextPart to use the string in m_strText
	m_preferences[DecoratorSDK::PREF_TEXTOVERRIDE] = PreferenceVariant(true);
	m_pBasePart->NewInternalPartRow();
	switch (m_cpmType) 
	{
		case Type::carparameter_type:
			{
				AddLabel("CarParam", 0, PARAM_TEXT_COLOR);
				AddSpacer(5);
				AddInternalPart(NewCPMNamePart(CPM_FONT_BOLD_INDEX));
				AddLabel(": ");
				AddAttributePart(CPM_PARAMETER_VALUE_ATTR);
				AddCPMUnitsPart(CPM_FONT_ITALICS_INDEX);
				break;
			}
		case Type::parameter_type:
			{
				RenderParameter();
				break;
			}
		case Type::property_type:
			{
				AddLabel("Prop", 0, PROP_TEXT_COLOR);
				AddSpacer(5);
				AddInternalPart(NewCPMNamePart(CPM_FONT_BOLD_INDEX));
				AddLabel(": ");
				AddAttributePart(CPM_PROPERTY_VALUE_ATTR);
				AddCPMUnitsPart(CPM_FONT_ITALICS_INDEX);
				CString tol = GetAttribute(CPM_PROPERTY_TOLERANCE_ATTR);
				if (tol != "") {
					m_pBasePart->NewInternalPartRow();
					AddSpacer(10);
					AddLabel("Tolerance: ");
					AddSpacer(5);
					AddAttributePart(CPM_PROPERTY_TOLERANCE_ATTR);
				}
				break;
			}
		case Type::metric_type:
			{
				AddLabel("Metric", 0, COLOR_GRAY);
				AddSpacer(10);
				AddInternalPart(NewCPMNamePart(CPM_FONT_BOLD_INDEX));
				RenderValueUnit();
				break;
			}
		default:
			AddInternalPart(NewCPMNamePart(CPM_FONT_BOLD_INDEX));
			break;
	}
}


};

