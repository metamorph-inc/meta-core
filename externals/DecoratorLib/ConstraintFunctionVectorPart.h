//################################################################################################
//
// Constraint Function vector part decorator class
//	ConstraintFunctionVectorPart.h
//
//################################################################################################

#ifndef __CONSTRAINTFUNCTIONVECTORPART_H_
#define __CONSTRAINTFUNCTIONVECTORPART_H_


#include "StdAfx.h"
#include "ConstraintVectorPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ConstraintFunctionVectorPart
//
//################################################################################################

class ConstraintFunctionVectorPart: public ConstraintVectorPart
{
public:
	ConstraintFunctionVectorPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, COLORREF fillColor,
								 double thicknessRatio, long constraintWidth, long constraintHeight);
	virtual ~ConstraintFunctionVectorPart();

// =============== resembles IMgaElementDecorator
public:
	virtual void			InitializeEx		(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);
};

}; // namespace DecoratorSDK

#endif //__CONSTRAINTFUNCTIONVECTORPART_H_
