#pragma once

#include "Mga.h"
#include <exception>
#include <string>
#include <set>
#include <map>

struct python_error : public std::runtime_error
{
	explicit python_error(const std::string& _Message)
		: std::runtime_error(_Message) {}
};

void Main(const std::string& meta_path, CComPtr<IMgaProject> project, CComPtr<IMgaObject> focusObject,
	std::set<CComPtr<IMgaFCO> > selectedObjects,
	long param, std::map<_bstr_t, _variant_t>& componentParameters, std::string workingDir);
