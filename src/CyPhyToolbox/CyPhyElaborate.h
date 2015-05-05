/*******************************************************
CyPhyElaborate
Convert a ComponentAssembly using ComponentRef objects to one using Component instances, and vice versa

Typical Usage:
1) Create an instance of the CyPhyElaborate class.
2) Pass the ComponentAssembly you wish to parse to CyPhyElaborate::expand, and parse the returned ComponentAssembly instead.
		Note: Currently, the returned ComponentAssembly will be the same, but this may not be true in the future.
3) When you are finished parsing the ComponentAssembly, call CyPhyElaborate::release to convert it back.
   Changes that you've made to non-Component elements (such as Parameters and Properties of ComponentAssemblies) will persist.

Notes:
- If your component is read-only, I recommend starting a transaction, calling CyPhyElaborate::expand,
  parsing the model, and the aborting the transaction. This will prevent you from having to use the
  CyPhyElaborate::collapse function and will save time.
- You can use CyPhyElaborate::toggleGMEOverhead to toggle "Nested Transactions" and the GME Constraint Manager.
  Turning these off will dramatically speed up components that do a lot of model manipulation.
*******************************************************/

#pragma once
#include "CyPhyML.h"
#include "Mga.h"
#include "UdmGme.h"
#include <atlbase.h>
#include <atlcom.h>
#include "GMECOM.H"
#include "FormulaTraverse.h"
#include <map>
#include <algorithm>

//#define LOGGER

#ifdef LOGGER
	#include <iostream>
	#include <fstream>
#endif

using namespace CyPhyML;

class CyPhyElaborate {
	public:
		std::map<Udm::Object, Udm::Object> originalObjectToCopies;

		CyPhyElaborate() : copiedObjectsToOriginals(new std::map<Udm::Object, Udm::Object>()) {
			overheadstate = true;
		};
		~CyPhyElaborate() {};

		ComponentAssembly elaborate(const ComponentAssembly& ca) {
			toggleGMEOverhead(false,ca);

			ComponentAssembly rtn = expand(ca);
			
			try {
				Traverse(rtn);
			} catch (udm_exception &exc) {
				string what = exc.what();
				udm_exception new_exc("ValueFlow error: " + what);
				throw new_exc;
			}
			
			toggleGMEOverhead(true,ca);			
			tellCyPhyAddOnToIgnoreNextTransaction(ca);
			return rtn;
		}
		TestBenchType elaborate(const TestBenchType& tb) {
			toggleGMEOverhead(false,tb);

			TestBenchType rtn = expand(tb);
			
			try {
				Traverse(rtn);
			} catch (udm_exception &exc) {
				string what = exc.what();
				udm_exception new_exc("ValueFlow error: " + what);
				throw new_exc;
			}

			toggleGMEOverhead(true,tb);
			tellCyPhyAddOnToIgnoreNextTransaction(tb);

			return rtn;
		};

private: // KMS: collapse is broken
		void collapse(const ComponentAssembly& ca) {
			try {
				collapse_recursive(ca);
			} catch (udm_exception &exc) {
				string what = exc.what();
				udm_exception new_exc("Collapse error: " + what);
				throw new_exc;
			}
			tellCyPhyAddOnToIgnoreNextTransaction(ca);
		};
		void collapse(const TestBenchType& tb) {
			try {
				collapse_recursive(tb);
			} catch (udm_exception &exc) {
				string what = exc.what();
				udm_exception new_exc("Collapse error: " + what);
				throw new_exc;
			}
			tellCyPhyAddOnToIgnoreNextTransaction(tb);
		};

		CyPhyML::MgaObject Retrieve(
			CyPhyML::MgaObject testBench);

	private:

		void collapse_recursive(const ComponentAssembly& ca);
		void collapse_recursive(const TestBenchType& tb);

		struct gceResult {
			Udm::Object srcParRef;
			DesignElement srcParComp;
			Udm::Object srcObj;
			Udm::Object dstParRef;
			DesignElement dstParComp;
			Udm::Object dstObj;
		};
		
		map<TestBenchType,DesignEntity> tbToOriginalSUT;

		void addAllObjectsToTraceability(Udm::Object start);

		ComponentAssembly expand(const ComponentAssembly&);
		ComponentAssembly expand(const ComponentAssembly& ca, string ID_Prefix, std::set<Udm::Object> expandedInThisStack);
		TestBenchType expand(const TestBenchType&);
		template<typename CHILD_CLASS>
		void SwitchReference(CyPhyML::TestBenchType tb);

		typedef map<string, pair<string, string>> PropertyRegistryMap;								// DY: 9/9/11
		void copyCADPropertyRegistry2Inst(CyPhyML::ComponentRef& src, CyPhyML::Component& dst);			// DY: 9/9/11
		void copyCADPropertyRegistry2Ref(CyPhyML::Component& src, CyPhyML::ComponentRef& dst);		// DY: 9/9/11
		void copyCADRegistryValues(CComPtr<IMgaFCO>& src, CComPtr<IMgaFCO>& dst, PropertyRegistryMap&);	// DY: 9/9/11

		// Overhead state
		long original_prefmask;
		bool overheadstate;
		bool cyPhyAddOnState;

		void log(string fn,string path) {
			#ifdef LOGGER
				ofstream out("c:\\log.txt",ios_base::app);
				char entry[1024];
				sprintf(entry,"%s : %s\n",fn.c_str(),path.c_str());
				out << entry;
				out.close();
			#endif
		};

		// Tell CyPhyAddOn to skip ID assignment on the next transaction.
		void tellCyPhyAddOnToIgnoreNextTransaction(Udm::Object anyArbitraryObject)
		{
			// this will throw an exception if anyArbitraryObject is Udm::null
			anyArbitraryObject.isLibObject();
			CComPtr<IUnknown> object;
			object.Attach(UdmGme::Udm2Gme(anyArbitraryObject));
			CComQIPtr<IMgaObject> mgaObject = object;
			CComPtr<IMgaProject> project;
			COMTHROW(mgaObject->get_Project(&project));
			
			CComPtr<IMgaComponentEx> cyPhyAddOn;
			// Finding constraint manager among the addons
			CComPtr<IMgaComponents> comps;
			COMTHROW( project->get_AddOnComponents(&comps));
			MGACOLL_ITERATE(IMgaComponent, comps) 
			{
				CComBSTR name;
				COMTHROW(MGACOLL_ITER->get_ComponentName(&name));
				if(name == L"CyPhyAddOn") 
				{
					cyPhyAddOn = CComQIPtr<IMgaComponentEx>(MGACOLL_ITER); 
				}
			} MGACOLL_ITERATE_END;
			if (cyPhyAddOn)
			{
				COMTHROW( cyPhyAddOn->put_ComponentParameter(L"dontassignguidsonnexttransaction", CComVariant(true)) );
			}
		}

	public:		

		void toggleGMEOverhead(bool state,Udm::Object anyArbitraryObject) {	
			if (state == overheadstate)
				return;
			
			// this will throw an exception if anyArbitraryObject is Udm::null
			anyArbitraryObject.isLibObject();
			CComPtr<IUnknown> object;
			object.Attach(UdmGme::Udm2Gme(anyArbitraryObject));
			CComQIPtr<IMgaObject> mgaObject = object;
			CComPtr<IMgaProject> project;
			COMTHROW(mgaObject->get_Project(&project));
			
			CComPtr<IMgaComponentEx> constrMgr;
			// Finding constraint manager among the addons
			CComPtr<IMgaComponents> comps;
			COMTHROW( project->get_AddOnComponents(&comps));
			MGACOLL_ITERATE(IMgaComponent, comps) 
			{
				CComBSTR name;
				COMTHROW(MGACOLL_ITER->get_ComponentName(&name));
				if(name == L"ConstraintManager") 
				{
					constrMgr = CComQIPtr<IMgaComponentEx>(MGACOLL_ITER); 
				}
			} MGACOLL_ITERATE_END;
			if (constrMgr)
				COMTHROW(constrMgr->Enable(state));
			
			if (!state) {
				long prefmask;
				COMTHROW(project->get_Preferences(&prefmask));
				original_prefmask = prefmask;
				// KMS magic number MGAPREF_NO_NESTED_TX
				#define MGAPREF_NO_NESTED_TX 0x80
				prefmask |= MGAPREF_NO_NESTED_TX; 
				COMTHROW(project->put_Preferences(prefmask));
			} else
				COMTHROW(project->put_Preferences(original_prefmask));
			
			overheadstate = state;
		};

		set<Udm::Object> cr_null_ref_set;
		void addCopy(Udm::Object copy, Udm::Object original)
		{
			//if (copy == original)
			//{
			//	OutputDebugStringA((copy.getPath("/") + "    " + original.getPath("/") + "\n").c_str());
			//}
			//else
			//{
			//	OutputDebugStringA((copy.getPath("/") + "    " + original.getPath("/") + "\n").c_str());
			//}
			auto ent = copiedObjectsToOriginals->find(copy);
			if (ent == copiedObjectsToOriginals->end())
			{
				(*copiedObjectsToOriginals)[copy] = original;
			}
			else
			{
				(*copiedObjectsToOriginals)[copy] = ent->second;
				//copiedObjectsToOriginals.erase(ent);
			}
			originalObjectToCopies[original] = copy;
			// OutputDebugStringA((copy.getPath("/") + "    " + original.getPath("/") + "\n").c_str());
		}
		void addCopies(std::map<Udm::Object, Udm::Object>& cam)
		{
			std::for_each(cam.begin(), cam.end(), [this](std::map<Udm::Object, Udm::Object>::value_type& it) { addCopy(it.second, it.first); });
			//for(auto it = cam.begin(); it != cam.end(); ++it)
			//{
			//	addCopy(it->second, it->first);
			//}
		}
public:
		std::unique_ptr<std::map<Udm::Object, Udm::Object> > copiedObjectsToOriginals;

};
