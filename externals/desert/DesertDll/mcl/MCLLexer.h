#pragma once

#include "MCLLexerBase.h"

class MCLLexerError {
public:
	MCLLexerError(const std::string& error) :
		error(error) {
	}
	std::string error;
};

class MCLLexer : public MCLLexerBase {
public:
	MCLLexer(DLGInputStream *in,
		unsigned bufsize = 2000)
		: MCLLexerBase(in, bufsize)
	{
	}

	virtual void errstd(char *s) {
		char column[200] = {0};
		DLGStringInput* stringInput = dynamic_cast<DLGStringInput*>(input);
		if (stringInput) {
			sprintf_s(column, "column %d", 1 + (int)(stringInput->p - stringInput->input));
		}
		char buf[2048];
		sprintf_s(buf,
			"%s near line %d %s (text was '%s')\n",
			((s == NULL) ? "Lexical error" : s),
			_line, column, _lextext);

		// this leaks memory since parser code is not exception-aware, but better than silent failure
		throw MCLLexerError(std::string(buf));
	}
};
