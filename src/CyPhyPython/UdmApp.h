// UdmApp.h: interface for the CUdmApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UDMAPP_H__296A93EB_6DB6_4179_AA0B_A3D799FB3A1C__INCLUDED_)
#define AFX_UDMAPP_H__296A93EB_6DB6_4179_AA0B_A3D799FB3A1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Udm includes
#include "UdmBase.h"
#include "Uml.h"
#include "UmlExt.h"

#ifdef _USE_DOM
	#include "UdmDOM.h"
#endif

#include "UdmGme.h"
#include "UdmStatic.h"


struct python_error : public std::runtime_error
{
	explicit python_error(const string& _Message)
		: std::runtime_error(_Message) {}
};

class CUdmApp  
{

public:
	std::string meta_path;
	void UdmMain(Udm::DataNetwork* p_backend, Udm::Object currentObject, std::set<Udm::Object> selectedObjects, long param, map<_bstr_t, _variant_t>& componentParameters, std::string workingDir);
};

#endif // !defined(AFX_UDMAPP_H__296A93EB_6DB6_4179_AA0B_A3D799FB3A1C__INCLUDED_)
