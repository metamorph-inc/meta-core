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

void FormulaEvaluator(const Udm::Object &focusObject) {
	Traverse(focusObject);
};
