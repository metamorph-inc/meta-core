/*
*    Copyright (c) Vanderbilt University,  2011
*    ALL RIGHTS RESERVED
*
*/ 


//################################################################################################
//
// CPM decorator base part class
//	CPMBasePart.h
// Contains the specific decorator parts which compose the decorator
//
//################################################################################################

#ifndef __CPMBASEPART_H_
#define __CPMBASEPART_H_


#include "StdAfx.h"
#include "VectorPart.h"
#include "CPMAttributePart.h"
#include "ClassLabelPart.h"

using namespace DecoratorSDK;

namespace Decor {

class RelativeCoordCommand: public CoordCommand {
	public:
		enum Location {
			LEFT =   reinterpret_cast<int>(&((static_cast<CRect*>(0))->left)),
			TOP =    reinterpret_cast<int>(&((static_cast<CRect*>(0))->top)),
			RIGHT =  reinterpret_cast<int>(&((static_cast<CRect*>(0))->right)),
			BOTTOM = reinterpret_cast<int>(&((static_cast<CRect*>(0))->bottom)),
		};

	private:
	long	m_Coord;
	Location m_Location;

	public:

	RelativeCoordCommand(Location location, long coord): CoordCommand(), m_Location(location), m_Coord(coord) {};
	virtual ~RelativeCoordCommand() {};

	virtual long	ResolveCoordinate	(const CRect& extents) const { return m_Coord + 
		*(reinterpret_cast<const LONG*>(reinterpret_cast<const char*>(&extents) + m_Location)); };
};



//################################################################################################
//
// CLASS : CPMBasePart
//
//################################################################################################

class CPMBasePart: public DecoratorSDK::VectorPart
{
protected:
	ClassLabelPart*							m_LabelPart;
	std::vector<DecoratorSDK::PartInterface*>	m_AttributeParts;
	std::vector< std::vector<IInternalPart*> >				m_InternalParts;

	CSize						m_calcSize;
	long						m_lMaxTextWidth;
	long						m_lMaxTextHeight;
	long						m_lMinTextWidth;
	long						m_lMinTextHeight;
	COLORREF m_backgroundColor;

public:
	CPMBasePart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, COLORREF color);
	virtual ~CPMBasePart();

// =============== resembles IMgaElementDecorator
public:
	virtual void			Initialize			(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO);
	virtual void			Destroy				(void);
	virtual CString			GetMnemonic			(void) const;
	virtual feature_code	GetFeatures			(void) const;
	virtual void			SetParam			(const CString& strName, VARIANT vValue);
	virtual bool			GetParam			(const CString& strName, VARIANT* pvValue);
	virtual void			SetActive			(bool bIsActive);
	virtual CSize			GetPreferredSize	(void) const;
	virtual void			SetLocation			(const CRect& location);
	virtual CRect			GetLocation			(void) const;
	virtual CRect			GetLabelLocation	(void) const;
	virtual void			Draw				(CDC* pDC, Gdiplus::Graphics* gdip);
	virtual void			SaveState			(void);

	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences = PreferenceMap());
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

	virtual void	CalcRelPositions			(CDC* pDC = NULL, Gdiplus::Graphics* gdip = NULL);
	virtual void	AddBorderCommands			(CDC* pDC, Gdiplus::Graphics* gdip);
	virtual void	SetBoxLocation				(const CRect& cRect);
	virtual void	SetReferenced				(bool referenced);
	virtual void	SetParentPart				(PartBase* pPart);
	virtual void	AddInternalPart(IInternalPart* part, PartInterface* part2);
	virtual void	AddInternalPart(IInternalPart* part);
	virtual void	AddBottomLabel(CComPtr<IMgaFCO>& pFCO, CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
								 HWND& parentWnd, DecoratorSDK::PreferenceMap& preferences);

	virtual void	NewInternalPartRow();

};

}; // namespace Decor

#endif //__CPMBASEPART_H_
