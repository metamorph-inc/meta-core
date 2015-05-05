//################################################################################################
//
// Triangle vector part decorator class
//	TriangleVectorPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "TriangleVectorPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : TriangleVectorPart
//
//################################################################################################

TriangleVectorPart::TriangleVectorPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink,
									   long triangleWidth, long triangleHeight):
	VectorPart(pPart, eventSink, triangleWidth, triangleHeight)
{
}

TriangleVectorPart::~TriangleVectorPart()
{
}

// New functions
void TriangleVectorPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
									  HWND parentWnd, PreferenceMap& preferences)
{
	VectorPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);

	SimpleCoordCommand* leftMost		= new SimpleCoordCommand(LeftMost);
	SimpleCoordCommand* topMost			= new SimpleCoordCommand(TopMost);
	SimpleCoordCommand* rightMost		= new SimpleCoordCommand(RightMost);
	SimpleCoordCommand* bottomMost		= new SimpleCoordCommand(BottomMost);
	ComplexCoordCommand* triangletip	= new ComplexCoordCommand(LeftMost);
	triangletip->AddCommand(RightMost, 0.5, CoordAdd);
	triangletip->AddCommand(LeftMost, 0.5, CoordSubstract);
	m_coordCommands.push_back(leftMost);
	m_coordCommands.push_back(topMost);
	m_coordCommands.push_back(rightMost);
	m_coordCommands.push_back(bottomMost);
	m_coordCommands.push_back(triangletip);

	AddCommand(VectorCommand(VectorCommand::BeginPath));
	AddCommand(VectorCommand(leftMost,		bottomMost,	rightMost,		bottomMost,	VectorCommand::AddLineToPath));
	AddCommand(VectorCommand(rightMost,		bottomMost,	triangletip,	topMost,	VectorCommand::AddLineToPath));
	AddCommand(VectorCommand(triangletip,	topMost,	leftMost,		bottomMost,	VectorCommand::AddLineToPath));
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
