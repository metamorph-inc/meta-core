#ifndef DESERT_FORMULA_UTIL_H
#define DESERT_FORMULA_UTIL_H

#include "CyPhyML.h"
#include <string>
#include <map>

namespace DFUtil
{
	using namespace std;
	bool IsValidFormula(CyPhyML::CustomFormula& formula);
	bool IsValidExpression(string& expression);
	string ConvertToDesertFormat(CyPhyML::CustomFormula& formula);
	string ConvertExpression(string& expression, const map<string, string>& VariablePathMap);
	string getRelativePath(Udm::Object &obj, const CyPhyML::DesignEntity &cyphy_elem);
	string getRelativePath(Udm::Object &obj, Udm::Object &obj_ref_parent, const CyPhyML::DesignEntity &cyphy_elem);
	void ReplaceToDesertName(string& token, const map<string, string>& VariablePathMap);
	void CheckExpression(string& expression);

	bool ConvertCaret(std::string& expression);
};

#endif