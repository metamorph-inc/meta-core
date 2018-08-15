
#define _UNICODE
#define UNICODE

#include <afx.h>
#include "muparser_wrapper.h"
#include "muParser.h"

double muParserSetAndEval(std::string expression) {
	try {
		mu::Parser parser;
		parser.ResetLocale();
		parser.SetExpr(std::wstring(static_cast<const wchar_t*>(CString(expression.c_str()))));
		return parser.Eval();
	}
	catch (mu::Parser::exception_type &e) {
		throw muparser_exception(CStringA((e.GetExpr() + _T(": ") + e.GetMsg()).c_str()));
	}
}

double muParserSetAndEval(std::wstring expression) {
	try {
		// FIXME memoize this
		mu::Parser parser;
		parser.ResetLocale();
		parser.SetExpr(expression);
		return parser.Eval();
	}
	catch (mu::Parser::exception_type &e) {
		throw muparser_exception(CStringA((e.GetExpr() + _T(": ") + e.GetMsg()).c_str()));
	}
}
