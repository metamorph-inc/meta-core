//################################################################################################
//
// Ellipse vector part decorator class
//	EllipseVectorPart.h
//
//################################################################################################

#ifndef __ELLIPSEVECTORPART_H_
#define __ELLIPSEVECTORPART_H_


#include "StdAfx.h"
#include "VectorPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : EllipseVectorPart
//
//################################################################################################

class EllipseVectorPart: public VectorPart
{
public:
	EllipseVectorPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, long ellipseWidth, long ellipseHeight);
	virtual ~EllipseVectorPart();

// =============== resembles IMgaElementDecorator
public:
	virtual void			InitializeEx		(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);
};

}; // namespace DecoratorSDK

#endif //__ELLIPSEVECTORPART_H_
