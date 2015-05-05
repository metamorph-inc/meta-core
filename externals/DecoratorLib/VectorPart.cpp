//################################################################################################
//
// Vector decorator class
//	VectorPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "VectorPart.h"
#define _USE_MATH_DEFINES
#include <math.h>


namespace DecoratorSDK {

static long	ResolveCoord(CoordinateConstants coordConst, const CRect& extents)
{
	switch (coordConst) {
		case ZeroConstant:	return 0;
		case LeftMost:		return extents.left;
		case TopMost:		return extents.top;
		case RightMost:		return extents.right > extents.left ? extents.right - 1 : extents.right;
		case BottomMost:	return extents.bottom > extents.top ? extents.bottom - 1 : extents.bottom;
		case OneConstant:	return 1;
	}
	return -1;
}

//################################################################################################
//
// CLASS : SimpleCoordCommand
//
//################################################################################################

long	SimpleCoordCommand::ResolveCoordinate(const CRect& extents) const
{
	return ResolveCoord(m_CoordConst, extents);
}

//################################################################################################
//
// CLASS : ComplexCoordCommand
//
//################################################################################################

ComplexCoordCommand::ComplexCoordCommand(CoordinateConstants coordConst, double weight, CoordinateOperations operation):
	CoordCommand()
{
	m_coordConstList.push_back(coordConst);
	m_coordWeightList.push_back(weight);
	m_coordOperationList.push_back(operation);
}

ComplexCoordCommand::~ComplexCoordCommand()
{
	m_coordConstList.clear();
	m_coordWeightList.clear();
	m_coordOperationList.clear();
}

void ComplexCoordCommand::AddCommand(CoordinateConstants constant, double weight, CoordinateOperations operation)
{
	m_coordConstList.push_back(constant);
	m_coordWeightList.push_back(weight);
	m_coordOperationList.push_back(operation);
}

long	ComplexCoordCommand::ResolveCoordinate(const CRect& extents) const
{
	double result = 0.0;
	for (unsigned long i = 0; i < m_coordOperationList.size(); i++) {
		switch (m_coordOperationList[i]) {
			case CoordAdd:			result = result + ResolveCoord(m_coordConstList[i], extents) * m_coordWeightList[i]; break;
			case CoordSubstract:	result = result - ResolveCoord(m_coordConstList[i], extents) * m_coordWeightList[i]; break;
			case CoordDivide:		result = result / ResolveCoord(m_coordConstList[i], extents) * m_coordWeightList[i]; break;
			case CoordMultiply:		result = result * ResolveCoord(m_coordConstList[i], extents) * m_coordWeightList[i]; break;
		}
	}

	return static_cast<long> (result);
}

//################################################################################################
//
// CLASS : VectorCommand
//
//################################################################################################

CRect	VectorCommand::GetResolvedCoords(const CRect& extents) const
{
	ASSERT(!extents.IsRectEmpty());
	CRect resolved;
	unsigned long size = m_CoordCmds.size();
	resolved.left	= size >= 1 ? m_CoordCmds[0]->ResolveCoordinate(extents) : 0;
	resolved.top	= size >= 2 ? m_CoordCmds[1]->ResolveCoordinate(extents) : 0;
	resolved.right	= size >= 3 ? m_CoordCmds[2]->ResolveCoordinate(extents) : resolved.left;
	resolved.bottom	= size >= 4 ? m_CoordCmds[3]->ResolveCoordinate(extents) : resolved.top;

	return resolved;
}

std::vector<long> VectorCommand::GetResolvedValues(const CRect& extents) const
{
	std::vector<long> resolvedValues;
	for (std::vector<const CoordCommand*>::const_iterator ii = m_CoordCmds.begin(); ii != m_CoordCmds.end(); ++ii) {
		resolvedValues.push_back((*ii)->ResolveCoordinate(extents));
	}
	return resolvedValues;
}

VectorCommand::~VectorCommand()
{
	m_CoordCmds.clear();
};

//################################################################################################
//
// CLASS : VectorPart
//
//################################################################################################

VectorPart::VectorPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, long defaultWidth, long defaultHeight):
	ResizablePart				(pPart, eventSink),
	m_bCastShadow				(false),
	m_iShadowThickness			(9),
	m_iShadowDirection			(45),
	m_bGradientFill				(false),
	m_iGradientDirection		(0),
	m_mainPath					(NULL),
	m_shadowPath				(NULL),
	m_bInMainPathDefinition		(false),
	m_bInShadowPathDefinition	(false),
	m_bShadowCasted				(false),
	m_iDefaultWidth				(defaultWidth),
	m_iDefaultHeight			(defaultHeight)
{
	penColorVariableName		= PREF_COLOR;
	brushColorVariableName		= PREF_FILLCOLOR;
	gradientColorVariableName	= PREF_GRADIENTCOLOR;
	shadowColorVariableName		= PREF_SHADOWCOLOR;
}

VectorPart::~VectorPart()
{
	RemoveAllCommands();
	DisposeCoordCommands();
	if (m_mainPath != NULL) {
		delete m_mainPath;
		m_mainPath = NULL;
	}
	if (m_shadowPath != NULL) {
		delete m_shadowPath;
		m_shadowPath = NULL;
	}
}

void VectorPart::RemoveCommand(long index)
{
	ASSERT(index >= 0 && index < GetCommandNumber());
	long i = 0;
	for(std::vector<VectorCommand>::iterator ii = m_Commands.begin(); i < GetCommandNumber() && ii != m_Commands.end(); i++, ++ii) {
		if (i == index) {
			m_Commands.erase(ii);
		}
	}
}

void VectorPart::RemoveLastCommand(long cmdCount)
{
	ASSERT(cmdCount >= 1);
	ASSERT(cmdCount <= GetCommandNumber());
	for (long i = 0; i < cmdCount; i++)
		m_Commands.pop_back();
}

void VectorPart::DisposeCoordCommands(void)
{
	for(unsigned long i = 0; i < m_coordCommands.size(); i++) {
		delete m_coordCommands[i];
	}
	m_coordCommands.clear();
}

VectorCommand VectorPart::GetCommand(long index) const
{
	ASSERT(index >= 0 && index < GetCommandNumber());
	return m_Commands[index];
}

CSize VectorPart::GetPreferredSize(void) const
{
	CSize prefSize = ResizablePart::GetPreferredSize();
	if (prefSize != CSize(0, 0))
		return prefSize;
	return CSize(m_iDefaultWidth, m_iDefaultHeight);
}

void VectorPart::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
{
	if (m_Commands.size() <= 0)
		return;

	COLORREF penColor = m_bActive ? m_crPen : COLOR_GRAYED_OUT;
	m_CurrentPen = DecoratorSDK::getFacilities().GetPen(penColor);

	COLORREF brushColor = m_bActive ? m_crBrush : COLOR_GRAYED_OUT;
	m_CurrentBrush = DecoratorSDK::getFacilities().GetBrush(brushColor);

	for(std::vector<VectorCommand>::iterator ii = m_Commands.begin(); ii != m_Commands.end(); ++ii) {
		long cmdCode = (*ii).GetCode();
		switch(cmdCode) {
			case VectorCommand::DrawLine: {
					CRect rc = (*ii).GetResolvedCoords(m_Rect);
					gdip->DrawLine(m_CurrentPen, rc.left, rc.top, rc.right, rc.bottom);
				}
				break;
			case VectorCommand::DrawRectangle: {
					CRect rc = (*ii).GetResolvedCoords(m_Rect);
					gdip->DrawRectangle(m_CurrentPen, rc.left, rc.top, rc.Width(), rc.Height());
				}
				break;
			case VectorCommand::DrawEllipse: {
					CRect rc = (*ii).GetResolvedCoords(m_Rect);
					gdip->DrawEllipse(m_CurrentPen, rc.left, rc.top, rc.Width(), rc.Height());
				}
				break;
			case VectorCommand::DrawPolygon: {
					std::vector<long> points = (*ii).GetResolvedValues(m_Rect);
					long coordNum = static_cast<long> (points.size() / 2);
					Gdiplus::Point* ppoints = new Gdiplus::Point[coordNum];
					for (long i = 0; i < coordNum; i++)
						ppoints[i] = Gdiplus::Point(points[2 * i], points[2 * i + 1]);
					gdip->DrawPolygon(m_CurrentPen, ppoints, coordNum);
					delete [] ppoints;
				}
				break;
			case VectorCommand::FillRectangle: {
					CRect rc = (*ii).GetResolvedCoords(m_Rect);
					gdip->FillRectangle(m_CurrentBrush, rc.left, rc.top, rc.Width(), rc.Height());
				}
				break;
			case VectorCommand::FillEllipse: {
					CRect rc = (*ii).GetResolvedCoords(m_Rect);
					gdip->FillEllipse(m_CurrentBrush, rc.left, rc.top, rc.Width(), rc.Height());
				}
				break;
			case VectorCommand::FillPolygon: {
					std::vector<long> points = (*ii).GetResolvedValues(m_Rect);
					long coordNum = static_cast<long> (points.size() / 2);
					Gdiplus::Point* ppoints = new Gdiplus::Point[coordNum];
					for (long i = 0; i < coordNum; i++)
						ppoints[i] = Gdiplus::Point(points[2 * i], points[2 * i + 1]);
					gdip->FillPolygon(m_CurrentBrush, ppoints, coordNum);
					delete [] ppoints;
				}
				break;
			case VectorCommand::SelectPen: {
					std::vector<long> colors = (*ii).GetResolvedValues(m_Rect);
					ASSERT(colors.size() == 2);
					COLORREF penColor = m_bActive ? colors[0] : colors[1];
					m_CurrentPen = DecoratorSDK::getFacilities().GetPen(penColor);
				}
				break;
			case VectorCommand::SelectBrush: {
					std::vector<long> colors = (*ii).GetResolvedValues(m_Rect);
					ASSERT(colors.size() == 2);
					COLORREF brushColor = m_bActive ? colors[0] : colors[1];
					m_CurrentBrush = DecoratorSDK::getFacilities().GetBrush(brushColor);
				}
				break;
			case VectorCommand::BeginPath: {
					ASSERT(m_mainPath == NULL);
					if (m_mainPath != NULL)
						delete m_mainPath;
					m_mainPath = new Gdiplus::GraphicsPath();
					m_bInMainPathDefinition = true;
				}
				break;
			case VectorCommand::EndPath: {
					ASSERT(m_mainPath != NULL);
					m_bInMainPathDefinition = false;
				}
				break;
			case VectorCommand::BeginShadowPath:
				{
					ASSERT(m_shadowPath == NULL && m_bInMainPathDefinition == false);
					if (m_shadowPath != NULL)
						delete m_shadowPath;
					m_shadowPath = new Gdiplus::GraphicsPath();
					m_bInShadowPathDefinition = true;
				}
				break;
			case VectorCommand::EndShadowPath:
			case VectorCommand::CopyShadowPath: {
					ASSERT(m_shadowPath != NULL || cmdCode == VectorCommand::CopyShadowPath);
					if (cmdCode == VectorCommand::CopyShadowPath) {
						if (m_shadowPath != NULL)
							delete m_shadowPath;
						m_shadowPath = new Gdiplus::GraphicsPath();
						m_shadowPath->AddPath(m_mainPath, FALSE);
					}
					m_bInShadowPathDefinition = false;
				}
				break;
			case VectorCommand::CastShadowPath: {
					long shadowThickness = m_iShadowThickness;
					std::vector<long> points = (*ii).GetResolvedValues(m_Rect);
					if (points.size() > 0)
						shadowThickness = points[0];
					if (shadowThickness > 0 && m_bCastShadow) {
						if (m_shadowPath == NULL) {
							m_shadowPath = new Gdiplus::GraphicsPath();
							m_shadowPath->AddPath(m_mainPath, FALSE);
						}
						if (m_iShadowDirection < 0 || m_iShadowDirection > 360) {
							// Create a glow
							float horizontalScaleFactor = ((float)m_Rect.Width() + 2 * shadowThickness) / m_Rect.Width();
							float verticalScaleFactor = ((float)m_Rect.Height() + 2 * shadowThickness) / m_Rect.Height();

							Gdiplus::Matrix shadowTranslationMatrix1;
							shadowTranslationMatrix1.Translate(static_cast<float> (-m_Rect.left),
															   static_cast<float> (-m_Rect.top));
							m_shadowPath->Transform(&shadowTranslationMatrix1);
							Gdiplus::Matrix shadowTranslationMatrix2;
							shadowTranslationMatrix2.Scale(horizontalScaleFactor, verticalScaleFactor);
							m_shadowPath->Transform(&shadowTranslationMatrix2);
							Gdiplus::Matrix shadowTranslationMatrix3;
							shadowTranslationMatrix3.Translate(static_cast<float> (m_Rect.left - shadowThickness),
															   static_cast<float> (m_Rect.top - shadowThickness));
							m_shadowPath->Transform(&shadowTranslationMatrix3);
						} else {
							// Create a normal translated shadow
							Gdiplus::Matrix shadowTranslationMatrix;
							long iShadowDirection = m_iShadowDirection - (m_iShadowDirection / 360) * 360;
							shadowTranslationMatrix.Translate(static_cast<float> (cos(DecoratorSDK::getFacilities().Deg2Rad(m_iShadowDirection)) * shadowThickness),
															  static_cast<float> (sin(DecoratorSDK::getFacilities().Deg2Rad(m_iShadowDirection)) * shadowThickness));
							m_shadowPath->Transform(&shadowTranslationMatrix);
						}

						Gdiplus::PathGradientBrush shadowPathGradientBrush(m_shadowPath);
						// Set blend factors and positions for the path gradient brush.
						Gdiplus::Color shadowStartColor = Gdiplus::Color(255,
																		 GetRValue(m_crShadow),
																		 GetGValue(m_crShadow),
																		 GetBValue(m_crShadow));
						Gdiplus::Color shadowBlendedColor = Gdiplus::Color(128,
																		   GetRValue(m_crShadow),
																		   GetGValue(m_crShadow),
																		   GetBValue(m_crShadow));
						Gdiplus::Color shadowEndColor = Gdiplus::Color(0,
																	   GetRValue(m_crShadow),
																	   GetGValue(m_crShadow),
																	   GetBValue(m_crShadow));
						Gdiplus::Color presetColors[] = {
														shadowEndColor,
														shadowBlendedColor,
														shadowStartColor };
						float shadowBorder = static_cast<float> (shadowThickness / (m_Rect.Width() / 2.0));
						if (shadowBorder > 1.0)
							shadowBorder = 1.0;
						float interpolationPositions[] = {
							0.0f,
							shadowBorder,
							1.0f };
						shadowPathGradientBrush.SetInterpolationColors(presetColors, interpolationPositions, 3);

						gdip->FillPath(&shadowPathGradientBrush, m_shadowPath);
	
						m_bShadowCasted = true;
					}
				}
				break;
			case VectorCommand::FillPath:
			case VectorCommand::StrokePath:
			case VectorCommand::StrokeAndFillPath: {
					if (cmdCode == VectorCommand::FillPath || cmdCode == VectorCommand::StrokeAndFillPath) {
						std::vector<long> colors = (*ii).GetResolvedValues(m_Rect);
						ASSERT(colors.size() == 0 || colors.size() == 2 || colors.size() == 4);
						if (m_bGradientFill || m_bShadowCasted || colors.size() == 2) {
							if (m_bGradientFill) {
								int gradVectorEndX = 0;
								int gradVectorEndY = 0;
								if (m_iGradientDirection == 90) {
									gradVectorEndX = m_Rect.left;
									gradVectorEndY = m_Rect.bottom;
								} else {
									long gradientAngle = abs(m_iGradientDirection);
									gradientAngle = gradientAngle - (gradientAngle / 90) * 90;
									if (gradientAngle == 0) {
										gradVectorEndX = m_Rect.right;
										gradVectorEndY = m_Rect.top;
									} else {
										double beta = atan((double)m_Rect.Height() / m_Rect.Width());
										double alfa = DecoratorSDK::getFacilities().Deg2Rad(gradientAngle);
										double d = sqrt((double)m_Rect.Width() * m_Rect.Width() + m_Rect.Height() * m_Rect.Height());
										double r = d * cos(alfa < beta ? beta - alfa : alfa - beta);
										double x = r * cos(alfa);
										double y = r * sin(alfa);
										gradVectorEndX = static_cast<int> (x) + m_Rect.left + 1;
										gradVectorEndY = static_cast<int> (y) + m_Rect.top + 1;
//										TRACE("(l %ld, t %ld, w %ld, h %ld) a %lf b %lf x %lf y %lf xf %ld yf %ld\n",
//											  m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height(),
//											  (double)gradientAngle, beta / 2 / M_PI * 360.0, x, y,
//											  gradVectorEndX, gradVectorEndY);
									}
								}
								Gdiplus::Point gradVectorEnd(gradVectorEndX, gradVectorEndY);
								Gdiplus::LinearGradientBrush linearGradientBrush(
									Gdiplus::Point(m_Rect.left, m_Rect.top),
									gradVectorEnd,
									Gdiplus::Color(GetRValue(m_crBrush),
												   GetGValue(m_crBrush),
												   GetBValue(m_crBrush)),
									Gdiplus::Color(GetRValue(m_crGradient),
												   GetGValue(m_crGradient),
												   GetBValue(m_crGradient)));
								gdip->FillPath(&linearGradientBrush, m_mainPath);
							} else {
								Gdiplus::SolidBrush* fillBrush = NULL;
								if (colors.size() == 2)
									fillBrush = DecoratorSDK::getFacilities().GetBrush((COLORREF)(m_bActive ? colors[0] : colors[1]));
								else
									fillBrush = DecoratorSDK::getFacilities().GetBrush(m_crBrush);
								gdip->FillPath(fillBrush, m_mainPath);
							}
						}
					}
					if (cmdCode == VectorCommand::StrokePath || cmdCode == VectorCommand::StrokeAndFillPath) {
						COLORREF borderColor = m_bActive ? m_crPen : COLOR_GRAYED_OUT;
						Gdiplus::Pen* strokePen = DecoratorSDK::getFacilities().GetPen(borderColor);
						gdip->DrawPath(strokePen, m_mainPath);
					}
				}
				break;
			case VectorCommand::AddLineToPath: {
					CRect rc = (*ii).GetResolvedCoords(m_Rect);
					if (m_bInMainPathDefinition)
						m_mainPath->AddLine(rc.left, rc.top, rc.right, rc.bottom);
					if (m_bInShadowPathDefinition)
						m_shadowPath->AddLine(rc.left, rc.top, rc.right, rc.bottom);
				}
				break;
			case VectorCommand::AddEllipseToPath: {
					CRect rc = (*ii).GetResolvedCoords(m_Rect);
					if (m_bInMainPathDefinition)
						m_mainPath->AddEllipse(rc.left, rc.top, rc.Width(), rc.Height());
					if (m_bInShadowPathDefinition)
						m_shadowPath->AddEllipse(rc.left, rc.top, rc.Width(), rc.Height());
				}
				break;
			case VectorCommand::AddRectangleToPath:
			case VectorCommand::AddRoundRectangleToPath: {
					CRect rc = (*ii).GetResolvedCoords(m_Rect);
					std::vector<long> points = (*ii).GetResolvedValues(m_Rect);
					long radius = points[4];
					if (cmdCode == VectorCommand::AddRectangleToPath || radius == 0) {
						Gdiplus::Rect gdipRect(rc.left, rc.top, rc.Width(), rc.Height());
						if (m_bInMainPathDefinition)
							m_mainPath->AddRectangle(gdipRect);
						if (m_bInShadowPathDefinition)
							m_shadowPath->AddRectangle(gdipRect);
					} else {
						long diameter = 2 * radius;
						if (m_bInMainPathDefinition) {
							m_mainPath->AddArc(rc.left, rc.top, diameter, diameter, 180.0, 90.0);
							m_mainPath->AddLine(rc.left + radius, rc.top, rc.right - radius, rc.top);
							m_mainPath->AddArc(rc.right - diameter, rc.top, diameter, diameter, 270.0, 90.0);
							m_mainPath->AddLine(rc.right, rc.top + radius, rc.right, rc.bottom - radius);
							m_mainPath->AddArc(rc.right - diameter, rc.bottom - diameter, diameter, diameter, 0.0, 90.0);
							m_mainPath->AddLine(rc.right - radius, rc.bottom, rc.left + radius, rc.bottom);
							m_mainPath->AddArc(rc.left, rc.bottom - diameter, diameter, diameter, 90.0, 90.0);
							m_mainPath->AddLine(rc.left, rc.bottom - radius, rc.left, rc.top + radius);
						} else if (m_bInShadowPathDefinition) {
							m_shadowPath->AddArc(rc.left, rc.top, diameter, diameter, 180.0, 90.0);
							m_shadowPath->AddLine(rc.left + radius, rc.top, rc.right - radius, rc.top);
							m_shadowPath->AddArc(rc.right - diameter, rc.top, diameter, diameter, 270.0, 90.0);
							m_shadowPath->AddLine(rc.right, rc.top + radius, rc.right, rc.bottom - radius);
							m_shadowPath->AddArc(rc.right - diameter, rc.bottom - diameter, diameter, diameter, 0.0, 90.0);
							m_shadowPath->AddLine(rc.right - radius, rc.bottom, rc.left + radius, rc.bottom);
							m_shadowPath->AddArc(rc.left, rc.bottom - diameter, diameter, diameter, 90.0, 90.0);
							m_shadowPath->AddLine(rc.left, rc.bottom - radius, rc.left, rc.top + radius);
						}
					}
				}
				break;
			case VectorCommand::AddPolygonToPath: {
					std::vector<long> points = (*ii).GetResolvedValues(m_Rect);
					long coordNum = static_cast<long> (points.size() / 2);
					Gdiplus::Point* ppoints = new Gdiplus::Point[coordNum];
					for (long i = 0; i < coordNum; i++)
						ppoints[i] = Gdiplus::Point(points[2 * i], points[2 * i + 1]);
					if (m_bInMainPathDefinition)
						m_mainPath->AddPolygon(ppoints, coordNum);
					if (m_bInShadowPathDefinition)
						m_shadowPath->AddPolygon(ppoints, coordNum);
					delete [] ppoints;
				}
				break;
			case VectorCommand::AddArcToPath: {
					std::vector<long> values = (*ii).GetResolvedValues(m_Rect);
					ASSERT(values.size() == 6);
					if (m_bInMainPathDefinition)
						m_mainPath->AddArc(values[0], values[1], values[2], values[3], (float)values[4], (float)values[5]);
					if (m_bInShadowPathDefinition)
						m_shadowPath->AddArc(values[0], values[1], values[2], values[3], (float)values[4], (float)values[5]);
				}
				break;
			default: ASSERT(false);
		}
	}
	if (m_mainPath != NULL) {
		delete m_mainPath;
		m_mainPath = NULL;
	}
	if (m_shadowPath != NULL) {
		delete m_shadowPath;
		m_shadowPath = NULL;
	}

	ResizablePart::Draw(pDC, gdip);
}

// New functions
void VectorPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
							  HWND parentWnd, PreferenceMap& preferences)
{
	DecoratorSDK::getFacilities().getMetaFCO(pPart, m_spMetaFCO);

	ResizablePart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);

	// Pen's Color
	m_crPen = COLOR_BLACK;
	PreferenceMap::iterator it = preferences.find(penColorVariableName);
	if (it != preferences.end()) {
		m_crPen = it->second.uValue.crValue;
	} else {
		getFacilities().getPreference(m_spFCO, m_spMetaFCO, penColorVariableName, m_crPen);
	}

	// Brush's Color
	m_crBrush = COLOR_WHITE;
	it = preferences.find(brushColorVariableName);
	if (it != preferences.end()) {
		m_crBrush = it->second.uValue.crValue;
	} else {
		getFacilities().getPreference(m_spFCO, m_spMetaFCO, brushColorVariableName, m_crBrush);
	}

	m_bCastShadow = false;
	it = preferences.find(PREF_ITEMSHADOWCAST);
	if (it != preferences.end()) {
		m_bCastShadow = it->second.uValue.bValue;
	} else {
		getFacilities().getPreference(m_spFCO, m_spMetaFCO, PREF_ITEMSHADOWCAST, m_bCastShadow);
	}

	m_crShadow = COLOR_GREY;
	it = preferences.find(shadowColorVariableName);
	if (it != preferences.end()) {
		m_crShadow = it->second.uValue.crValue;
	} else {
		getFacilities().getPreference(m_spFCO, m_spMetaFCO, shadowColorVariableName, m_crShadow);
	}

	m_iShadowThickness = 9;
	it = preferences.find(PREF_SHADOWTHICKNESS);
	if (it != preferences.end()) {
		m_iShadowThickness = it->second.uValue.lValue;
	} else {
		getFacilities().getPreference(m_spFCO, m_spMetaFCO, PREF_SHADOWTHICKNESS, m_iShadowThickness, false);
	}

	m_iShadowDirection = 45;
	it = preferences.find(PREF_SHADOWDIRECTION);
	if (it != preferences.end()) {
		m_iShadowDirection = it->second.uValue.lValue;
	} else {
		getFacilities().getPreference(m_spFCO, m_spMetaFCO, PREF_SHADOWDIRECTION, m_iShadowDirection, false);
	}

	m_bGradientFill = false;
	it = preferences.find(PREF_ITEMGRADIENTFILL);
	if (it != preferences.end()) {
		m_bGradientFill = it->second.uValue.bValue;
	} else {
		getFacilities().getPreference(m_spFCO, m_spMetaFCO, PREF_ITEMGRADIENTFILL, m_bGradientFill);
	}

	m_crGradient = COLOR_WHITE;
	it = preferences.find(gradientColorVariableName);
	if (it != preferences.end()) {
		m_crGradient = it->second.uValue.crValue;
	} else {
		getFacilities().getPreference(m_spFCO, m_spMetaFCO, gradientColorVariableName, m_crGradient);
	}

	m_iGradientDirection = 0;
	it = preferences.find(PREF_GRADIENTDIRECTION);
	if (it != preferences.end()) {
		m_iGradientDirection = it->second.uValue.lValue;
	} else {
		getFacilities().getPreference(m_spFCO, m_spMetaFCO, PREF_GRADIENTDIRECTION, m_iGradientDirection, false);
	}
}

}; // namespace DecoratorSDK
