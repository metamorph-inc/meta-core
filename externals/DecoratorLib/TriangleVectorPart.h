//################################################################################################
//
// Triangle vector part decorator class
//	TriangleVectorPart.h
//
//################################################################################################

#ifndef __TRIANGLEVECTORPART_H_
#define __TRIANGLEVECTORPART_H_


#include "StdAfx.h"
#include "VectorPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : TriangleVectorPart
//
//################################################################################################

class TriangleVectorPart: public VectorPart
{
public:
	TriangleVectorPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, long triangleWidth, long triangleHeight);
	virtual ~TriangleVectorPart();

// =============== resembles IMgaElementDecorator
public:
	virtual void			InitializeEx		(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);
};

}; // namespace DecoratorSDK

#endif //__TRIANGLEVECTORPART_H_
