//################################################################################################
//
// Constraint vector part decorator class
//	ConstraintVectorPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "ConstraintVectorPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ConstraintVectorPart
//
//################################################################################################

ConstraintVectorPart::ConstraintVectorPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, COLORREF fillColor,
										   double thicknessRatio, long constraintWidth, long constraintHeight):
	VectorPart			(pPart, eventSink, constraintWidth, constraintHeight),
	m_crFill			(fillColor),
	m_thicknessRatio	(thicknessRatio)
{
}

ConstraintVectorPart::~ConstraintVectorPart()
{
}

// New functions
void ConstraintVectorPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
										HWND parentWnd, PreferenceMap& preferences)
{
	VectorPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);

	COLORREF backgroundColor = COLOR_WHITE;
	PreferenceMap::iterator it = preferences.find(PREF_BORDERCOLOR);
	if (it != preferences.end()) {
		backgroundColor = it->second.uValue.crValue;
	} else {
		getFacilities().getPreference(m_spFCO, m_spMetaFCO, PREF_BORDERCOLOR, backgroundColor);
	}

	double sin45 = 0.70710678118654752440084436210485;
	double sin45plushalf = 0.85355339059327376220042218105242;
	SimpleCoordCommand* leftMost		= new SimpleCoordCommand(LeftMost);
	SimpleCoordCommand* topMost			= new SimpleCoordCommand(TopMost);
	SimpleCoordCommand* rightMost		= new SimpleCoordCommand(RightMost);
	SimpleCoordCommand* bottomMost		= new SimpleCoordCommand(BottomMost);
	ComplexCoordCommand* leftoThick		= new ComplexCoordCommand(LeftMost, 1.0 - m_thicknessRatio);
	leftoThick->AddCommand(RightMost, m_thicknessRatio, CoordAdd);
	leftoThick->AddCommand(OneConstant, 1.0, CoordAdd);	// correction in case of small sizes
	ComplexCoordCommand* topoThick		= new ComplexCoordCommand(TopMost, 1.0 - m_thicknessRatio);
	topoThick->AddCommand(BottomMost, m_thicknessRatio, CoordAdd);
	topoThick->AddCommand(OneConstant, 1.0, CoordAdd);	// correction in case of small sizes
	ComplexCoordCommand* rightoThick	= new ComplexCoordCommand(RightMost, 1.0 - m_thicknessRatio);
	rightoThick->AddCommand(LeftMost, m_thicknessRatio, CoordAdd);
	ComplexCoordCommand* bottomoThick	= new ComplexCoordCommand(BottomMost, 1.0 - m_thicknessRatio);
	bottomoThick->AddCommand(TopMost, m_thicknessRatio, CoordAdd);

	ComplexCoordCommand* rLTx = new ComplexCoordCommand(LeftMost, sin45plushalf * (1.0 - sin45 * m_thicknessRatio));
	rLTx->AddCommand(RightMost, 1.0 - sin45plushalf * (1.0 - sin45 * m_thicknessRatio), CoordAdd);
	ComplexCoordCommand* rLTy = new ComplexCoordCommand(TopMost, sin45plushalf);
	rLTy->AddCommand(BottomMost, 1.0 - sin45plushalf, CoordAdd);
	ComplexCoordCommand* rLBx = new ComplexCoordCommand(LeftMost, sin45plushalf);
	rLBx->AddCommand(RightMost, 1.0 - sin45plushalf, CoordAdd);
	ComplexCoordCommand* rLBy = new ComplexCoordCommand(TopMost, sin45plushalf * (1.0 - sin45 * m_thicknessRatio));
	rLBy->AddCommand(BottomMost, 1.0 - sin45plushalf * (1.0 - sin45 * m_thicknessRatio), CoordAdd);
	ComplexCoordCommand* rRTx = new ComplexCoordCommand(LeftMost, 1.0 - sin45plushalf);
	rRTx->AddCommand(RightMost, sin45plushalf, CoordAdd);
	ComplexCoordCommand* rRTy = new ComplexCoordCommand(TopMost, 1.0 - sin45plushalf * (1.0 - sin45 * m_thicknessRatio));
	rRTy->AddCommand(BottomMost, sin45plushalf * (1.0 - sin45 * m_thicknessRatio), CoordAdd);
	ComplexCoordCommand* rRBx = new ComplexCoordCommand(LeftMost, 1.0 - sin45plushalf * (1.0 - sin45 * m_thicknessRatio));
	rRBx->AddCommand(RightMost, sin45plushalf * (1.0 - sin45 * m_thicknessRatio), CoordAdd);
	ComplexCoordCommand* rRBy = new ComplexCoordCommand(TopMost, 1.0 - sin45plushalf);
	rRBy->AddCommand(BottomMost, sin45plushalf, CoordAdd);

	AbsoluteCoordCommand* bkgndColorCmd		= new AbsoluteCoordCommand(backgroundColor);
	AbsoluteCoordCommand* fillColorCmd		= new AbsoluteCoordCommand(m_crFill);
	AbsoluteCoordCommand* garyedColorCmd	= new AbsoluteCoordCommand(COLOR_GRAYED_OUT);

	m_coordCommands.push_back(leftMost);
	m_coordCommands.push_back(topMost);
	m_coordCommands.push_back(rightMost);
	m_coordCommands.push_back(bottomMost);
	m_coordCommands.push_back(leftoThick);
	m_coordCommands.push_back(topoThick);
	m_coordCommands.push_back(rightoThick);
	m_coordCommands.push_back(bottomoThick);
	m_coordCommands.push_back(rLTx);
	m_coordCommands.push_back(rLTy);
	m_coordCommands.push_back(rLBx);
	m_coordCommands.push_back(rLBy);
	m_coordCommands.push_back(rRTx);
	m_coordCommands.push_back(rRTy);
	m_coordCommands.push_back(rRBx);
	m_coordCommands.push_back(rRBy);
	m_coordCommands.push_back(bkgndColorCmd);
	m_coordCommands.push_back(fillColorCmd);
	m_coordCommands.push_back(garyedColorCmd);

	std::vector<const CoordCommand*> polygonPoints;
	polygonPoints.push_back(rLTx);
	polygonPoints.push_back(rLTy);
	polygonPoints.push_back(rLBx);
	polygonPoints.push_back(rLBy);
	polygonPoints.push_back(rRBx);
	polygonPoints.push_back(rRBy);
	polygonPoints.push_back(rRTx);
	polygonPoints.push_back(rRTy);

	AddCommand(VectorCommand(VectorCommand::BeginShadowPath));
	AddCommand(VectorCommand(leftMost, topMost, rightMost, bottomMost, VectorCommand::AddEllipseToPath));
	AddCommand(VectorCommand(VectorCommand::EndShadowPath));
	AddCommand(VectorCommand(VectorCommand::CastShadowPath));

	AddCommand(VectorCommand(fillColorCmd, garyedColorCmd, VectorCommand::SelectBrush));
	AddCommand(VectorCommand(leftMost, topMost, rightMost, bottomMost, VectorCommand::FillEllipse));
	AddCommand(VectorCommand(bkgndColorCmd, bkgndColorCmd, VectorCommand::SelectBrush));
	AddCommand(VectorCommand(leftoThick, topoThick, rightoThick, bottomoThick, VectorCommand::FillEllipse));
	AddCommand(VectorCommand(fillColorCmd, garyedColorCmd, VectorCommand::SelectBrush));
	AddCommand(VectorCommand(polygonPoints, VectorCommand::FillPolygon));
}

}; // namespace DecoratorSDK
