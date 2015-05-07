#ifndef _CREATE_TEMPLATE_H_
#define _CREATE_TEMPLATE_H_

#include <vector>
#include <map>

#include "utils.h"

#include "ctemplate/template.h"

using namespace std;

class TemplateUtil 
{
	typedef std::vector< ctemplate::TemplateDictionary * >	TemplateDictionaryStack;

private:
	void addTemplateDictionary( ctemplate::TemplateDictionary *templateDictionary ) {
		_templateDictionaryStack.push_back( templateDictionary );
	}

protected:

	TemplateDictionaryStack		_templateDictionaryStack;
	ctemplate::TemplateDictionary		_emptyDictionary;
	ctemplate::TemplateDictionary		*_mainDictionary;

	void ClearDictionary( void ) {
		// Delete the main dictionary
		delete this->_mainDictionary;
		// Create an empty dictionary
		_templateDictionaryStack.clear();
	}

	void InitDictionary() {
		_mainDictionary = this->_emptyDictionary.MakeCopy( "MAIN" );
		addTemplateDictionary( this->_mainDictionary );
	}

public:
	TemplateUtil() : _emptyDictionary("EMPTY") { InitDictionary(); }
	virtual ~TemplateUtil() { ClearDictionary(); }

	ctemplate::TemplateDictionary & GetMainDictionary( void ) { return *(this->_mainDictionary); }

	void AddSectionDictionary( const std::string &sectionName ) {
		this->_templateDictionaryStack.push_back(
			this->GetTemplateDictionary().AddSectionDictionary( sectionName )
		);
	}
	void PopSectionDictionary( void ) {
		if ( _templateDictionaryStack.size() > 1 ) {
			this->_templateDictionaryStack.pop_back();
		} else {
			std::cerr << "WARNING: ATTEMPT TO POP MAIN DICTIONARY" << std::endl;
		}
	}
	ctemplate::TemplateDictionary& GetTemplateDictionary( void ) {
		return *(this->_templateDictionaryStack.back());
	}
};

class PNTemplateBuilder
{
protected:
	TemplateUtil tmpl;

public:
	PNTemplateBuilder( const std::string & modname, const std::string & timestamp );
	virtual ~PNTemplateBuilder() { }

	// First pass for global function declarations
	void AddExterns();
	void AddSendTransition( const string & pnname, const string & transname,
						    vector< pair< string, string > > & varlist );
	void EndExterns();

	void AddPetriNet( const std::string & pnname );
	void AddStates( std::map< std::string, bool > & statelist );
	void AddGlobalVars( map< string, string > & varlist );
	void AddStruct();
	void AddSetup();
	void AddTransition( const string & transname, bool inbound, 
						   vector< pair< string, string > > & varlist, 
						   vector< string > & srcstates, 
						   vector< string > & tgtstates,
						   const string & guardstr, const string & actionstr );
	void ClosePetriNet();

	void WriteModuleFile( const std::string & fname, const std::string & tmplname );
};




#endif // _CREATE_TEMPLATE_H_