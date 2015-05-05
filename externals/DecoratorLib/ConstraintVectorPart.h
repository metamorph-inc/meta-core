//################################################################################################
//
// Constraint vector part decorator class
//	ConstraintVectorPart.h
//
//################################################################################################

#ifndef __CONSTRAINTVECTORPART_H_
#define __CONSTRAINTVECTORPART_H_


#include "StdAfx.h"
#include "VectorPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ConstraintVectorPart
//
//################################################################################################

class ConstraintVectorPart: public VectorPart
{
protected:
	COLORREF				m_crFill;
	double					m_thicknessRatio;

public:
	ConstraintVectorPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, COLORREF fillColor,
						 double thicknessRatio, long constraintWidth, long constraintHeight);
	virtual ~ConstraintVectorPart();

// =============== resembles IMgaElementDecorator
public:
	virtual void			InitializeEx		(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);
};

}; // namespace DecoratorSDK

#endif //__CONSTRAINTVECTORPART_H_
