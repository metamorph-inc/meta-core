#pragma once

#include "CyPhyElaborate.h"

/*******************
Toolbox use cases
	User runs FormulaEvaluator
		Expand, evaluate, collapse
	Other interpreter runs FormulaEvaluator
		evaluate
		assume master knows what he's doing, and that we are elaborated
	(User | Interpreter) runs Elaborator to expand
		Expand, evaluate
	(User | Interpreter) runs Elaborator to collapse
		just collapse
		we do this because we don't know if the model 
		  is fully elaborated and "safe" to evaluate
***********************/

void FormulaEvaluator(const Udm::Object &focusObject,bool elaborateAndCollapse) {
	if (elaborateAndCollapse) {
		CyPhyElaborate cpe;
		Uml::Class type = focusObject.type();
		if (Uml::IsDerivedFrom(type, CyPhyML::TestBenchType::meta)) {
			TestBenchType tb_ = TestBenchType::Cast(focusObject);
			TestBenchType tb;

			try {
				// Elaborate includes traverse.
				tb = cpe.elaborate(tb_);
			} catch (udm_exception &exc) {
				string what = exc.what();
				
				// Don't rethrow ValueFlow errors
				if (what.substr(0,16) != "ValueFlow error:")
					throw exc;
				GMEConsole::Console::writeLine("Udm exception: " + what, MSG_ERROR);
			} 
		} else if (type == CyPhyML::ComponentAssembly::meta) {
			ComponentAssembly ca_ = ComponentAssembly::Cast(focusObject);
			ComponentAssembly ca;

			try {
				// Elaborate includes traverse
				ca = cpe.elaborate(ca_);
			} catch (udm_exception &exc) {
				string what = exc.what();
				
				// Don't rethrow ValueFlow errors
				if (what.substr(0,16) != "ValueFlow error:")
					throw exc;
				GMEConsole::Console::writeLine("Udm exception: " + what, MSG_ERROR);
			}
		} else if (Uml::IsDerivedFrom(type,CyPhyML::ComponentType::meta)) {  // ZL 11/20/2013 test component support
			Traverse(focusObject);
		} else if (type == CyPhyML::TestBenchSuite::meta) {
			Traverse(focusObject);
		}
	} else
		Traverse(focusObject);
};

void FormulaEvaluator(const std::set<Udm::Object> &selectedObjects,bool elaborateAndCollapse) {
	for (set<Udm::Object>::const_iterator i = selectedObjects.begin(); i != selectedObjects.end(); i++) {
		Udm::Object oi(*i);
		FormulaEvaluator(oi,elaborateAndCollapse);
	}
};
