#ifndef RAWCOMPONENT_H
#define RAWCOMPONENT_H

#include <string>
#include <set>
#include <vector>
// Declaration of the main RAW COM component interface class


#ifdef BUILDER_OBJECT_NETWORK
#error   This file should only be included in the RAW COM configurations
#endif



class RawComponent {
////////////////////
// Insert your application specific member and method definitions here
public:
	RawComponent() { ; }
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
private:
	std::set<_bstr_t> numericIdfcoKinds; ///< Kinds that we assign the int ID attribute
	std::set<_bstr_t> guidIdfcoKinds; ///< Kinds that we assign the GUID ID attribute
	std::set<int> ids;
	std::vector<CComPtr<IMgaFCO>> fcolist;
	int maxId;
	void createStringSetForMeta_id();
	CString getFCOMetaName(IMgaFCO *fco);
	CString getObjectMetaDisplayedName(IMgaObject *obj);
	void traverseFolder(IMgaFolder *fdr);
	void traverseFCO(IMgaFCO *fco, _bstr_t &metaName);
	bool isDesignElement(IMgaFCO *fco);
	void makeDirtyConfigurations_by_constraint(IMgaFCO *fco);
	void makeDirtyConfigurations_by_component(IMgaFCO *fco);
};


#endif //RAWCOMPONENT_H