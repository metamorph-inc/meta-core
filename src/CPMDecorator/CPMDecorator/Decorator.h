//################################################################################################
//
// Decorator class
//	Decorator.h
// This class represents the C++ side of the decorator COM connection layer
// This is still just a relay class like teh COM/ATL SampleDecoratorImpl.h,cpp,
// the meat should go into the composite part or into the parts in the composite part
//
//################################################################################################

#ifndef __DECORATOR_H_
#define __DECORATOR_H_


#include "StdAfx.h"
#include "DecoratorInterface.h"
#include "PartInterface.h"
#include <memory>

#define CPM_NAMESPACE			L"CPM::"
#define CPM_NAMESPACE_LENGTH	5

// icon is RGB(0xcc, 0xff, 0xcc)
#define PARAM_TEXT_COLOR RGB(0x7e, 0x9e, 0x7e)
#define PARAM_DROP_COLOR PARAM_TEXT_COLOR
// icon is RGB(0xe3, 0xdd, 0x8e)
#define PROP_TEXT_COLOR RGB(0xc4, 0xbf, 0x7b)
#define PROP_DROP_COLOR PROP_TEXT_COLOR

// CPM Attributes

#define  CPM_PARAMETER_RANGE_ATTR			L"Range"
#define  CPM_PARAMETER_VALUE_ATTR			L"Value"


#define  CPM_PROPERTY_TOLERANCE_ATTR		L"Tolerance"
#define  CPM_PROPERTY_VALUE_ATTR			L"Value"



//CPM KINDS
#define	 CPM_COMPONENT_OBJ				L"Component"
#define	 CPM_TESTCOMPONENT_OBJ			L"TestComponent"
#define	 CPM_CYBERCOMPONENT_OBJ			L"CyberComponent"

#define	 CPM_PARAMETER_OBJ				L"Parameter"
#define	 CPM_PROPERTY_OBJ				L"Property"
#define	 CPM_METRIC_OBJ					L"Metric"
#define	 CPM_CARPARAMETER_OBJ			L"CarParameter"

#define	 CPM_THERMAL_PORT_OBJ			L"ThermalPowerPort"
#define	 CPM_HYDROLIC_PORT_OBJ			L"HydraulicPowerPort"
#define	 CPM_ELECTRICAL_PORT_OBJ		L"ElectricalPowerPort"
#define	 CPM_MAGNETIC_PORT_OBJ			L"MagneticPowerPort"
#define	 CPM_ELECTROMAGNETIC_PORT_OBJ	L"ElectromagneticPort"
#define	 CPM_ROTATIONAL_PORT_OBJ		L"RotationalPowerPort"
#define	 CPM_TRANSLATIONAL_PORT_OBJ		L"TranslationalPowerPort"
#define	 CPM_PNEUMATIC_PORT_OBJ			L"PneumaticPowerPort"
#define	 CPM_ACOUSTIC_PORT_OBJ			L"AcousticPowerPort"
#define  CPM_AGGREGATE_PORT_OBJ			L"AggregatePort"

#define	 CPM_ELECTRONICSIGNAL_PORT_OBJ	L"ElectricalSignalPort"
#define	 CPM_SOFTWARE_PORT_OBJ			L"SoftwareCommunicationPort"






namespace Decor {
namespace Type {
typedef enum {
	none_type = 0,
  component_type = 1,
  testcomponent_type,
  cybercomponent_type,
  componentassembly_type,
  alternativerepresentationcontainer_type,
  componentref_type,
  componentassemblyref_type,
  testinjectionpoint_type,
  toplevelsystemundertest_type,
  testbenchref_type,
  designentityref_type,
  parameter_type,
  property_type,
  carparameter_type,
  metric_type,
  thermal_type,
  hydrolic_type,
  electrical_type,
  magnetic_type,
  electromagnetic_type,
  rotational_type,
  translational_type,
  acoustic_type,
  pneumatic_type,
  electronicsignal_type,
  software_type,
  documentation_type,
} cpm_type;
};
};

#define	 CPM_DECORATOR_MARGINX		6
#define	 CPM_DECORATOR_MARGINY		6
#define	 CPM_DECORATOR_BORDERSIZE	5
#define	 CPM_DECORATOR_GAPY			0
#define  CPM_DECORATOR_MINATTRSIZE	13
#define GME_GRID_SIZE 7

#define  CPM_DECORATOR_MAXROWSIZE	80

// These should be unique
#define CPM_FONT_ITALICS_INDEX 0x59BEBD8E
#define CPM_FONT_ITALICS_BOLD_INDEX (CPM_FONT_ITALICS_INDEX + 1)
#define CPM_FONT_BOLD_INDEX (CPM_FONT_ITALICS_INDEX + 2)





namespace Decor {

//################################################################################################
//
// CLASS : Decorator
//
//################################################################################################

class Decorator: public DecoratorSDK::DecoratorInterface
{
protected:
	DecoratorSDK::PartInterface*		m_part;
	CComPtr<IMgaCommonDecoratorEvents>	m_eventSink;
	std::unique_ptr<DecoratorSDK::TileVector> m_tiles;

public:
	Decorator(CComPtr<IMgaCommonDecoratorEvents>& eventSink);
	virtual ~Decorator();

	//static utility methods to get element type, port type, determine if port is input/output
	static Type::cpm_type GetType(CString& type);

// =============== resembles IMgaElementDecorator
public:
	virtual void			Initialize			(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO);
	virtual void			Destroy				(void);
	virtual CString			GetMnemonic			(void) const;
	virtual feature_code	GetFeatures			(void) const;
	virtual void			SetParam			(const CString& strName, VARIANT vValue);
	virtual void			GetParam			(const CString& strName, VARIANT* pvValue);
	virtual void			SetActive			(bool bIsActive);
	virtual CSize			GetPreferredSize	(void) const;
	virtual void			SetLocation			(const CRect& location);
	virtual CRect			GetLocation			(void) const;
	virtual CRect			GetLabelLocation	(void) const;
	virtual CRect			GetPortLocation		(CComPtr<IMgaFCO>& fco) const;
	virtual void			GetPorts			(CComPtr<IMgaFCOs>& portFCOs) const;
	virtual void			Draw				(CDC* pDC, Gdiplus::Graphics* gdip);
	virtual void			SaveState			(void);

	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd);
	

	virtual void	SetSelected					(bool bIsSelected);
	virtual bool	MouseMoved					(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseLeftButtonDown			(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseLeftButtonUp			(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseLeftButtonDoubleClick	(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseRightButtonDown		(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseRightButtonUp			(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseRightButtonDoubleClick	(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseMiddleButtonDown		(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseMiddleButtonUp			(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseMiddleButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseWheelTurned			(UINT nFlags, short distance, const CPoint& point, HDC transformHDC);
	virtual bool	DragEnter					(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC);
	virtual bool	DragOver					(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC);
	virtual bool	Drop						(COleDataObject* pDataObject, DROPEFFECT dropEffect, const CPoint& point, HDC transformHDC);
	virtual bool	DropFile					(HDROP p_hDropInfo, const CPoint& point, HDC transformHDC);
	virtual bool	MenuItemSelected			(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	OperationCanceledByGME		(void);

	
};

}; // namespace Decor

#endif //__DECORATOR_H_
