#ifndef _ISIS_SFUNC_CODE_TEMPLATE
#define _ISIS_SFUNC_CODE_TEMPLATE

#include <sstream>

class SfuncCodeTemplate {
private:
	class TemplateString {
	private:
		std::string _templateString;
	public:
		TemplateString( void );
		const std::string &getTemplateString( void ) { return _templateString; }
	};

public:
	static const std::string &getTemplate( void ) {
		static TemplateString templateString;
		return templateString.getTemplateString();
	}
};

#endif
