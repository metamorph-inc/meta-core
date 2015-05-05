//################################################################################################
//
// Decorator class
//	Decorator.cpp
// This class represents the C++ side of the decorator COM connection layer
// This is still just a relay class like teh COM/ATL DecoratorImpl.h,cpp,
// the meat should go into the composite part or into the parts in the composite part
//
//################################################################################################

#include "StdAfx.h"
#include "Decorator.h"
#include "DecoratorCompositePart.h"
#include "LabelPart.h"
#include "ObjectAndTextPart.h"
#include "TypeableBitmapPart.h"
#include "DiamondVectorPart.h"
#include "ModelComplexPart.h"
#include "Resource.h"
#include "CPMBasePart.h"
#include "CPMDecoratorStyle.h"
#include <map>

#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;
/* Parses (Default.png)?(Aspect=icon.png)* into std::map(""="Default.png", "Aspect"="icon.png",...)
*/
struct IconStringParser
{
public:
	qi::rule<wchar_t*, std::wstring()> filename;
	qi::rule<wchar_t*, std::wstring()> aspect;
	IconStringParser()
	{
		using namespace boost::spirit::standard_wide;
		filename = +(char_(L" -~") - char_(L"<>:\"|?*") - /* disambiguate grammar */char_(L";="));
		aspect = char_(L"a-zA-Z_") >> +(char_(L"a-zA-Z_0-9 "));
	}
	std::map<std::wstring, std::wstring> parse(wchar_t* input);
} iconStringParser;

std::map<std::wstring, std::wstring> IconStringParser::parse(wchar_t* input)
{
	std::map<std::wstring, std::wstring> ret;
	std::wstring aspect;
	std::wstring filename;
	auto assign_filename = [&](std::wstring filename_, qi::unused_type, qi::unused_type){ filename = filename_; };
	auto add_default_mapping = [&](qi::unused_type, qi::unused_type, qi::unused_type){ ret[L""] = filename; };
	auto assign_aspect = [&](std::wstring aspect_, qi::unused_type, qi::unused_type){ aspect = aspect_; };
	auto add_mapping = [&](std::wstring filename, qi::unused_type, qi::unused_type){ ret[aspect] = filename; };
	// n.b. the semantic action for -(filename) may be executed, but then backtracked when ";" is not found. Store the default icon (add_default_mapping) when we see ";"
	qi::parse(input, input + wcslen(input),
		-((this->filename[assign_filename]) >> (qi::lit(";") | qi::eoi)[add_default_mapping]) >>
		*(this->aspect[assign_aspect] >> qi::lit("=") >> (this->filename[add_mapping]) >> (qi::lit(";") | qi::eoi)));
	return ret;
}

PreferenceVariant GetIcon(CComPtr<IMgaFCO> pFCO, CComPtr<IMgaMetaPart>& pPart, _bstr_t default_)
{
	_bstr_t defaultStrIcon;
	_bstr_t strIcon;
	CComPtr<IMgaFCO> fco = pFCO;
	std::wstring aspectName = pPart->GetParentAspect()->Name;
	std::wstring aspectDisplayedName = pPart->GetParentAspect()->DisplayedName;
	auto setStrIcons = [&](_bstr_t& icon)
	{
		auto parseMap = iconStringParser.parse(icon);
		std::wstring* names[] = { &aspectName, &aspectDisplayedName, NULL };
		for (std::wstring** name = &names[0]; *name; name++)
		{
			auto it = parseMap.find(**name);
			if (it != parseMap.end() && it->second.length())
			{
				strIcon = it->second.c_str();
			}
		}
		if (!defaultStrIcon)
		{
			auto it = parseMap.find(L"");
			if (it != parseMap.end())
			{
				defaultStrIcon = it->second.c_str();
			}
		}
	};
	while (fco)
	{
		CComQIPtr<IMgaReference> ref = fco;

		while (fco)
		{
			IMgaRegNodePtr regnode = fco->imp_GetRegistryNodeDisp(PREF_ICON);
			long status = ATTSTATUS_INVALID;
			regnode->get_Status(&status);
			if (status == ATTSTATUS_HERE && regnode->Value.length())
			{
				_bstr_t icon = regnode->Value;
				setStrIcons(icon);
				if (!!strIcon)
				{
					break;
				}
			}
			fco = fco->DerivedFrom;
		}
		if (!!strIcon)
		{
			break;
		}
		fco = NULL;
		if (ref)
		{
			COMTHROW(ref->get_Referred(&fco));
		}
	}
	if (!strIcon)
		strIcon = defaultStrIcon;
	if (!strIcon)
	{
		//pFCO->Meta->GetRegistryValueDisp(_bstr_t(PREF_ICON), strIcon.GetAddress());
		_bstr_t icon = pFCO->Meta->RegistryValue[_bstr_t(PREF_ICON)];
		if (icon.length())
			setStrIcons(icon);
		if (!strIcon)
			strIcon = defaultStrIcon;
	}
	if (!strIcon)
	{
		strIcon = default_;
	}
	/* parser test cases
	auto t1 = iconStringParser.parse(L"");
	auto t2 = iconStringParser.parse(L"Default.png;aspect1=icon1.png;aspect2=icon2.png");
	auto t3 = iconStringParser.parse(L"aspect1=icon1.png;aspect2=icon2.png");
	auto t4 = iconStringParser.parse(L"Default.png;aspect1=icon1.png;aspect2=icon2.png;");
	auto t5 = iconStringParser.parse(L"Default.png");
	*/
	
	return PreferenceVariant(CString(static_cast<const TCHAR*>(strIcon)));
}

namespace Decor {

	//################################################################################################
	//
	// CLASS : Decorator
	//
	//################################################################################################




	void createCPMFontsIfNeeded() {
		if (DecoratorSDK::getFacilities().GetFont(CPM_FONT_ITALICS_BOLD_INDEX) == NULL) {
			DecoratorSDK::getFacilities().createFont(CPM_FONT_ITALICS_INDEX, "Arial", FW_NORMAL, true, 16);
			DecoratorSDK::getFacilities().createFont(CPM_FONT_ITALICS_BOLD_INDEX, "Arial", FW_BOLD, true, 16);
			DecoratorSDK::getFacilities().createFont(CPM_FONT_BOLD_INDEX, "Arial", FW_BOLD, false, 16);
		}
	}

	Decorator::Decorator(CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	m_part(NULL),
		m_eventSink(eventSink)
	{
		createCPMFontsIfNeeded();
		
	}

	Decorator::~Decorator()
	{

		

	}

	void Decorator::Initialize(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO)
	{
		m_part->Initialize(pProject, pPart, pFCO);
	}

	void Decorator::Destroy()
	{
		if (m_part) {
			m_part->Destroy();
			delete m_part;
			m_part = NULL;
		}
	}

	CString Decorator::GetMnemonic(void) const
	{
		return m_part->GetMnemonic();
	}

	feature_code Decorator::GetFeatures(void) const
	{
		return m_part->GetFeatures();
	}

	void Decorator::SetParam(const CString& strName, VARIANT vValue)
	{
		m_part->SetParam(strName, vValue);
	}

	void Decorator::GetParam(const CString& strName, VARIANT* pvValue)
	{
		m_part->GetParam(strName, pvValue);
	}

	void Decorator::SetActive(bool bIsActive)
	{
		m_part->SetActive(bIsActive);
	}

	CSize Decorator::GetPreferredSize(void) const
	{
		return m_part->GetPreferredSize();
	}


	void Decorator::SetLocation(const CRect& location)
	{
		m_part->SetLocation(location);
	}

	CRect Decorator::GetLocation(void) const
	{
		return m_part->GetLocation();
	}

	CRect Decorator::GetLabelLocation(void) const
	{
		return m_part->GetLabelLocation();
	}

	CRect Decorator::GetPortLocation(CComPtr<IMgaFCO>& fco) const
	{
		return m_part->GetPortLocation(fco);
	}

	void Decorator::GetPorts(CComPtr<IMgaFCOs>& portFCOs) const
	{
		m_part->GetPorts(portFCOs);
	}

	void Decorator::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
	{
		m_part->Draw(pDC, gdip);
	}

	void Decorator::SaveState()
	{
		m_part->SaveState();
	}

	// New functions
	void Decorator::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
		HWND parentWnd)
	{
		/*DecoratorCompositePart* decoratorComposite = new DecoratorCompositePart(NULL, m_eventSink);
		m_part = decoratorComposite;

		decoratorComposite->InitializeEx(pProject, pPart, pFCO, parentWnd);*/

		CComPtr<IMgaMetaRole> metaRole;
		CComPtr<IMgaMetaFCO> metaFco;
		COMTHROW(pPart->get_Role(&metaRole));
		VERIFY(metaRole);
		COMTHROW(metaRole->get_Kind(&metaFco));
		VERIFY(metaFco);
		CComBSTR bstr;
		COMTHROW(metaFco->get_Name(&bstr));
		CString type = bstr;
		if (type.Find(CPM_NAMESPACE)==0)
			type = type.Mid(CPM_NAMESPACE_LENGTH);
		Type::cpm_type cpmType = GetType(type);
		DecoratorSDK::PreferenceMap preferences;
		DecoratorSDK::getFacilities().loadPathes(pProject, true);
		

		if (pFCO)
		{
			if ((cpmType >= Type::parameter_type) && (cpmType <=Type::metric_type))
			{
				COLORREF drop_color = RGB(0x00, 0xc0, 0xc0);
				if (cpmType == Type::parameter_type)
				{
					drop_color = PARAM_DROP_COLOR;
				}
				else if (cpmType == Type::property_type)
				{
					drop_color = PROP_DROP_COLOR;
				}
				CPMBasePart* cpmPart = new CPMBasePart(NULL, m_eventSink, drop_color);
				CPMDecoratorStyle style(m_eventSink, pFCO, cpmPart, cpmType, parentWnd, pPart, pProject);
				style.AddInternalParts();
				cpmPart->InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
				m_part = cpmPart;
			}
			else
			{
				ObjectAndTextPart* objectAndText = new ObjectAndTextPart(NULL, m_eventSink);
				ModelComplexPart* modelpart = new ModelComplexPart(NULL, m_eventSink);
				LabelPart* text = new LabelPart(NULL, m_eventSink);
				objectAndText->AddObjectPart(modelpart);
				objectAndText->AddTextPart(text);
				if (cpmType == Type::toplevelsystemundertest_type
					|| cpmType == Type::testinjectionpoint_type)
				{
					preferences[PREF_ICON] = GetIcon(pFCO, pPart, L"SystemUnderTest.bmp");
				}
				else if (cpmType == Type::componentassembly_type
					|| cpmType == Type::alternativerepresentationcontainer_type
					|| cpmType == Type::componentassemblyref_type)
				{
					preferences[PREF_ICON] = GetIcon(pFCO, pPart, L"ComponentAssembly.png");
				}
				else if (cpmType == Type::documentation_type)
				{
					preferences[PREF_ICON] = GetIcon(pFCO, pPart, L"Documentation.jpg");
				}
				else if (cpmType == Type::component_type)
				{
					preferences[PREF_ICON] = GetIcon(pFCO, pPart, L"Component_piece.png");
				}
				else if (cpmType == Type::componentref_type)
				{
					preferences[PREF_ICON] = GetIcon(pFCO, pPart, L"Component_piece.png");
				}
				else if (cpmType == Type::testbenchref_type)
				{
					preferences[PREF_ICON] = GetIcon(pFCO, pPart, L"testbenchref.png");
				}
				else
				{
					preferences[PREF_ICON] = GetIcon(pFCO, pPart, L"Component_piece.png");
				}

				objectAndText->InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
				m_part = objectAndText;

			}
		}



		if (m_part == NULL)
		{
			_bstr_t icon;
			pPart->Role->Kind->GetRegistryValueDisp(_bstr_t(PREF_ICON), icon.GetAddress());

			if (icon.length() == 0)
			{
				icon = L"Component_piece.png";
			}
			preferences[PREF_ICON] = PreferenceVariant(CString(static_cast<const TCHAR*>(icon)));

			m_tiles = std::unique_ptr<TileVector>(new TileVector());
			m_tiles->push_back(BackgroundTile());
			preferences[PREF_TILES] = PreferenceVariant(m_tiles.get());
			preferences[PREF_TILESDEFAULT] = PreferenceVariant(m_tiles.get());
			preferences[PREF_TILESUNDEF] = PreferenceVariant(m_tiles.get());

			ObjectAndTextPart* objectAndText = new ObjectAndTextPart(NULL, m_eventSink);
			PartBase* bitmap;
			LabelPart* text = new LabelPart(NULL, m_eventSink);
			bitmap = new TypeableBitmapPart(NULL, m_eventSink);
			objectAndText->AddObjectPart(bitmap);
			objectAndText->AddTextPart(text);

			objectAndText->InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
			m_part = objectAndText;
		}

	}

	Type::cpm_type Decorator::GetType(CString& type) {
		if (type == CPM_COMPONENT_OBJ) {
			return Type::component_type;
		} else if (type == CPM_TESTCOMPONENT_OBJ) {
			return Type::testcomponent_type;
		} else if (type == CPM_CYBERCOMPONENT_OBJ) {
			return Type::cybercomponent_type;
		} else if (type == "TestInjectionPoint" || type == "CriticalComponent" || type == "BallisticTarget") {
			return Type::testinjectionpoint_type;
		} else if (type == "DesignEntityRef" || type == "BuiltDesignEntityRef") {
			return Type::designentityref_type;
		} else if (type == "TestBenchRef") {
			return Type::testbenchref_type;
		} else if (type == "TopLevelSystemUnderTest") {
			return Type::toplevelsystemundertest_type;
		} else if (type == "ComponentRef") {
			return Type::componentref_type;
		} else if (type == "ComponentAssemblyRef") {
			return Type::componentassemblyref_type;
		} else if (type == "ComponentAssembly") {
			return Type::componentassembly_type;
		} else if (type == "AlternativeRepresentationContainer") {
			return Type::alternativerepresentationcontainer_type;
		} else if (type == CPM_PARAMETER_OBJ) {
			return Type::parameter_type;
		} else if (type == CPM_PROPERTY_OBJ){
			return Type::property_type;
		} else if (type == CPM_METRIC_OBJ){
			return Type::metric_type;
		} else if (type == CPM_THERMAL_PORT_OBJ) {
			return Type::thermal_type;
		} else if (type == CPM_ELECTRICAL_PORT_OBJ) {
			return Type::electrical_type;
		} else if (type == CPM_HYDROLIC_PORT_OBJ) {
			return Type::hydrolic_type;
		}else if (type == CPM_MAGNETIC_PORT_OBJ) {
			return Type::magnetic_type;
		}else if (type == CPM_ELECTROMAGNETIC_PORT_OBJ) {
			return Type::electromagnetic_type;
		}else if (type == CPM_ROTATIONAL_PORT_OBJ) {
			return Type::rotational_type;
		}else if (type == CPM_TRANSLATIONAL_PORT_OBJ) {
			return Type::translational_type;
		}else if (type == CPM_PNEUMATIC_PORT_OBJ) {
			return Type::pneumatic_type;
		}else if (type == CPM_ACOUSTIC_PORT_OBJ) {
			return Type::acoustic_type;
		}else if (type == CPM_ELECTRONICSIGNAL_PORT_OBJ) {
			return Type::electronicsignal_type;
		} else if (type == CPM_SOFTWARE_PORT_OBJ) {
			return Type::software_type;
		} else if (type == "Documentation") {
			return Type::documentation_type;
		}


		return Type::none_type;
	}

	void Decorator::SetSelected(bool bIsSelected)
	{
		m_part->SetSelected(bIsSelected);
	}

	bool Decorator::MouseMoved(UINT nFlags, const CPoint& point, HDC transformHDC)
	{
		return m_part->MouseMoved(nFlags, point, transformHDC);
	}

	bool Decorator::MouseLeftButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
	{
		return m_part->MouseLeftButtonDown(nFlags, point, transformHDC);
	}

	bool Decorator::MouseLeftButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
	{
		return m_part->MouseLeftButtonUp(nFlags, point, transformHDC);
	}

	bool Decorator::MouseLeftButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
	{
		return m_part->MouseLeftButtonDoubleClick(nFlags, point, transformHDC);
	}

	bool Decorator::MouseRightButtonDown(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC)
	{
		return m_part->MouseRightButtonDown(hCtxMenu, nFlags, point, transformHDC);
	}

	bool Decorator::MouseRightButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
	{
		return m_part->MouseRightButtonUp(nFlags, point, transformHDC);
	}

	bool Decorator::MouseRightButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
	{
		return m_part->MouseRightButtonDoubleClick(nFlags, point, transformHDC);
	}

	bool Decorator::MouseMiddleButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
	{
		return m_part->MouseMiddleButtonDown(nFlags, point, transformHDC);
	}

	bool Decorator::MouseMiddleButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
	{
		return m_part->MouseMiddleButtonUp(nFlags, point, transformHDC);
	}

	bool Decorator::MouseMiddleButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
	{
		return m_part->MouseMiddleButtonDoubleClick(nFlags, point, transformHDC);
	}

	bool Decorator::MouseWheelTurned(UINT nFlags, short distance, const CPoint& point, HDC transformHDC)
	{
		return m_part->MouseWheelTurned(nFlags, distance, point, transformHDC);
	}

	bool Decorator::DragEnter(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC)
	{
		return m_part->DragEnter(dropEffect, pDataObject, dwKeyState, point, transformHDC);
	}

	bool Decorator::DragOver(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC)
	{
		return m_part->DragOver(dropEffect, pDataObject, dwKeyState, point, transformHDC);
	}

	bool Decorator::Drop(COleDataObject* pDataObject, DROPEFFECT dropEffect, const CPoint& point, HDC transformHDC)
	{
		return m_part->Drop(pDataObject, dropEffect, point, transformHDC);
	}

	bool Decorator::DropFile(HDROP p_hDropInfo, const CPoint& point, HDC transformHDC)
	{
		return m_part->DropFile(p_hDropInfo, point, transformHDC);
	}

	bool Decorator::MenuItemSelected(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC)
	{
		return m_part->MenuItemSelected(menuItemId, nFlags, point, transformHDC);
	}

	bool Decorator::OperationCanceledByGME(void)
	{
		return m_part->OperationCanceledByGME();
	}



}; // namespace Decor
