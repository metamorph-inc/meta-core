//################################################################################################
//
// Ellipse vector part decorator class
//	EllipseVectorPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "EllipseVectorPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : EllipseVectorPart
//
//################################################################################################

EllipseVectorPart::EllipseVectorPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink,
									 long ellipseWidth, long ellipseHeight):
	VectorPart(pPart, eventSink, ellipseWidth, ellipseHeight)
{
}

EllipseVectorPart::~EllipseVectorPart()
{
}

// New functions
void EllipseVectorPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
									 HWND parentWnd, PreferenceMap& preferences)
{
	VectorPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);

	SimpleCoordCommand* leftMost	= new SimpleCoordCommand(LeftMost);
	SimpleCoordCommand* topMost		= new SimpleCoordCommand(TopMost);
	SimpleCoordCommand* rightMost	= new SimpleCoordCommand(RightMost);
	SimpleCoordCommand* bottomMost	= new SimpleCoordCommand(BottomMost);
	m_coordCommands.push_back(leftMost);
	m_coordCommands.push_back(topMost);
	m_coordCommands.push_back(rightMost);
	m_coordCommands.push_back(bottomMost);

	AddCommand(VectorCommand(VectorCommand::BeginPath));
	AddCommand(VectorCommand(leftMost, topMost, rightMost, bottomMost, VectorCommand::AddEllipseToPath));
	AddCommand(VectorCommand(VectorCommand::EndPath));
	AddCommand(VectorCommand(VectorCommand::CopyShadowPath));
	AddCommand(VectorCommand(VectorCommand::CastShadowPath));
	AbsoluteCoordCommand* fillColorCmd = new AbsoluteCoordCommand(m_crBrush);
	AbsoluteCoordCommand* grayedCmd = new AbsoluteCoordCommand(COLOR_GRAYED_OUT);
	m_coordCommands.push_back(fillColorCmd);
	m_coordCommands.push_back(grayedCmd);
	AddCommand(VectorCommand(fillColorCmd, grayedCmd, VectorCommand::StrokeAndFillPath));
}

}; // namespace DecoratorSDK
