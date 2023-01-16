#pragma once

#include "Mga.h"
#include <exception>
#include <string>
#include <set>
#include <map>
#include "..\bin\Python310\Include\Python.h"

std::wstring GetMetaPath();
HMODULE LoadPythonDll(const std::wstring& metapath);


struct python_error : public std::runtime_error
{
	explicit python_error(const std::string& _Message)
		: std::runtime_error(_Message) {}
	explicit python_error(const std::wstring& _Message)
		: std::runtime_error(static_cast<const char*>(_bstr_t(_Message.c_str()))) {}
};

struct UnGil {
	PyGILState_STATE state;
	UnGil(PyGILState_STATE state) : state(state) {}
	~UnGil() {
		PyGILState_Release(state);
	}
};

struct RAIIFreeLibrary
{
	HMODULE module;
	RAIIFreeLibrary(HMODULE module) : module(module) {
	}

	~RAIIFreeLibrary()
	{
		if (module != INVALID_HANDLE_VALUE) {
			::FreeLibrary(module);
		}
	}
};

void Main(const std::wstring& meta_path, CComPtr<IMgaProject> project, CComPtr<IMgaObject> focusObject,
	std::set<CComPtr<IMgaFCO> > selectedObjects,
	long param, std::map<_bstr_t, _variant_t>& componentParameters, std::wstring workingDir);
