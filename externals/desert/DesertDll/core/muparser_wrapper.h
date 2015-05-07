#include <string>
#include <exception>

double muParserSetAndEval(std::string expression);
double muParserSetAndEval(std::wstring expression);

class muparser_exception : public std::exception {
public:
	muparser_exception(const char* what) :
		std::exception(what) {}
};
