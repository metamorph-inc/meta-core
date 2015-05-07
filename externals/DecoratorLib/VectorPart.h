//################################################################################################
//
// Vector part class (decorator part)
//	VectorPart.h
//
//################################################################################################

#ifndef __VECTORPART_H_
#define __VECTORPART_H_


#include "StdAfx.h"
#include "ResizablePart.h"
#include <vector>


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : CoordinateConstants
//
//################################################################################################

enum CoordinateConstants
{
	ZeroConstant		= 0,
	LeftMost			= 1,
	TopMost				= 2,
	RightMost			= 3,
	BottomMost			= 4,
	OneConstant			= 5
};

enum CoordinateOperations
{
	CoordAdd			= 0,
	CoordSubstract		= 1,
	CoordDivide			= 2,
	CoordMultiply		= 3
};

//################################################################################################
//
// CLASS : CoordCommand
//
//################################################################################################

class CoordCommand {
public:
	CoordCommand() {};
	virtual ~CoordCommand() {};

	virtual long	ResolveCoordinate	(const CRect& extents) const { return 0; };
};

//################################################################################################
//
// CLASS : NopCoordCommand
//
//################################################################################################

class NopCoordCommand: public CoordCommand {
	CoordinateConstants	m_CoordConst;

public:
	NopCoordCommand(): CoordCommand() {};
	virtual ~NopCoordCommand() {};
};

//################################################################################################
//
// CLASS : SimpleCoordCommand
//
//################################################################################################

class SimpleCoordCommand: public CoordCommand {
	CoordinateConstants	m_CoordConst;

public:
	SimpleCoordCommand(CoordinateConstants coordConst): CoordCommand(), m_CoordConst(coordConst) {};
	virtual ~SimpleCoordCommand() {};

	virtual long	ResolveCoordinate	(const CRect& extents) const;
};

//################################################################################################
//
// CLASS : AbsoluteCoordCommand
//
//################################################################################################

class AbsoluteCoordCommand: public CoordCommand {
	long	m_Coord;

public:
	AbsoluteCoordCommand(long coord): CoordCommand(), m_Coord(coord) {};
	virtual ~AbsoluteCoordCommand() {};

	virtual long	ResolveCoordinate	(const CRect& extents) const { return m_Coord; };
};

//################################################################################################
//
// CLASS : ComplexCoordCommand
//
//################################################################################################

class ComplexCoordCommand: public CoordCommand {
	std::vector<CoordinateConstants>	m_coordConstList;
	std::vector<double>					m_coordWeightList;
	std::vector<CoordinateOperations>	m_coordOperationList;

public:
	ComplexCoordCommand(CoordinateConstants coordConst, double weight = 1.0, CoordinateOperations operation = CoordAdd);
	virtual ~ComplexCoordCommand();

	void AddCommand(CoordinateConstants constant, double weight, CoordinateOperations operation);

	virtual long	ResolveCoordinate	(const CRect& extents) const;
};

//################################################################################################
//
// CLASS : VectorCommand
//
//################################################################################################

class VectorCommand {
public:
	enum Codes {
		DrawLine				= 0,
		DrawRectangle			= 1,
		DrawEllipse				= 2,
		DrawPolygon				= 3,
		FillRectangle			= 4,
		FillEllipse				= 5,
		FillPolygon				= 6,
		SelectPen				= 7,
		SelectBrush				= 8,
		BeginPath				= 9,
		EndPath					= 10,
		StrokePath				= 11,
		FillPath				= 12,
		StrokeAndFillPath		= 13,
		AddLineToPath			= 14,
		AddRectangleToPath		= 15,
		AddRoundRectangleToPath	= 16,
		AddEllipseToPath		= 17,
		AddPolygonToPath		= 18,
		AddArcToPath			= 19,
		BeginShadowPath			= 20,
		EndShadowPath			= 21,
		CopyShadowPath			= 22,
		CastShadowPath			= 23
	};

public:
	VectorCommand(long code):
		m_Code(code) {};
	VectorCommand(const CoordCommand* sxCmd, long code):
		m_Code(code) { m_CoordCmds.push_back(sxCmd); };
	VectorCommand(const CoordCommand* sxCmd, const CoordCommand* syCmd, long code):
		m_Code(code) { m_CoordCmds.push_back(sxCmd); m_CoordCmds.push_back(syCmd); };
	VectorCommand(const CoordCommand* sxCmd, const CoordCommand* syCmd, const CoordCommand* exCmd, const CoordCommand* eyCmd, long code):
		m_Code(code) { m_CoordCmds.push_back(sxCmd); m_CoordCmds.push_back(syCmd); m_CoordCmds.push_back(exCmd); m_CoordCmds.push_back(eyCmd); };
	VectorCommand(std::vector<const CoordCommand*> cmds, long code):
		m_Code(code), m_CoordCmds(cmds) {};
	virtual ~VectorCommand();

	CRect	GetResolvedCoords	(const CRect& extents) const;
	std::vector<long>			GetResolvedValues(const CRect& extents) const;
	long	GetCode				(void) const { return m_Code; };

protected:
	std::vector<const CoordCommand*>	m_CoordCmds;
	long								m_Code;
};

//################################################################################################
//
// CLASS : VectorPart
//
//################################################################################################

class VectorPart: public ResizablePart
{
protected:
	// Preference variable name strings
	CString						penColorVariableName;
	CString						brushColorVariableName;
	CString						gradientColorVariableName;
	CString						shadowColorVariableName;

	COLORREF					m_crPen;
	COLORREF					m_crBrush;
	COLORREF					m_crShadow;
	bool						m_bCastShadow;
	bool						m_bGradientFill;
	long						m_iShadowThickness;
	long						m_iShadowDirection;
	COLORREF					m_crGradient;
	long						m_iGradientDirection;

	std::vector<VectorCommand>	m_Commands;

	Gdiplus::GraphicsPath*		m_mainPath;
	Gdiplus::GraphicsPath*		m_shadowPath;
	bool						m_bInMainPathDefinition;
	bool						m_bInShadowPathDefinition;
	bool						m_bShadowCasted;
	Gdiplus::Brush*				m_CurrentBrush;
	Gdiplus::Pen*				m_CurrentPen;

	long						m_iDefaultWidth;
	long						m_iDefaultHeight;

	std::vector<CoordCommand*>	m_coordCommands;

public:
	VectorPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink,
			   long defaultWidth = 0, long defaultHeight = 0);
	virtual ~VectorPart();

	void			AddCommand					(const VectorCommand& cmd) { m_Commands.push_back(cmd); };
	long			GetCommandNumber			(void) const { return (long)m_Commands.size(); };
	void			RemoveLastCommand			(long cmdCount = 1);
	void			RemoveCommand				(long index);
	void			RemoveAllCommands			(void) { m_Commands.clear(); };
	void			DisposeCoordCommands		(void);
	VectorCommand	GetCommand					(long index) const;

// =============== resembles IMgaElementDecorator
public:
	virtual CSize			GetPreferredSize	(void) const;

	virtual void			Draw				(CDC* pDC, Gdiplus::Graphics* gdip);

	virtual void			InitializeEx		(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);
};

}; // namespace DecoratorSDK

#endif //__VECTORPART_H_
