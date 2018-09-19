/*
 * Himanshu: Using the 'muParser' library functions, implemented a math function call for BDDs.
*/

#include "core/bddmain.h"

#include "muparser_wrapper.h"

#include <string>
#include <sstream>
#include <iomanip>
#ifdef _UNICODE
typedef std::wstring tstring;
typedef std::wstringstream tstringstream;
#else
typedef std::string tstring;
typedef std::stringstream tstringstream;
#endif


extern "C"
double mtbdd_call_muParser_math_function(double to, bdd_unary_math_function math_function)
{
   tstring expression;
   tstringstream expressionStr;
   expressionStr.precision(17);
   switch(math_function)
   {
   case bdd_sin:
	   expressionStr << _T("sin(") << to << _T(")");
	   break;
   case bdd_cos:
	   expressionStr << _T("cos(") << to << _T(")");
	   break;
   case bdd_tan:
	   expressionStr << _T("tan(") << to << _T(")");
	   break;
   case bdd_asin:
	   expressionStr << _T("asin(") << to << _T(")");
	   break;
   case bdd_acos:
	   expressionStr << _T("acos(") << to << _T(")");
	   break;
   case bdd_atan:
	   expressionStr << _T("atan(") << to << _T(")");
	   break;
   case bdd_sinh:
	   expressionStr << _T("sinh(") << to << _T(")");
	   break;
   case bdd_cosh:
	   expressionStr << _T("cosh(") << to << _T(")");
	   break;
   case bdd_tanh:
	   expressionStr << _T("tanh(") << to << _T(")");
	   break;
   case bdd_asinh:
	   expressionStr << _T("asinh(") << to << _T(")");
	   break;
   case bdd_acosh:
	   expressionStr << _T("acosh(") << to << _T(")");
	   break;
   case bdd_atanh:
	   expressionStr << _T("atanh(") << to << _T(")");
	   break;
   case bdd_log2:
	   expressionStr << _T("log2(") << to << _T(")");
	   break;
   case bdd_log10:
	   expressionStr << _T("log10(") << to << _T(")");
	   break;
   case bdd_ln:
	   expressionStr << _T("ln(") << to << _T(")");
	   break;
   case bdd_exp:
	   expressionStr << _T("exp(") << to << _T(")");
	   break;
   case bdd_sqrt:
	   expressionStr << _T("sqrt(") << to << _T(")");
	   break;
   case bdd_sign:
	   expressionStr << _T("sign(") << to << _T(")");
	   break;
   case bdd_rint:
	   expressionStr << _T("rint(") << to << _T(")");
	   break;
   case bdd_abs:
	   expressionStr << _T("abs(") << to << _T(")");
	   break;
   default:
	   break;
   }
   expression = expressionStr.str();

   return muParserSetAndEval(expression);
}

extern "C"
int call_muParser_math_function(double arg, double *ret, bdd_unary_math_function math_function)
{
	if(arg<=0)
		if(math_function==bdd_log2 || math_function==bdd_log10 || math_function==bdd_ln)
			return 1;
	if(arg<0 && math_function==bdd_sqrt)
		return 1;

	try{
		*ret = mtbdd_call_muParser_math_function(arg, math_function);
	}
	catch(std::exception &e)
	{
		return 1;
	}
	return 0;
}