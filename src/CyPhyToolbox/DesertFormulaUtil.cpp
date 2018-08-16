#include "DesertFormulaUtil.h"
#include "UdmUtil.h"
#include <climits>
#include "UmlExt.h"


namespace DFUtil
{
	bool IsValidFormula(CyPhyML::CustomFormula& formula)
	{
		string invalidOperators = "?<>=#!$%&|~";
		std::string expression = formula.Expression();
		string::size_type pos = expression.find_first_of(invalidOperators);
		if(pos!=std::string::npos)
			return false;
		else
			return true;
	}

	string ConvertToDesertFormat(CyPhyML::CustomFormula& formula)
	{
		// Not supported:
		//		[1] Multiple outputs
		//		[2] Formula-2-Formula		
		
		string expression = formula.Expression();
		map<string, string> VariablePathMap;

		set<CyPhyML::ValueFlow> inputVFs = formula.srcValueFlow();
		set<CyPhyML::ValueFlow> outputVFs = formula.dstValueFlow();

		if (inputVFs.empty() || outputVFs.empty())
			return expression;

		// [1] get output 
		// [2] get inputs' path based on output

		CyPhyML::ValueFlow vf = *(outputVFs.begin());
		CyPhyML::ValueFlowTarget dst_vfend = vf.dstValueFlow_end();
		Udm::Object dst_parent = vf.dstValueFlow_refport_parent();
		
	//	if (Uml::IsDerivedFrom(dst_vfend.type(), CyPhyML::ValueFormula::meta))		
	//		return expression;		
		if(dst_parent==Udm::null)
			dst_parent = dst_vfend.parent();

		if (!Uml::IsDerivedFrom(dst_parent.type(), CyPhyML::DesignEntity::meta))		// check might not be needed
			return expression;

		// create a map of desert conforming variable names
		CyPhyML::DesignEntity cyphy_elem = CyPhyML::DesignEntity::Cast(dst_parent);
		for (set<CyPhyML::ValueFlow>::const_iterator ci = inputVFs.begin(); ci != inputVFs.end(); ci++)
		{
			CyPhyML::ValueFlowTarget src_vfend = ci->srcValueFlow_end();
			Udm::Object src_vfend_ref_parent = ci->srcValueFlow_refport_parent();

			string src_vfend_name = src_vfend.name();
			string FormulaVariableName = ci->FormulaVariableName();

			string variablePath = getRelativePath(src_vfend, src_vfend_ref_parent, cyphy_elem) + src_vfend_name + "()";
			VariablePathMap[(FormulaVariableName == "" ? src_vfend_name : FormulaVariableName)] = variablePath;
		}

		return ConvertExpression(expression, VariablePathMap);
	}

	bool IsValidExpression(string& expression)
	{
		string invalidOperators = "?<>=#!$%&|~";
		string::size_type pos = expression.find_first_of(invalidOperators);
		if(pos!=std::string::npos)
			return false;
		else
			return true;
	}
	string ConvertExpression(string& expression, const map<string, string>& VariablePathMap)
	{
		// 7-7-2012: Converts Carets to exp(exponent*ln(base)) - will convert nested carets
		bool status = true;
		while (status)
			status = ConvertCaret(expression);	
		// 7-7-2012

		static string Operators = "/+-*?<>=#!$%&|~'() ";		// left out ^ b/c we are going to expand it

		string desertExpression;
		CheckExpression(expression);
		
		string::size_type lastPos = expression.find_first_not_of(Operators, 0);		// skip delimiters at beginning		
		string::size_type pos = expression.find_first_of(Operators, lastPos);		// find first "non-delimiter"
		string::size_type delimBegin;

		if (lastPos == 0)
			delimBegin = std::string::npos;
		else
			delimBegin = expression.find_first_of(Operators, 0);

		while (std::string::npos != pos || std::string::npos != lastPos)
		{
			if (std::string::npos != delimBegin)			
				desertExpression += expression.substr(delimBegin, lastPos - delimBegin);
			
			string token = expression.substr(lastPos, pos - lastPos);						// found piece w/o Operator characters
			ReplaceToDesertName(token, VariablePathMap);
			desertExpression += token;

			delimBegin = pos;											
			lastPos = expression.find_first_not_of(Operators, pos);							// skip delimiters using "not_of"			
			pos = expression.find_first_of(Operators, lastPos);								// find next "non-delimiter"
		}

		if (delimBegin < expression.length())
			desertExpression += expression.substr(delimBegin);

		return desertExpression;
	}

	
	void ReplaceToDesertName(string& token, const map<string, string>& VariablePathMap)
	{
		map<string, string>::const_iterator ci = VariablePathMap.find(token);
		if (ci != VariablePathMap.end())
		{
			string replacement = ci->second;
			token = replacement;
		}

		
#if 0		// 7-7-2012: Using ConvertCaret().

		string::size_type expPos = token.find("^");
		string toBReplaced = token.substr(0, expPos);

		map<string, string>::const_iterator ci = VariablePathMap.find(toBReplaced);
		if (ci != VariablePathMap.end())
		{
			string replacement = ci->second;

			if (expPos != string::npos)		// exponent
			{
				int exponent = std::atoi(token.substr(expPos + 1, token.length()-expPos).c_str());
				if (exponent == 0 || exponent == INT_MAX || exponent == INT_MIN)
					throw udm_exception ("Encountered unconvertable/0 exponent value in formula at: " + token);

				string tmp = "(";
				for (int i = 1; i <= abs(exponent); i++)		// use abs() in case of negative exponent
				{
					tmp += replacement;
					if (i < exponent)
						tmp += "*";
				}
				tmp += ")";

				if (exponent < 0)								// negative exponent
					token = "(1/" + tmp + ")";
				else
					token = tmp;
			}
			else							// no exponent
				token = replacement;
		}
#endif
	}

	bool ConvertCaret(std::string& expression)
	{
		// FIXME replace this with a proper parser
		//  (e.g. what if ^ is contained within a string)
		bool hasCarat = 0;
		string Operators = "/+-*?<>=#!$%&|~' ";
		string startExpr, endExpr;
		int size = expression.size(), start, end;
	
		size_t pos = expression.find("^");
		if (pos != std::string::npos)
		{
		
			std::string left, right;

			// find left side
			stack<char> bracketStack;
			int i = pos-1;
			for (; i > 0; --i)
			{

				char chr = expression[i];
				if (chr == ')')
					bracketStack.push(chr);
				else if (chr == '(')
				{
					if (bracketStack.empty())
						break;
					else
					{
						bracketStack.pop();
						if (bracketStack.empty())
							break;
					}
				}
				else
				{
					if (bracketStack.empty() && Operators.find(chr) != std::string::npos)
					{
						i++;
						break;
					}
				}			
			}

			start = i;
			startExpr = expression.substr(0, start);
			left = expression.substr(i, pos-i);

			// find right side
			bracketStack = stack<char>();
			i = pos+1;
			for (; i < size; ++i)
			{
				char chr = expression[i];
			
				if (chr == '(')
					bracketStack.push(chr);
				else if (chr == ')')
				{
					if (bracketStack.empty())
						break;
					else
					{
						bracketStack.pop();
						if (bracketStack.empty())
							break;
					}
				}
				else
				{
					if (bracketStack.empty() && Operators.find(chr) != std::string::npos)
					{
						i--;
						break;
					}
				}	
			}
			end = i+1;
			if(end<size)
				endExpr = expression.substr(end,size-1);				
			else
				endExpr = "";
			right = expression.substr(pos+1, i-pos);

			std::string newString = startExpr + ("(exp(" + right + "*ln(" + left + ")))") + endExpr;
			expression = newString;

			if (expression.find("^") != std::string::npos)
				hasCarat = 1;
		}
		return hasCarat;
	}


	string getRelativePath(Udm::Object &obj, const CyPhyML::DesignEntity &cyphy_elem)
	{
		std::string ret;
		std::string nn = cyphy_elem.name();
		if(obj == Udm::null) return ret;
		
		Udm::Object pobj = obj.GetParent();
		//if(pobj == cyphy_elem)
		//	ret = "self.";
		if (pobj != cyphy_elem)
		{
			Udm::Object ppobj = pobj.GetParent();
			if(ppobj == cyphy_elem)
			{
				ret = "self.children(\""+UdmUtil::ExtractName(pobj)+"\").";
			}
			else
			{
				Udm::Object cobj = cyphy_elem.parent();
				if(cobj == pobj)
					ret = "self.parent().";
				else if(cobj == ppobj)
					ret = "self.parent().children(\""+UdmUtil::ExtractName(pobj)+"\").";
			}
		} 
		return ret;
	}

	string getRelativePath(Udm::Object &obj, Udm::Object &obj_ref_parent, const CyPhyML::DesignEntity &cyphy_elem)
	{
		std::string ret;
		if(obj == Udm::null) return ret;

		Udm::Object pobj = obj_ref_parent;
		if(pobj==Udm::null) 
			pobj = obj.GetParent();

		if(pobj == cyphy_elem)
			ret = "";
		else
		{
			Udm::Object ppobj = pobj.GetParent();
			if(ppobj == cyphy_elem)
			{
				ret = "children(\""+UdmUtil::ExtractName(pobj)+"\").";
			}
			else
			{
				Udm::Object cobj = cyphy_elem.parent();
				if(cobj == pobj)
					ret = "parent().";
				else if(cobj == ppobj)
					ret = "parent().children(\""+UdmUtil::ExtractName(pobj)+"\").";
				else
				{
					if(Uml::IsDerivedFrom(cobj.type(), CyPhyML::DesignEntity::meta))
						ret = "parent()."+getRelativePath(obj, obj_ref_parent, CyPhyML::DesignEntity::Cast(cobj));
				}
			}
		}	
		return ret;
	}

	void CheckExpression(string& expression)
	{
		string cleanExpression;

		for (int i = 0; i < expression.size(); i++)
		{
			if (expression[i] != '\n' && expression[i] != '\t')
				cleanExpression += expression[i];
		}
		expression = cleanExpression;
	}

};

#include <Windows.h>
#include <OleAuto.h>
extern "C" __declspec(dllexport) BSTR __stdcall Exponentiate(wchar_t* expression)
{
	std::string expr;
	{
		char buf[4096];
		sprintf_s(buf, "%S", expression);
		map<string, string> VariablePathMap;
		expr = buf;
		bool status = true;
		while (status)
			status = DFUtil::ConvertCaret(expr);
	}

	{
		wchar_t buf[4096];
		swprintf_s(buf, L"%S", expr.c_str());
		return SysAllocString(buf);
	}
}