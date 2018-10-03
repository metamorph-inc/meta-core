//################################################################################################
//
// Model complex part class (decorator part)
//	ModelComplexPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "ModelComplexPart.h"
#include "DecoratorExceptions.h"
#include "Resource.h"
#include "TextPart.h"
#include "PortLabelPart.h"
#include <algorithm>
#include <new>
#include <io.h>

#define MAXIMUM_ICON_SIZE_NONOVERLAPPING_WITH_PORT_LABELS 115
static const int cornerRadius = 15;

namespace Decor{


bool operator==(const _bstr_t& a, const wchar_t* b) {
	if (a.length() == 0) {
		return wcslen(b) == 0;
	}
	return wcscmp(a, b) == 0;
}

struct {
	const wchar_t* kind;
	const wchar_t* fileAttribute;
} PETKinds[] = {
	{ L"AnalysisBlock", L"PyFilename" },
	{ L"ExcelWrapper", L"ExcelFilename" },
	{ L"MATLABWrapper", L"MFilename" },
	{ L"PythonWrapper", L"PyFilename" },
};
const wchar_t* PETWrapperLookup(_bstr_t& kind) {
	for (size_t i = 0; i < _countof(PETKinds); i++) {
		if (kind == PETKinds[i].kind) {
			return PETKinds[i].fileAttribute;
		}
	}
	return NULL;
}

void invokeCyPhyPETMethod(IMgaFCO* fco, const wchar_t* methodName) {
	CComDispatchDriver dd;
	if (SUCCEEDED(dd.CoCreateInstance(L"MGA.Interpreter.CyPhyPET", nullptr, CLSCTX_INPROC))) {
		IMgaProjectPtr proj = fco->Project;
		CComPtr<IMgaTerritory> terr;
		proj->BeginTransactionInNewTerr(TRANSACTION_NON_NESTED, &terr);
		CComVariant variantFCO = (IDispatch*)fco;
		dd.Invoke1(methodName, &variantFCO);
		proj->CommitTransaction();
	}
};

void PETRefreshButtonClicked(IMgaFCO* fco) {
	invokeCyPhyPETMethod(fco, L"RefreshButtonClicked");
}
void PETEditButtonClicked(IMgaFCO* fco) {
	invokeCyPhyPETMethod(fco, L"EditButtonClicked");
}

class CPMPortLabelPart : public PortLabelPart
{
public:
	CString GetText() const {
		return m_vecText[0];
	}
};

//################################################################################################
//
// CLASS : PortPartData
//
//################################################################################################
class PortPartData {
public:
	PortPartData(PortPart* pD, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO):
		portPart(pD), spPart(pPart), spFCO(pFCO) {};
	~PortPartData() { if(portPart) delete portPart; };

	void Destroy() { if(portPart) portPart->Destroy(); }

	PortPart*				portPart;
	CComPtr<IMgaMetaPart>	spPart;
	CComPtr<IMgaFCO>		spFCO;
};

//################################################################################################
//
// CLASS : ModelComplexPart
//
//################################################################################################

ModelComplexPart::ModelComplexPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	TypeableBitmapPart			(pPart, eventSink),

	m_iMaxPortTextLength		(MAX_PORT_LENGTH),
	m_crPortText				(COLOR_BLACK),
	m_bPortLabelInside			(true),
	m_iLongestPortTextLength	(0),
	m_bStretch(false)
{
}

ModelComplexPart::~ModelComplexPart()
{
	m_LeftPorts.clear();
	m_RightPorts.clear();
	for (std::vector<PortPartData*>::iterator ii = m_AllPorts.begin(); ii != m_AllPorts.end(); ++ii)
		delete (*ii);
	m_AllPorts.clear();
}

void ModelComplexPart::Initialize(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO)
{
// Akos: when this gets called, the left and right ports list should not exist
	/*
	for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
		(*ii)->Initialize(pProject, pPart, pFCO);
	}
	for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
		(*ii)->Initialize(pProject, pPart, pFCO);
	}*/
	TypeableBitmapPart::Initialize(pProject, pPart, pFCO);


	embelishment = Embelishment::NONE;
	if (pFCO)
	{
		IMgaReferencePtr ref = pFCO.p;
		std::wstring kinds[] = { // keep these alphabetical
			L"BuiltDesignEntityRef",
			L"ComponentAssemblyRef",
			L"ComponentRef",
			L"DesignEntityRef", // TODO: set Decorator=Mga.CPMDecorator in the meta
			L"TestBenchRef",
			L"TestInjectionPoint",
			L"TopLevelSystemUnderTest",
		};
		if (ref && std::binary_search(kinds, kinds + (sizeof(kinds)/sizeof(kinds[0])), std::wstring(ref->Meta->Name),
			[](const std::wstring& s1, const std::wstring& s2) { return wcscmp(s1.c_str(), s2.c_str()) < 0; }))
		{
			if (ref->Referred)
				embelishment = Embelishment::REFERENCE;
			else
				embelishment = Embelishment::NULL_REFERENCE;
		}
	}
}

void ModelComplexPart::Destroy()
{
	for (std::vector<PortPartData*>::iterator ii = m_AllPorts.begin(); ii != m_AllPorts.end(); ++ii)
		(*ii)->Destroy();
	TypeableBitmapPart::Destroy();
}

CString ModelComplexPart::GetMnemonic(void) const
{
	return TypeableBitmapPart::GetMnemonic();
}

feature_code ModelComplexPart::GetFeatures(void) const
{
	feature_code featureCodes = 0;

	for (std::vector<Decor::PortPart*>::const_iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
		feature_code portFeatureCodes = (*ii)->GetFeatures();
		featureCodes |= portFeatureCodes;
	}
	for (std::vector<Decor::PortPart*>::const_iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
		feature_code portFeatureCodes = (*ii)->GetFeatures();
		featureCodes |= portFeatureCodes;
	}

	feature_code partFeatureCodes = TypeableBitmapPart::GetFeatures();
	featureCodes |= partFeatureCodes;

	return featureCodes;
}

void ModelComplexPart::SetParam(const CString& strName, VARIANT vValue)
{
	CString pName(DEC_CONNECTED_PORTS_ONLY_PARAM);
	if(pName == strName && vValue.boolVal == VARIANT_TRUE)
		ReOrderConnectedOnlyPorts();
	else
	{
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			(*ii)->SetParam(strName, vValue);
		}
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			(*ii)->SetParam(strName, vValue);
		}
	TypeableBitmapPart::SetParam(strName, vValue);
	}
}

bool ModelComplexPart::GetParam(const CString& strName, VARIANT* pvValue)
{
	try {
		if (TypeableBitmapPart::GetParam(strName, pvValue))
			return true;
	}
	catch(UnknownParameterException&) {
	}
	return false;
}

void ModelComplexPart::SetActive(bool bIsActive)
{
	for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
		(*ii)->SetActive(bIsActive);
	}
	for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
		(*ii)->SetActive(bIsActive);
	}
	TypeableBitmapPart::SetActive(bIsActive);
}

template<typename IT, typename F>
int transform_max(IT begin, IT end, F func)
{
	int max_ret = INT_MIN;
	while (begin != end)
	{
		max_ret = max(max_ret, func(*begin));
		++begin;
	}
	return max_ret;
}

CSize ModelComplexPart::GetPreferredSize(void) const
{
	const auto& button = this->button;
	const auto& button2 = this->button2;
	int RightPortsMaxLabelLength = 0;
	auto setButtonPosition = [&RightPortsMaxLabelLength, &button, &button2](const CSize& size) {
		if (button && button->m_bmp) {
			// bottom right, left of port names
			int y = size.cy - button->m_bmp->GetHeight();
			int x = size.cx - RightPortsMaxLabelLength - button->m_bmp->GetWidth();
			button->position = CRect(x, y, x + button->m_bmp->GetWidth(), y + button->m_bmp->GetHeight());
			if (button2 && button2->m_bmp) {
				// left of button
				y -= button->m_bmp->GetHeight() - button2->m_bmp->GetHeight();
				x -= button2->m_bmp->GetWidth();
				button2->position = CRect(x, y, x + button2->m_bmp->GetWidth(), y + button2->m_bmp->GetHeight());
			}
		}
	};

	CSize size = ResizablePart::GetPreferredSize();
	bool hasStoredCustomSize = (size.cx * size.cy != 0);

	if (!hasStoredCustomSize && m_LeftPorts.empty() && m_RightPorts.empty()) {
		if (!m_pBitmap) {
			CSize ret(WIDTH_MODEL, HEIGHT_MODEL);
			setButtonPosition(ret);
			return ret;
		} else {
			CSize bitmapSize = TypeableBitmapPart::GetPreferredSize();
			setButtonPosition(bitmapSize);
			return bitmapSize;
		}
	}

	int LeftPortsMaxLabelLength = 0;
	LOGFONT logFont;
	getFacilities().GetFont(FONT_PORT)->gdipFont->GetLogFontW(getFacilities().getGraphics(), &logFont);
	{
		HDC dc = GetDC(NULL);
		Gdiplus::Graphics g(dc);
		Gdiplus::Font f(dc, &logFont);
		Gdiplus::PointF zero(0.0, 0.0);
		Gdiplus::RectF box;

		auto measure_f = [&](const Decor::PortPart* portPart) -> int {
			CStringW porttext = static_cast<CPMPortLabelPart* /* n.b. this lie doesn't matter */>(portPart->GetTextPart())->GetText();
			g.MeasureString(static_cast<const wchar_t*>(porttext), min(porttext.GetLength(), m_iMaxPortTextLength == 0 ? INT_MAX : m_iMaxPortTextLength), &f, zero, &box);
			return (int)(box.Width + 0.5f);
		};
		int base_port_gap = max(cornerRadius, GAP_LABEL + WIDTH_PORT + GAP_XMODELPORT);
		LeftPortsMaxLabelLength = max(-base_port_gap, transform_max(this->m_LeftPorts.begin(), m_LeftPorts.end(), measure_f)) + base_port_gap;
		RightPortsMaxLabelLength = max(-base_port_gap, transform_max(this->m_RightPorts.begin(), m_RightPorts.end(), measure_f)) + base_port_gap;
		LeftPortsMaxLabelLength = max(cornerRadius, LeftPortsMaxLabelLength);
		RightPortsMaxLabelLength = max(cornerRadius, RightPortsMaxLabelLength);
		const_cast<ModelComplexPart*>(this)->m_LeftPortsMaxLabelLength = LeftPortsMaxLabelLength;
		const_cast<ModelComplexPart*>(this)->m_RightPortsMaxLabelLength = RightPortsMaxLabelLength;
		::ReleaseDC(NULL, dc);
	}

	long lWidth = 0;
	if (m_bPortLabelInside) {
		ASSERT(m_iLongestPortTextLength >= 0 && m_iLongestPortTextLength <= 1000);
		ASSERT(m_iMaxPortTextLength >= 0 && m_iMaxPortTextLength <= 1000);
		lWidth = LeftPortsMaxLabelLength + RightPortsMaxLabelLength + min(TypeableBitmapPart::GetPreferredSize().cx, MAXIMUM_ICON_SIZE_NONOVERLAPPING_WITH_PORT_LABELS);
	} else {
		lWidth = (8 * 3 + GAP_LABEL + WIDTH_PORT + GAP_XMODELPORT) * 2 + GAP_PORTLABEL;
	}

	long lHeight = GAP_YMODELPORT * 2 +
					max(m_LeftPorts.size(), m_RightPorts.size()) * (HEIGHT_PORT + GAP_PORT) - GAP_PORT;

	if (hasStoredCustomSize) {
		CSize calcSize = CSize(min(size.cx, lWidth), min(size.cy, lHeight));
		const_cast<Decor::ModelComplexPart*>(this)->resizeLogic.SetMinimumSize(calcSize);
		setButtonPosition(size);
		return size;
	}
	else {
	CSize bitmapSize = TypeableBitmapPart::GetPreferredSize();
	lWidth = max(lWidth, bitmapSize.cx);
	lHeight = max(lHeight, bitmapSize.cy);
	const_cast<Decor::ModelComplexPart*>(this)->resizeLogic.SetMinimumSize(CSize(lWidth, lHeight));

		CSize size(max((long)WIDTH_MODEL, lWidth), max((long)HEIGHT_MODEL, lHeight));
		setButtonPosition(size);
		return size;
	}
}

void ModelComplexPart::SetLocation(const CRect& location)
{
	for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
		(*ii)->SetLocation(location);
	}
	for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
		(*ii)->SetLocation(location);
	}
	TypeableBitmapPart::SetLocation(location);
}

CRect ModelComplexPart::GetLocation(void) const
{
	return TypeableBitmapPart::GetLocation();
}

CRect ModelComplexPart::GetLabelLocation(void) const
{
	CRect labelLocation(0,0,0,0);
	try {
		labelLocation = TypeableBitmapPart::GetLabelLocation();
	}
	catch(NotImplementedException&) {
		for (std::vector<Decor::PortPart*>::const_iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			labelLocation = (*ii)->GetLabelLocation();
			if (!labelLocation.IsRectEmpty())
				break;
		}
		if (labelLocation.IsRectEmpty()) {
			for (std::vector<Decor::PortPart*>::const_iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
				labelLocation = (*ii)->GetLabelLocation();
				if (!labelLocation.IsRectEmpty())
					break;
			}
		}
	}
	if (labelLocation.IsRectEmpty())
		throw hresult_exception(E_DECORATOR_NOT_IMPLEMENTED);
	return labelLocation;
}

CRect ModelComplexPart::GetPortLocation(CComPtr<IMgaFCO>& pFCO) const
{
	CRect portLocation(0,0,0,0);
	Decor::PortPart* pPortPart = GetPort(pFCO);
	if (pPortPart != NULL) {
		portLocation = pPortPart->GetLocation();
		CPoint offset = GetLocation().TopLeft();
		portLocation.OffsetRect(-offset.x, -offset.y);
	}
	portLocation.InflateRect(1, 1, 1, 1);
	return portLocation;
}

bool ModelComplexPart::GetPorts(CComPtr<IMgaFCOs>& portFCOs) const
{
	CComPtr<IMgaFCOs> spFCOs;
	COMTHROW(spFCOs.CoCreateInstance(OLESTR("Mga.MgaFCOs")));

	for (std::vector<Decor::PortPart*>::const_iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
		COMTHROW(spFCOs->Append((*ii)->GetFCO()));
	}
	for (std::vector<Decor::PortPart*>::const_iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
		COMTHROW(spFCOs->Append((*ii)->GetFCO()));
	}

	portFCOs = spFCOs.Detach();
	return true;
}

void ModelComplexPart::DrawMarker(Gdiplus::Graphics& g, const wchar_t* s, CPoint p, Gdiplus::Color& bgColor)
{
	using namespace Gdiplus;
	//g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
	FontFamily family(L"Palatino Linotype");
	Gdiplus::Font f(&family, 11, FontStyleBold, Gdiplus::UnitPixel);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);
	RectF area((REAL)p.x - 13 / 2, (REAL)p.y - 13 / 2, (REAL)13, (REAL)13);
	RectF shadow = area;
	shadow.X -= 1;
	shadow.Y += 1;
	shadow.Width += 1;
	shadow.Height += 1;
	SolidBrush b(Color(128, 0, 0, 0));
	g.FillEllipse(&b, shadow);
	SolidBrush b2(bgColor);
	g.FillEllipse(&b2, area);
	Pen darkBlue(Color::DarkBlue);
	g.DrawEllipse(&darkBlue, area);
	SolidBrush blackBrush(Color::Black);
	area.X += 0.5;
	area.Y += 0.5;
	g.DrawString(s, 1, &f, area, &format, &blackBrush);
}

void ModelComplexPart::DrawEmbellishments(Gdiplus::Graphics& g)
{
	if (embelishment != Embelishment::NONE)
	{
		CPoint loc(this->m_Rect.right - 1, this->m_Rect.top - 1);
		if (embelishment == Embelishment::REFERENCE)
		{
			Gdiplus::Color bgColor(m_bActive ? Gdiplus::Color::LightBlue : Gdiplus::Color::Gray);
			DrawMarker(g, L"R", loc, bgColor);
		}
		else if (embelishment == Embelishment::NULL_REFERENCE)
		{
			Gdiplus::Color bgColor(m_bActive ? Gdiplus::Color::Red : Gdiplus::Color::Gray);
			DrawMarker(g, L"R", loc, bgColor);
		}
	}
}


void ModelComplexPart::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
{
	TypeableBitmapPart::Draw(pDC, gdip);

	DrawEmbellishments(*gdip);
}

void ModelComplexPart::SaveState()
{
	for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
		(*ii)->SaveState();
	}
	for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
		(*ii)->SaveState();
	}
	TypeableBitmapPart::SaveState();
}

// New functions
void ModelComplexPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
									HWND parentWnd, PreferenceMap& preferences)
{
	preferences[PREF_ICONDEFAULT]	= PreferenceVariant(createResString(IDB_MODEL));
	preferences[PREF_TILESDEFAULT]	= PreferenceVariant(getFacilities().getTileVector(TILE_MODELDEFAULT));
	preferences[PREF_TILESUNDEF]	= PreferenceVariant(getFacilities().getTileVector(TILE_PORTDEFAULT));

	if (pPart) {
		kind = pPart->Role->Kind->Name;
	}
	if (pFCO) {
		PreferenceMap::iterator it = preferences.find(PREF_PORTLABELCOLOR);
		if (it != preferences.end())
			m_crPortText = it->second.uValue.crValue;
		else
			getFacilities().getPreference(pFCO, PREF_PORTLABELCOLOR, m_crPortText);

		it = preferences.find(PREF_PORTLABELINSIDE);
		if (it != preferences.end())
			m_bPortLabelInside = it->second.uValue.bValue;
		else
			getFacilities().getPreference(pFCO, PREF_PORTLABELINSIDE, m_bPortLabelInside);

		long o = m_iMaxPortTextLength;
		it = preferences.find(PREF_PORTLABELLENGTH);
		if (it != preferences.end()) {
			m_iMaxPortTextLength = it->second.uValue.lValue;
		} else {
			if (getFacilities().getPreference(pFCO, PREF_PORTLABELLENGTH, m_iMaxPortTextLength))
				m_iMaxPortTextLength = abs(m_iMaxPortTextLength);	//convert any negative value to positive
			else	//if not found in registry
				m_iMaxPortTextLength = MAX_PORT_LENGTH;	// the default value in Preferences
		}
		if (m_iMaxPortTextLength == 0)	// if 0 it means it has to show all way long
			m_iMaxPortTextLength = 999;	// so we set a huge value

		TypeableBitmapPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);

		LoadPorts();

		if (PETWrapperLookup(kind)) {
			button = std::unique_ptr<ModelButton>(new ModelButton());
			button->callback = PETRefreshButtonClicked;
			if (wcscmp(kind, L"ExcelWrapper") != 0)
			{
				button2 = std::unique_ptr<ModelButton>(new ModelButton());
				button2->callback = PETEditButtonClicked;
			}

			if (getFacilities().arePathesValid())
			{
				std::vector<CString> vecPathes = getFacilities().getPathes();

				CString filenames[] = { L"refresh.png", L"open_button.png" };
				std::unique_ptr<ModelButton>* buttons[] = { &button, &button2 };
				for (int i = 0; i < _countof(buttons) && *buttons[i]; i++)
				{
					CString& strFName = filenames[i];
					auto& m_bmp = (**buttons[i]).m_bmp;

					for (unsigned int i = 0; i < vecPathes.size(); i++)
					{
						CString imageFileName = vecPathes[i] + strFName;
						m_bmp = std::unique_ptr<Gdiplus::Bitmap>(Gdiplus::Bitmap::FromFile(CStringW(imageFileName)));
						if (m_bmp && m_bmp->GetLastStatus() == Gdiplus::Ok)
						{
							UINT widthToSet = m_bmp->GetWidth();
							UINT heightToSet = m_bmp->GetHeight();
							ASSERT(widthToSet > 0);	// valid sizes, otherwise AutoRouter fails
							ASSERT(heightToSet > 0);
							break;
						}
						else
						{
							m_bmp = nullptr;
						}
					}
				}
			}
		}
	} else {
		TypeableBitmapPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
	}

	if (m_LeftPorts.empty() && m_RightPorts.empty())
		m_pTileVector = getFacilities().getTileVector(TILE_ATOMDEFAULT);

	PreferenceMap::iterator iconint = preferences.find("iconint");
	if (iconint != preferences.end())
	{
		m_bmp = std::unique_ptr<Gdiplus::Bitmap>(new Gdiplus::Bitmap(((HINSTANCE)&__ImageBase), (WCHAR*) MAKEINTRESOURCE(iconint->second.uValue.lValue)));
	}
	else if (preferences.find(PREF_ICON) != preferences.end())
	{
		if ( ! getFacilities().arePathesValid() )
			return;
		std::vector<CString> vecPathes = getFacilities().getPathes();

		bool success = false;
		CString& strFName = *preferences.find(PREF_ICON)->second.uValue.pstrValue;

		for (unsigned int i = 0; i < vecPathes.size() ; i++ )
		{
			CString imageFileName = vecPathes[ i ] + strFName;
			m_bmp = std::unique_ptr<Gdiplus::Bitmap>(Gdiplus::Bitmap::FromFile(CStringW(imageFileName)));
			if( m_bmp && m_bmp->GetLastStatus() == Gdiplus::Ok)
			{
				UINT widthToSet = m_bmp->GetWidth();
				UINT heightToSet = m_bmp->GetHeight();
				ASSERT( widthToSet > 0);	// valid sizes, otherwise AutoRouter fails
				ASSERT( heightToSet > 0);
				break;
			}
			else
			{
				m_bmp = nullptr;
			}
		}
	}

	if (preferences.find(brushColorVariableName) == preferences.end() &&
		getFacilities().getPreference(m_spFCO, m_spMetaFCO, brushColorVariableName, m_crBrush) == false)
	{
		m_crBrush = RGB(0xdd, 0xdd, 0xdd);
	}
}

void ModelComplexPart::SetSelected(bool bIsSelected)
{
	if (this->m_spFCO)
	{
		auto addOns = this->m_spFCO->Project->AddOnComponents;
		for (int i = 1; i <= addOns->Count; i++)
		{
			if (wcscmp(addOns->GetItem(i)->ComponentName, L"CyPhyMetaLinkAddon") == 0)
			{
				IDispatchPtr disp = addOns->GetItem(i);
				if (disp)
				{
					CComDispatchDriver d(disp);
					CComVariant varFCO(this->m_spFCO);
					CComVariant varSelected(bIsSelected);
					d.Invoke2(L"SetSelected", &varFCO, &varSelected);
				}
			}
		}
	}

	for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
		(*ii)->SetSelected(bIsSelected);
	}
	for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
		(*ii)->SetSelected(bIsSelected);
	}
	TypeableBitmapPart::SetSelected(bIsSelected);
}

bool ModelComplexPart::MouseMoved(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseMoved(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseMoved(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseMoved(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool ModelComplexPart::MouseLeftButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	std::unique_ptr<ModelButton>* buttons[] = { &button, &button2, nullptr };
	for (std::unique_ptr<ModelButton>** it = &buttons[0]; *it; it++) {
		std::unique_ptr<ModelButton>& button = **it;
		if (!button || !button->m_bmp) {
			continue;
		}
		//HWND wnd = WindowFromDC(transformHDC);
		CPoint converted = point;
		//ClientToScreen(wnd, &converted);
		CRect loc = TypeableBitmapPart::GetBoxLocation(false);
		CRect buttonPosition = button->position;
		buttonPosition.OffsetRect(loc.TopLeft());

		if (buttonPosition.PtInRect(converted)) {
			button->callback(m_spFCO);
			return true;
		}
	}
	try {
		if (TypeableBitmapPart::MouseLeftButtonDown(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseLeftButtonDown(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseLeftButtonDown(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool ModelComplexPart::MouseLeftButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseLeftButtonUp(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseLeftButtonUp(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseLeftButtonUp(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool ModelComplexPart::MouseLeftButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	CComPtr<IMgaProject> proj;
	if (m_spFCO && m_spMetaFCO && SUCCEEDED(m_spFCO->get_Project(&proj)))
	{
		CComPtr<IMgaTerritory> terr;
		if (kind.length() && kind == L"Data Sheet")
		{
			proj->BeginTransactionInNewTerr(TRANSACTION_NON_NESTED, &terr);
			CComBSTR path;
			if (SUCCEEDED(m_spFCO->get_StrAttrByName(CComBSTR(L"Path"), &path)) && path)
			{
				std::wstring docpath;
				CComBSTR connStr;
				HRESULT hr = S_OK;
				if (SUCCEEDED(hr))
					hr = proj->get_ProjectConnStr(&connStr);
				wchar_t fullpath[MAX_PATH];
				wchar_t* filepart;
				if (SUCCEEDED(hr))
				if (wcsncmp(connStr, L"MGA=", 4) == 0)
				if (GetFullPathNameW(static_cast<const wchar_t*>(connStr) + 4, sizeof(fullpath)/sizeof(fullpath[0]), fullpath, &filepart) != 0 && filepart)
				{
					*filepart = L'\0';
					docpath = fullpath;
					docpath += path;

					ShellExecuteW(0, L"open", docpath.c_str(), NULL, NULL, SW_SHOW);

					proj->CommitTransaction();
					return true;
				}
			}
			// n.b. don't abort, it will Reset the view and destruct this
			proj->CommitTransaction();
		}

		if (kind.length() && PETWrapperLookup(kind))
		{
			proj->BeginTransactionInNewTerr(TRANSACTION_NON_NESTED, &terr);
			if (m_spFCO->GetStrAttrByName(PETWrapperLookup(kind)).length() > 0) {
				; // use default action (open model)
			}
			else {
				CComDispatchDriver dd;
				CComVariant variantFCO = (IDispatch*)m_spFCO.p;
				if (SUCCEEDED(dd.CoCreateInstance(L"MGA.Interpreter.CyPhyPET", nullptr, CLSCTX_INPROC))) {
					dd.Invoke1(L"RefreshButtonClicked", &variantFCO);
					proj->CommitTransaction();
					return true;
				}
			}
			// n.b. don't abort, it will Reset the view and destruct this
			proj->CommitTransaction();
		}
	}

	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseLeftButtonDoubleClick(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseLeftButtonDoubleClick(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseLeftButtonDoubleClick(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool ModelComplexPart::MouseRightButtonDown(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseRightButtonDown(hCtxMenu, nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseRightButtonDown(hCtxMenu, nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseRightButtonDown(hCtxMenu, nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool ModelComplexPart::MouseRightButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseRightButtonUp(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseRightButtonUp(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseRightButtonUp(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool ModelComplexPart::MouseRightButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseRightButtonDoubleClick(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseRightButtonDoubleClick(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseRightButtonDoubleClick(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool ModelComplexPart::MouseMiddleButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseMiddleButtonDown(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseMiddleButtonDown(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseMiddleButtonDown(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool ModelComplexPart::MouseMiddleButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseMiddleButtonUp(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseMiddleButtonUp(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseMiddleButtonUp(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool ModelComplexPart::MouseMiddleButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseMiddleButtonDoubleClick(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseMiddleButtonDoubleClick(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseMiddleButtonDoubleClick(nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool ModelComplexPart::MouseWheelTurned(UINT nFlags, short distance, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseWheelTurned(nFlags, distance, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseWheelTurned(nFlags, distance, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->MouseWheelTurned(nFlags, distance, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool ModelComplexPart::DragEnter(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::DragEnter(dropEffect, pDataObject, dwKeyState, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->DragEnter(dropEffect, pDataObject, dwKeyState, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->DragEnter(dropEffect, pDataObject, dwKeyState, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool ModelComplexPart::DragOver(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::DragOver(dropEffect, pDataObject, dwKeyState, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->DragOver(dropEffect, pDataObject, dwKeyState, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->DragOver(dropEffect, pDataObject, dwKeyState, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool ModelComplexPart::Drop(COleDataObject* pDataObject, DROPEFFECT dropEffect, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::Drop(pDataObject, dropEffect, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->Drop(pDataObject, dropEffect, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->Drop(pDataObject, dropEffect, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool ModelComplexPart::DropFile(HDROP p_hDropInfo, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::DropFile(p_hDropInfo, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->DropFile(p_hDropInfo, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->DropFile(p_hDropInfo, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool ModelComplexPart::MenuItemSelected(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MenuItemSelected(menuItemId, nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->MenuItemSelected(menuItemId, nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->MenuItemSelected(menuItemId, nFlags, point, transformHDC))
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool ModelComplexPart::OperationCanceledByGME(void)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::OperationCanceledByGME())
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
			try {
				if ((*ii)->OperationCanceledByGME())
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
			try {
				if ((*ii)->OperationCanceledByGME())
					return true;
			}
			catch(hresult_exception& e) {
				retVal = e.hr;
			}
			catch(DecoratorException& e) {
				retVal = e.GetHResult();
			}
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

void ModelComplexPart::DrawBackground(CDC* pDC, Gdiplus::Graphics* gdip)
{
	CSize cExtentD = pDC->GetViewportExt();
	CSize cExtentL = pDC->GetWindowExt();
	if (m_bStretch && m_bmp.get())
	{
		CRect cRect = TypeableBitmapPart::GetBoxLocation(false);
		Gdiplus::Rect grect(cRect.left, cRect.top, cRect.Width(), cRect.Height());
		gdip->DrawImage(m_bmp.get(), grect, 0, 0, m_bmp->GetWidth(), m_bmp->GetHeight(), Gdiplus::UnitPixel);
	}
	else
	{
		CRect cRect = TypeableBitmapPart::GetBoxLocation(false);
		cRect.BottomRight() -= CPoint(1, 1);

		CRect location = cRect;
		if (m_LeftPorts.size() != 0 || m_RightPorts.size() != 0 || m_bmp.get() == nullptr)
		{

			Gdiplus::Rect rect(cRect.left, cRect.top, cRect.Width(), cRect.Height());
			Gdiplus::LinearGradientBrush linearGradientBrush(rect,
				Gdiplus::Color(GetRValue(m_crBrush), GetGValue(m_crBrush), GetBValue(m_crBrush)),
				Gdiplus::Color(GetRValue(m_crGradient), GetGValue(m_crGradient), GetBValue(m_crGradient)),
				Gdiplus::LinearGradientModeVertical);

			Gdiplus::SolidBrush solidBrush(Gdiplus::Color(GetRValue(m_crBrush), GetGValue(m_crBrush), GetBValue(m_crBrush)));

			Gdiplus::Brush& brush = m_bGradientFill ? (Gdiplus::Brush&)linearGradientBrush : (Gdiplus::Brush&)solidBrush;

			Gdiplus::GraphicsPath path;
			path.AddArc(location.left, location.top, cornerRadius, cornerRadius, 180, 90);
			path.AddArc(location.right - cornerRadius, location.top, cornerRadius, cornerRadius, 270, 90);
			path.AddArc(location.right - cornerRadius, location.bottom - cornerRadius, cornerRadius, cornerRadius, 0, 90);
			path.AddArc(location.left, location.bottom - cornerRadius, cornerRadius, cornerRadius, 90, 90);
			gdip->FillPath(&brush, &path);
		}

		if (m_bmp.get())
		{
			int height = m_bmp->GetHeight();
			int width = m_bmp->GetWidth();

			int x; // Render according to m_LeftPortsMaxLabelLength if the image fits inside the rounded rectangle
			// if not, just center it
			int gutter = min(m_LeftPortsMaxLabelLength + m_RightPortsMaxLabelLength, cRect.Width() - width);
			int x_margin = (int)(gutter * (double)m_LeftPortsMaxLabelLength / (m_LeftPortsMaxLabelLength + m_RightPortsMaxLabelLength));
			int left_corner = cornerRadius;
			int right_corner = cRect.Width() - cornerRadius;
			if (x_margin < left_corner - 1 /*slop*/ || x_margin > right_corner - width + 1 /*slop*/)
			{
				// center
				x = cRect.Width() / 2 - (int)m_bmp->GetWidth() / 2;
			}
			else
			{
				x = x_margin;
			}

			Gdiplus::Rect grect((int)cRect.left + x, (int)cRect.top + cRect.Height() / 2 - (int)m_bmp->GetHeight() / 2,
 				width, height);
 			gdip->DrawImage(m_bmp.get(), grect, 0, 0, (int)m_bmp->GetWidth(), (int)m_bmp->GetHeight(), Gdiplus::UnitPixel);
		}
	}

	for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
		(*ii)->Draw(pDC, gdip);
	}
	for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
		(*ii)->Draw(pDC, gdip);
	}
	{
		CRect cRect = TypeableBitmapPart::GetBoxLocation(false);
		cRect.BottomRight() -= CPoint(1, 1);
		std::unique_ptr<ModelButton>* buttons[] = { &button, &button2, nullptr };
		for (std::unique_ptr<ModelButton>** it = &buttons[0]; *it; it++) {
			std::unique_ptr<ModelButton>& button = **it;
			if (button && button->m_bmp) {
				Gdiplus::Rect grect(cRect.left + button->position.left, cRect.top + button->position.top, button->position.Width(), button->position.Height());
				gdip->DrawImage(button->m_bmp.get(), grect, 0, 0, (int)button->m_bmp->GetWidth(), (int)button->m_bmp->GetHeight(), Gdiplus::UnitPixel);
			}
		}
	}
}

void ModelComplexPart::LoadPorts()
{
	if (m_spPart == NULL)
		return;

	CComBSTR regName("showPorts");
	CComBSTR dispPortTxt;
	CComPtr<IMgaFCO> obj = m_parentPart->GetFCO();
	obj->get_RegistryValue(regName, &dispPortTxt);
	if(dispPortTxt.Length())
	{
		if(dispPortTxt == "false")
			return;						// do not need ports for this reference: see Meta paradigm ReferTo connection showPorts attribute
	}

	CComPtr<IMgaMetaAspect>	spParentAspect;
	COMTHROW(m_spPart->get_ParentAspect(&spParentAspect));
	CComBSTR bstrParentAspect;
	COMTHROW(spParentAspect->get_Name(&bstrParentAspect));

	CComQIPtr<IMgaModel> spModel = m_spFCO;
	CComQIPtr<IMgaReference> spRef = m_spFCO;
	while (spModel == NULL && spRef != NULL)
	{
		CComPtr<IMgaFCO> referred;
		spRef->get_Referred(&referred);
		spRef = referred;
		spModel = referred;
	}
	if (spModel == NULL)
		return;
	CComPtr<IMgaMetaFCO> spMetaFCO;
	COMTHROW(spModel->get_Meta(&spMetaFCO));
	CComQIPtr<IMgaMetaModel> spMetaModel = spMetaFCO;

	CComBSTR bstrAspect;
	COMTHROW(m_spPart->get_KindAspect(&bstrAspect));
	if (bstrAspect.Length() == 0) {
		bstrAspect.Empty();
		COMTHROW( spParentAspect->get_Name(&bstrAspect));
	}

	CComPtr<IMgaMetaAspect> spAspect;
	HRESULT hr = spMetaModel->get_AspectByName(bstrAspect, &spAspect);

	if (hr == E_NOTFOUND) {
	//	hr = spMetaModel->get_AspectByName(CComBSTR(L"All"), &spAspect);
	}
	if (hr == E_NOTFOUND) {
		try {
			// PETER: If the proper aspect cannot be found, use the first one
			spAspect = NULL;
			CComPtr<IMgaMetaAspects> spAspects;
			COMTHROW(spMetaModel->get_Aspects(&spAspects));
			ASSERT(spAspects);
			long nAspects = 0;
			COMTHROW(spAspects->get_Count(&nAspects));
			if (nAspects > 0) {
				COMTHROW(spAspects->get_Item(1, &spAspect));
			}
		}
		catch(hresult_exception&) {
		}
	}

	if (spAspect) {
		CComPtr<IMgaFCOs> spFCOs;
		COMTHROW(spModel->get_ChildFCOs(&spFCOs));
		MGACOLL_ITERATE(IMgaFCO, spFCOs) {
			CComPtr<IMgaPart> spPart;
			if (MGACOLL_ITER->get_Part(spAspect, &spPart) == S_OK) {
				if (spPart) {
					CComPtr<IMgaFCO> mgaFco = MGACOLL_ITER;
					CComPtr<IMgaMetaPart> spMetaPart;
					COMTHROW(spPart->get_Meta(&spMetaPart));
					VARIANT_BOOL vbLinked;
					COMTHROW(spMetaPart->get_IsLinked(&vbLinked));

					CComPtr<IMgaMetaFCO> child_meta;
					COMTHROW(MGACOLL_ITER->get_Meta(&child_meta));
					CComBSTR child_kind_name;
					COMTHROW(child_meta->get_Name(&child_kind_name));
					// Only display Propertys and Parameters in ValueFlow, TBValueFlowAspect, All, TestBenchCompositionAspect, and DesignSpace aspects
					bool display_port =
						(child_kind_name != L"Property"
						  && child_kind_name != L"Parameter"
						  && child_kind_name != L"CADProperty"
						  && child_kind_name != L"CADParameter"
						  && child_kind_name != L"CyberParameter")
						|| bstrParentAspect == L"ValueFlowAspect"
						|| bstrParentAspect == L"TBValueFlowAspect"
						|| bstrParentAspect == L"DesignSpaceAspect"
						|| bstrParentAspect == L"All"
						|| bstrParentAspect == L"TestBenchCompositionAspect";
					// TBDriverMonitor: no StructuralInterfaceRoleForwarder ports
					display_port &=
						child_kind_name != L"StructuralInterfaceRoleForwarder"
						|| bstrParentAspect != L"TBDriverMonitorAspect";

					// Except in ValueFlow, show AggregatePort
					display_port |=
						   bstrParentAspect != L"ValueFlow"
						&& bstrParentAspect != L"TBValueFlowAspect"
						&& child_kind_name == L"AggregatePort";

					// In dynamics aspect, Components have only power ports
					display_port &=
						( bstrParentAspect != L"Dynamics" || bstrParentAspect != L"All")
						|| (   child_kind_name == L"HydraulicPowerPort"
							|| child_kind_name == L"ThermalPowerPort"
							|| child_kind_name == L"ElectricalPowerPort"
							|| child_kind_name == L"MagneticPowerPort"
							|| child_kind_name == L"PneumaticPowerPort"
							|| child_kind_name == L"AcousticPowerPort"
							|| child_kind_name == L"RotationalPowerPort"
							|| child_kind_name == L"TranslationalPowerPort"
							|| child_kind_name == L"InSignal"
							|| child_kind_name == L"OutSignal");

					if (   (bstrParentAspect == L"Dynamics" || bstrParentAspect == L"DynamicsAspect")
						&& (child_kind_name == L"StructuralInterface" || child_kind_name == L"StructuralInterfaceForwarder"))
					{
						display_port = false;
					}
					if (   bstrParentAspect == L"SolidModeling"
						&& child_kind_name == L"AggregatePort")
					{
						display_port = false;
					}

					if (display_port && m_spFCO->ParentModel && m_spFCO->ParentModel->Meta->Name == _bstr_t(L"Configurations"))
					{
						display_port = false;
					}
					if (child_kind_name == L"Property" && mgaFco->GetBoolAttrByName(L"IsProminent") == VARIANT_FALSE)
					{
						display_port = false;
					}

					if (display_port && vbLinked) {
						long lX = 0;
						long lY = 0;
						//COMTHROW( spPart->GetGmeAttrs( 0, &lX, &lY ) );
						// zolmol, in case regnodes are not present or invalid will throw otherwise
						if(spPart->GetGmeAttrs(0, &lX, &lY) != S_OK)
							ASSERT(0);

						//new code
						CComBSTR bstr;
						COMTHROW(spMetaPart->get_Name(&bstr));
						CString pname(bstr);
						_bstr_t icon;
						mgaFco->GetRegistryValueDisp(_bstr_t(PREF_PORTICON), icon.GetAddress());
						if (icon.length() == 0)
							mgaFco->GetRegistryValueDisp(_bstr_t(PREF_ICON), icon.GetAddress());
						if (icon.length() == 0)
							icon = L"Component_piece.png";

						PortPart* portPart = new PortPart(static_cast<TypeableBitmapPart*> (this), m_eventSink, CPoint(lX, lY), icon);
						PortPartData* partData = new PortPartData(portPart, spMetaPart, MGACOLL_ITER);
						m_AllPorts.push_back(partData);
					} else {
						COMTHROW(MGACOLL_ITER->Close());
					}
				} else {
					COMTHROW(MGACOLL_ITER->Close());
				}
			}
		} MGACOLL_ITERATE_END;

		OrderPorts();
	}
}

struct PortLess
{
	bool operator()(Decor::PortPart* pPortA, Decor::PortPart* pPortB)
	{
		CPoint posA = pPortA->GetInnerPosition();
		CPoint posB = pPortB->GetInnerPosition();

		if (posA.y != posB.y)
			return posA.y < posB.y;

		if (posA.x != posB.x)
			return posA.x < posB.x;

		_bstr_t pPortAName = pPortA->GetFCO()->Name;
		_bstr_t pPortBName = pPortB->GetFCO()->Name;
		const wchar_t* zPortAName = pPortAName.length() == 0 ? L"" : pPortAName;
		const wchar_t* zPortBName = pPortBName.length() == 0 ? L"" : pPortBName;
		return (wcscmp(zPortAName, zPortBName) < 0);
	}
};

void ModelComplexPart::OrderPorts()
{
	long lMin = 100000000;
	long lMax = 0;

	for (std::vector<Decor::PortPartData*>::iterator ii = m_AllPorts.begin(); ii != m_AllPorts.end(); ++ii) {
		lMin = min(lMin, (*ii)->portPart->GetInnerPosition().x);
		lMax = max(lMax, (*ii)->portPart->GetInnerPosition().x);
	}

	for (std::vector<Decor::PortPartData*>::iterator ii = m_AllPorts.begin(); ii != m_AllPorts.end(); ++ii) {
		PreferenceMap mapPrefs;
		mapPrefs[PREF_LABELCOLOR]		= PreferenceVariant(m_crPortText);
		mapPrefs[PREF_LABELLENGTH]		= PreferenceVariant((long) m_iMaxPortTextLength);
		mapPrefs[PREF_PORTLABELINSIDE]	= PreferenceVariant(m_bPortLabelInside);

		if ((*ii)->portPart->GetInnerPosition().x <= WIDTH_MODELSIDE ||
			(*ii)->portPart->GetInnerPosition().x < lMax / 2)
		{
			mapPrefs[PREF_LABELLOCATION] = PreferenceVariant(m_bPortLabelInside ? L_EAST : L_WEST);
			m_LeftPorts.push_back((*ii)->portPart);
		} else {
			mapPrefs[PREF_LABELLOCATION] = PreferenceVariant(m_bPortLabelInside? L_WEST: L_EAST);
			m_RightPorts.push_back((*ii)->portPart);
		}

		(*ii)->portPart->InitializeEx(m_spProject, (*ii)->spPart, (*ii)->spFCO, m_parentWnd, mapPrefs);
		long k = (*ii)->portPart->GetLongest();
		if (m_iLongestPortTextLength < k)
		{
			m_iLongestPortTextLength = k;
		}
	}

	std::sort(m_LeftPorts.begin(), m_LeftPorts.end(), PortLess());
	std::sort(m_RightPorts.begin(), m_RightPorts.end(), PortLess());
}

void ModelComplexPart::ReOrderConnectedOnlyPorts()
{
	long lMin = 100000000;
	long lMax = 0;

	m_LeftPorts.clear();
	m_RightPorts.clear();
	m_iLongestPortTextLength = 0;

	for (std::vector<PortPartData*>::iterator ii = m_AllPorts.begin(); ii != m_AllPorts.end(); ++ii) {
		lMin = min(lMin, (*ii)->portPart->GetInnerPosition().x);
		lMax = max(lMax, (*ii)->portPart->GetInnerPosition().x);
	}

	for (std::vector<PortPartData*>::iterator ii = m_AllPorts.begin(); ii != m_AllPorts.end(); ++ii) {
		PreferenceMap mapPrefs;
		mapPrefs[PREF_LABELCOLOR]		= PreferenceVariant(m_crPortText);
		mapPrefs[PREF_LABELLENGTH]		= PreferenceVariant((long) m_iMaxPortTextLength);
		mapPrefs[PREF_PORTLABELINSIDE]	= PreferenceVariant(m_bPortLabelInside);

		bool needThisPort = false;
		CComPtr<IMgaConnPoints> cps;
		(*ii)->spFCO->get_PartOfConns(&cps);
		long num;
		cps->get_Count(&num);
		if(num > 0)
		{
			MGACOLL_ITERATE( IMgaConnPoint , cps ) {
				CComPtr<IMgaConnPoint> cp = MGACOLL_ITER;
				CComPtr<IMgaFCOs> refs;
				COMTHROW( cp->get_References( &refs));
				long l;
				COMTHROW( refs->get_Count( &l));

				CComPtr<IMgaConnection> conn;
				COMTHROW( cp->get_Owner( &conn));
				CComPtr<IMgaParts> connParts;
				COMTHROW(conn->get_Parts(&connParts));
				CComPtr<IMgaMetaAspect>	spParentAspect;
				COMTHROW(m_spPart->get_ParentAspect(&spParentAspect));
				bool aspectMatch = false;
				MGACOLL_ITERATE( IMgaPart , connParts ) {
					CComPtr<IMgaPart> connPart = MGACOLL_ITER;
					CComPtr<IMgaMetaAspect> connAsp;
					COMTHROW(connPart->get_MetaAspect(&connAsp));
					if(connAsp == spParentAspect)
					{
						aspectMatch = true;
						break;
					}
				} MGACOLL_ITERATE_END;

				if(aspectMatch)
				{
					CComPtr<IMgaModel> container;
					COMTHROW( conn->get_ParentModel( &container));
					CComPtr<IMgaFCO> parent = m_parentPart->GetFCO();
					CComPtr<IMgaModel> grandparent;
					COMTHROW(parent->get_ParentModel(&grandparent));

					if(l == 0)
					{
						needThisPort = (parent == m_spFCO && container == grandparent);
					}
					else
					{
						CComPtr<IMgaFCO> ref;
						COMTHROW( refs->get_Item( 1, &ref));
						needThisPort = (ref == parent && container == grandparent);
					}
					if(needThisPort)
						break;
				}
			} MGACOLL_ITERATE_END;
		}

		if(needThisPort)
		{
			if ((*ii)->portPart->GetInnerPosition().x <= WIDTH_MODELSIDE ||
				(*ii)->portPart->GetInnerPosition().x < lMax / 2)
			{
				mapPrefs[PREF_LABELLOCATION] = PreferenceVariant(m_bPortLabelInside ? L_EAST : L_WEST);
				m_LeftPorts.push_back((*ii)->portPart);
			} else {
				mapPrefs[PREF_LABELLOCATION] = PreferenceVariant(m_bPortLabelInside? L_WEST: L_EAST);
				m_RightPorts.push_back((*ii)->portPart);
			}

			long k = (*ii)->portPart->GetLongest();
			if (m_iLongestPortTextLength < k)
				m_iLongestPortTextLength = k;
		}
	}

	std::sort(m_LeftPorts.begin(), m_LeftPorts.end(), PortLess());
	std::sort(m_RightPorts.begin(), m_RightPorts.end(), PortLess());
}

void ModelComplexPart::SetBoxLocation(const CRect& cRect)
{
	TypeableBitmapPart::SetBoxLocation(cRect);
	long lY = (m_Rect.Height() - m_LeftPorts.size() * (HEIGHT_PORT + GAP_PORT) + GAP_PORT) / 2;

	for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
		CRect boxRect = CRect(GAP_XMODELPORT, lY, GAP_XMODELPORT + WIDTH_PORT, lY + HEIGHT_PORT);
		boxRect.OffsetRect(cRect.TopLeft());
		(*ii)->SetBoxLocation(boxRect);
		lY += HEIGHT_PORT + GAP_PORT;
	}
	lY = (m_Rect.Height() - m_RightPorts.size() * (HEIGHT_PORT + GAP_PORT) + GAP_PORT) / 2;
	for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
		CRect boxRect = CRect(cRect.Width() - GAP_XMODELPORT - WIDTH_PORT, lY, cRect.Width() - GAP_XMODELPORT, lY + HEIGHT_PORT);
		boxRect.OffsetRect(cRect.TopLeft());
		(*ii)->SetBoxLocation(boxRect);
		lY += HEIGHT_PORT + GAP_PORT;
	}
}

void ModelComplexPart::SetReferenced(bool referenced)
{
	for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
		(*ii)->SetReferenced(referenced);
	}
	for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
		(*ii)->SetReferenced(referenced);
	}
	TypeableBitmapPart::SetReferenced(referenced);
}

void ModelComplexPart::SetParentPart(PartBase* pPart)
{
	for (std::vector<Decor::PortPart*>::iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
		(*ii)->SetParentPart(pPart);
	}
	for (std::vector<Decor::PortPart*>::iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
		(*ii)->SetParentPart(pPart);
	}
	TypeableBitmapPart::SetParentPart(pPart);
}

Decor::PortPart* ModelComplexPart::GetPort(CComPtr<IMgaFCO> spFCO) const
{
	for (std::vector<Decor::PortPart*>::const_iterator ii = m_LeftPorts.begin(); ii != m_LeftPorts.end(); ++ii) {
		if (spFCO == (*ii)->GetFCO()) {
			return (*ii);
		}
	}
	for (std::vector<Decor::PortPart*>::const_iterator ii = m_RightPorts.begin(); ii != m_RightPorts.end(); ++ii) {
		if (spFCO == (*ii)->GetFCO()) {
			return (*ii);
		}
	}
	return NULL;
}

}; // namespace Decor
