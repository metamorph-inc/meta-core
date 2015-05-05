/*
 * Himanshu: Using the 'muParser' library functions, implemented a math function call for BDDs.
*/

#include "core/bddmain.h"
#include "muParser.h"


extern "C"
double mtbdd_call_muParser_math_function(double to, bdd_unary_math_function math_function)
{
   mu::Parser parser;
   parser.ResetLocale();
   std::string expression;
   std::stringstream expressionStr;
   switch(math_function)
   {
   case bdd_sin:
	   expressionStr << "sin(" << to << ")";
	   break;
   case bdd_cos:
	   expressionStr << "cos(" << to << ")";
	   break;
   case bdd_tan:
	   expressionStr << "tan(" << to << ")";
	   break;
   case bdd_asin:
	   expressionStr << "asin(" << to << ")";
	   break;
   case bdd_acos:
	   expressionStr << "acos(" << to << ")";
	   break;
   case bdd_atan:
	   expressionStr << "atan(" << to << ")";
	   break;
   case bdd_sinh:
	   expressionStr << "sinh(" << to << ")";
	   break;
   case bdd_cosh:
	   expressionStr << "cosh(" << to << ")";
	   break;
   case bdd_tanh:
	   expressionStr << "tanh(" << to << ")";
	   break;
   case bdd_asinh:
	   expressionStr << "asinh(" << to << ")";
	   break;
   case bdd_acosh:
	   expressionStr << "acosh(" << to << ")";
	   break;
   case bdd_atanh:
	   expressionStr << "atanh(" << to << ")";
	   break;
   case bdd_log2:
	   expressionStr << "log2(" << to << ")";
	   break;
   case bdd_log10:
	   expressionStr << "log10(" << to << ")";
	   break;
   case bdd_ln:
	   expressionStr << "ln(" << to << ")";
	   break;
   case bdd_exp:
	   expressionStr << "exp(" << to << ")";
	   break;
   case bdd_sqrt:
	   expressionStr << "sqrt(" << to << ")";
	   break;
   case bdd_sign:
	   expressionStr << "sign(" << to << ")";
	   break;
   case bdd_rint:
	   expressionStr << "rint(" << to << ")";
	   break;
   case bdd_abs:
	   expressionStr << "abs(" << to << ")";
	   break;
   default:
	   break;
   }
   expression = expressionStr.str();
   parser.SetExpr(expression);
  return parser.Eval();
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
	catch(mu::Parser::exception_type &e)
	{
		return 1;
	}
	return 0;
}