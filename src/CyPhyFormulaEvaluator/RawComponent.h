#ifndef RAWCOMPONENT_H
#define RAWCOMPONENT_H

#include "Mga.h"


// Declaration of the main RAW COM component interface class


#ifdef BUILDER_OBJECT_NETWORK
#error   This file should only be included in the RAW COM configurations
#endif

#include <comutil.h>
#include <map>
#include <string>

#include "atlsafe.h"

class RawComponent {
////////////////////
// Insert your application specific member and method definitions here
public:
	std::map<std::string, _variant_t> componentParameters;
	std::string delimiter;

	void UpdateParameters();

	RawComponent()
	{
		delimiter = ";\n";
		std::string Value = "";
		// initialize component parameters
		componentParameters.clear();
		//componentParameters["key"]            = _variant_t("value");
		componentParameters["name"]             = _variant_t("Formula evaluator interpreter");

		Value.erase();
		Value += "Calculates all formulas and assign calculated values to Parameter/Property/Metric, etc.\n";
		componentParameters["description"]      = _variant_t(Value.c_str());

		componentParameters["help"]             = _variant_t("Help string message. Usage: TBA...");
		componentParameters["console_messages"] = _variant_t("on");
		componentParameters["output_dir"]       = _variant_t("");
		componentParameters["automation"]       = _variant_t("false");
		componentParameters["expanded"]         = _variant_t("false");
		componentParameters["do_not_generate_post_processing"] = _variant_t("false");
		componentParameters["runCommand"]       = _variant_t("main_post_process.py testbench_manifest.json");  // JS modified to allow batch from Master T/Job Processor
		componentParameters["labels"]       = _variant_t(L""); // empty string means use default label
		componentParameters["original_project_file"]	= _variant_t("");
		componentParameters["results_zip_py"]	= _variant_t("");

		CComSafeArray<BSTR> safe;
		// list of all supported kinds
		safe.Add(CComBSTR(L"TestBench"));
		safe.Add(CComBSTR(L"ComponentAssembly"));
		safe.Add(CComBSTR(L"Component"));

		_variant_t pVal("");
		CComVariant(safe).Detach(&pVal);
		componentParameters["supports"]         = _variant_t(pVal);
	}
private:
	
	
// Try not to modify the code below this line
////////////////////
public:	
#ifdef GME_ADDON
	CComPtr<IMgaProject> project;  // this is set before Initialize() is called
	CComPtr<IMgaAddOn> addon;      // this is set before Initialize() is called
#endif
	bool interactive;
	
	STDMETHODIMP Initialize(struct IMgaProject *);
	STDMETHODIMP Invoke(IMgaProject* gme, IMgaFCOs *models, long param);
	STDMETHODIMP InvokeEx( IMgaProject *project,  IMgaFCO *currentobj,  IMgaFCOs *selectedobjs,  long param);
	STDMETHODIMP ObjectsInvokeEx( IMgaProject *project,  IMgaObject *currentobj,  IMgaObjects *selectedobjs,  long param);
	STDMETHODIMP get_ComponentParameter(BSTR name, VARIANT *pVal);
	STDMETHODIMP put_ComponentParameter(BSTR name, VARIANT newVal);

#ifdef GME_ADDON
	STDMETHODIMP GlobalEvent(globalevent_enum event);
	STDMETHODIMP ObjectEvent(IMgaObject * obj, unsigned long eventmask, VARIANT v);
#endif
};


#endif //RAWCOMPONENT_H