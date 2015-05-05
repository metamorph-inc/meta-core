//################################################################################################
//
// Inheritance vector part decorator class
//	InheritanceVectorPart.h
//
//################################################################################################

#ifndef __INHERITANCEVECTORPART_H_
#define __INHERITANCEVECTORPART_H_


#include "StdAfx.h"
#include "TriangleVectorPart.h"


namespace DecoratorSDK {

enum InheritanceType {
	NormalInheritance,
	ImplementationInheritance,
	InterfaceInheritance
};

//################################################################################################
//
// CLASS : InheritanceVectorPart
//
//################################################################################################

class InheritanceVectorPart: public TriangleVectorPart
{
protected:
	InheritanceType		m_inheritanceType;

public:
	InheritanceVectorPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink,
						  long inheritanceWidth, long inheritanceHeight, InheritanceType inheritanceType);
	virtual ~InheritanceVectorPart();

// =============== resembles IMgaElementDecorator
public:
	virtual void			InitializeEx		(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);
};

}; // namespace DecoratorSDK

#endif //__INHERITANCEVECTORPART_H_
