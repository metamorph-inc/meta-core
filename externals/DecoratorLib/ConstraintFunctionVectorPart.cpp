//################################################################################################
//
// Constraint Function vector part decorator class
//	ConstraintFunctionVectorPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "ConstraintFunctionVectorPart.h"

#include "DecoratorDefs.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ConstraintFunctionVectorPart
//
//################################################################################################

ConstraintFunctionVectorPart::ConstraintFunctionVectorPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink,
														   COLORREF fillColor, double thicknessRatio,
														   long constraintWidth, long constraintHeight):
	ConstraintVectorPart(pPart, eventSink, fillColor, thicknessRatio, constraintWidth, constraintHeight)
{
}

ConstraintFunctionVectorPart::~ConstraintFunctionVectorPart()
{
}

// New functions
void ConstraintFunctionVectorPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
									  HWND parentWnd, PreferenceMap& preferences)
{
	ConstraintVectorPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);

	AbsoluteCoordCommand* penColorCmd		= new AbsoluteCoordCommand(m_crPen);
	AbsoluteCoordCommand* garyedColorCmd	= new AbsoluteCoordCommand(COLOR_GRAYED_OUT);

	m_coordCommands.push_back(penColorCmd);
	m_coordCommands.push_back(garyedColorCmd);

	AddCommand(VectorCommand(penColorCmd, garyedColorCmd, VectorCommand::SelectPen));
	AddCommand(VectorCommand(penColorCmd, garyedColorCmd, VectorCommand::SelectBrush));

	double ratio1 = 1.0 / 3.0;
	double ratio2 = ratio1 / 3.0;
	ComplexCoordCommand* fLeft	= new ComplexCoordCommand(LeftMost, 1.0 - ratio1);
	fLeft->AddCommand(RightMost, ratio1, CoordAdd);
	ComplexCoordCommand* fx1	= new ComplexCoordCommand(LeftMost, 1.0 - (ratio1 + m_thicknessRatio));
	fx1->AddCommand(RightMost, ratio1 + m_thicknessRatio, CoordAdd);
	ComplexCoordCommand* fx2	= new ComplexCoordCommand(LeftMost, ratio1 + m_thicknessRatio / 3.0);
	fx2->AddCommand(RightMost, 1.0 - (ratio1 + m_thicknessRatio / 3.0), CoordAdd);
	ComplexCoordCommand* fRight	= new ComplexCoordCommand(LeftMost, ratio1);
	fRight->AddCommand(RightMost, 1.0 - ratio1, CoordAdd);

	ComplexCoordCommand* fTop	= new ComplexCoordCommand(TopMost, 1.0 - 1.5 * m_thicknessRatio);
	fTop->AddCommand(BottomMost, 1.5 * m_thicknessRatio, CoordAdd);
	ComplexCoordCommand* fy1	= new ComplexCoordCommand(TopMost, 1.0 - 2.5 * m_thicknessRatio);
	fy1->AddCommand(BottomMost, 2.5 * m_thicknessRatio, CoordAdd);
	ComplexCoordCommand* fy2	= new ComplexCoordCommand(TopMost, 1.0 - 3.5 * m_thicknessRatio);
	fy2->AddCommand(BottomMost, 3.5 * m_thicknessRatio, CoordAdd);
	ComplexCoordCommand* fy3	= new ComplexCoordCommand(TopMost, 1.0 - 4.5 * m_thicknessRatio);
	fy3->AddCommand(BottomMost, 4.5 * m_thicknessRatio, CoordAdd);
	ComplexCoordCommand* fBott	= new ComplexCoordCommand(TopMost, ratio1 - m_thicknessRatio);
	fBott->AddCommand(BottomMost, 1.0 - ratio1 + m_thicknessRatio, CoordAdd);

	m_coordCommands.push_back(fLeft);
	m_coordCommands.push_back(fx1);
	m_coordCommands.push_back(fx2);
	m_coordCommands.push_back(fRight);
	m_coordCommands.push_back(fTop);
	m_coordCommands.push_back(fy1);
	m_coordCommands.push_back(fy2);
	m_coordCommands.push_back(fy3);
	m_coordCommands.push_back(fBott);

	std::vector<const CoordCommand*> polygonPoints;
	polygonPoints.push_back(fLeft);
	polygonPoints.push_back(fTop);
	polygonPoints.push_back(fLeft);
	polygonPoints.push_back(fBott);
	polygonPoints.push_back(fx1);
	polygonPoints.push_back(fBott);
	polygonPoints.push_back(fx1);
	polygonPoints.push_back(fy3);
	polygonPoints.push_back(fx2);
	polygonPoints.push_back(fy3);
	polygonPoints.push_back(fx2);
	polygonPoints.push_back(fy2);
	polygonPoints.push_back(fx1);
	polygonPoints.push_back(fy2);
	polygonPoints.push_back(fx1);
	polygonPoints.push_back(fy1);
	polygonPoints.push_back(fRight);
	polygonPoints.push_back(fy1);
	polygonPoints.push_back(fRight);
	polygonPoints.push_back(fTop);

	AddCommand(VectorCommand(polygonPoints, VectorCommand::FillPolygon));
}

}; // namespace DecoratorSDK
