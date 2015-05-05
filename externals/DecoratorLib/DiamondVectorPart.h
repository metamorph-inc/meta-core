//################################################################################################
//
// Diamond vector part decorator class
//	DiamondVectorPart.h
//
//################################################################################################

#ifndef __DIAMONDVECTORPART_H_
#define __DIAMONDVECTORPART_H_


#include "StdAfx.h"
#include "VectorPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : DiamondVectorPart
//
//################################################################################################

class DiamondVectorPart: public VectorPart
{
public:
	DiamondVectorPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, long diamondWidth, long diamondHeight);
	virtual ~DiamondVectorPart();

// =============== resembles IMgaElementDecorator
public:
	virtual void			InitializeEx		(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);
};

}; // namespace DecoratorSDK

#endif //__DIAMONDVECTORPART_H_
