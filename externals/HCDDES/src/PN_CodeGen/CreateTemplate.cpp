#include <iostream>
#include <fstream>


#include "CreateTemplate.h"

using namespace std;

PNTemplateBuilder::PNTemplateBuilder( const std::string & modname, const std::string & timestamp ) 
{ 
	tmpl.GetTemplateDictionary().SetValue("MODNAME", modname); 
	tmpl.GetTemplateDictionary().SetValue("CREATE_TIME", timestamp); 
}

void PNTemplateBuilder::AddExterns()
{
	tmpl.AddSectionDictionary( "EXTERN_SECTION" );

}

void PNTemplateBuilder::AddSendTransition( const string & pnname, const string & transname,
											vector< pair< string, string > > & varlist )
{
	tmpl.AddSectionDictionary( "SEND_SECTION" );
	tmpl.GetTemplateDictionary().SetValue( "PN_NAME", pnname );
	tmpl.GetTemplateDictionary().SetValue( "TRANS_NAME", transname );
	tmpl.AddSectionDictionary( "VAR_SECTION" );
	for( vector< pair< string, string > >::iterator varIter = varlist.begin();
		 varIter != varlist.end(); varIter++ )
	{
		tmpl.GetTemplateDictionary().SetValue( "VAR_NAME", varIter->first );
		tmpl.GetTemplateDictionary().SetValue( "VAR_TYPE", varIter->second );
	}
	tmpl.PopSectionDictionary();
	tmpl.PopSectionDictionary();
}

void PNTemplateBuilder::EndExterns()
{
	tmpl.PopSectionDictionary();
}

void PNTemplateBuilder::AddPetriNet( const string & pnname )
{
	tmpl.AddSectionDictionary( "PN_SECTION" );
	tmpl.GetTemplateDictionary().SetValue( "PN_NAME", pnname );
}

void PNTemplateBuilder::AddStates( map< string, bool > & statelist )
{
	// Iterate and create state section
	for ( map< string, bool >::iterator stIter = statelist.begin(); stIter != statelist.end(); stIter++ ) {
		tmpl.AddSectionDictionary( "STATE_SECTION" );
		tmpl.GetTemplateDictionary().SetValue( "STATE_NAME", stIter->first );
		tmpl.PopSectionDictionary();
	}	
	
	// Iterate and create initial state section
	for ( map< string, bool >::iterator stIter = statelist.begin(); stIter != statelist.end(); stIter++ ) {
		if ( stIter->second )
		{
			tmpl.AddSectionDictionary( "INITIAL_STATES_SECTION" );
			tmpl.GetTemplateDictionary().SetValue( "STATE_NAME", stIter->first );
			tmpl.PopSectionDictionary();
		}
	}

	// Iterate and create non-initial state section
	for ( map< string, bool >::iterator stIter = statelist.begin(); stIter != statelist.end(); stIter++ ) {
		if ( !(stIter->second) )
		{
			tmpl.AddSectionDictionary( "NONINITIAL_STATES_SECTION" );
			tmpl.GetTemplateDictionary().SetValue( "STATE_NAME", stIter->first );
			tmpl.PopSectionDictionary();
		}
	}
}

void PNTemplateBuilder::AddGlobalVars( map< string, string > & varlist )
{
	for ( map< string, string >::iterator varIter = varlist.begin(); varIter != varlist.end(); varIter++ )
	{
		tmpl.AddSectionDictionary( "GLOBAL_VAR_SECTION" );
		tmpl.GetTemplateDictionary().SetValue( "VAR_NAME", varIter->first );
		tmpl.GetTemplateDictionary().SetValue( "VAR_TYPE", varIter->second );
		tmpl.PopSectionDictionary();
	}
}

void PNTemplateBuilder::AddStruct()
{
	tmpl.AddSectionDictionary( "STRUCT_SECTION" );
	tmpl.PopSectionDictionary();
}

void PNTemplateBuilder::AddSetup()
{
	tmpl.AddSectionDictionary( "SETUP_SECTION" );
	tmpl.PopSectionDictionary();
}

void PNTemplateBuilder::AddTransition( const string & transname, bool inbound, 
									   vector< pair< string, string > > & varlist, 
									   vector< string > & srcstates, 
									   vector< string > & tgtstates,
									   const string & guardstr, const string & actionstr )
{
	tmpl.AddSectionDictionary( "TRANS_SECTION" );
	tmpl.GetTemplateDictionary().SetValue( "TRANS_NAME", transname );

	// Do the vars for this transition
	for ( vector< pair< string, string > >::iterator varIter = varlist.begin(); varIter != varlist.end(); varIter++ )
	{
		tmpl.AddSectionDictionary( "VAR_SECTION" );
		tmpl.GetTemplateDictionary().SetValue( "VAR_NAME", varIter->first );
		tmpl.GetTemplateDictionary().SetValue( "VAR_TYPE", varIter->second );
		tmpl.PopSectionDictionary();
	}

	// Figure out if it's a receiver or sender, and make the appropriate signature
	if ( inbound ) // receiver
	{
		tmpl.AddSectionDictionary( "TRANS_RECV_SECTION" );
	}
	else // sender
	{
		tmpl.AddSectionDictionary( "TRANS_SEND_SECTION" );
	}

	// Source states for this transition
	for ( vector< string >::iterator sstIter = srcstates.begin(); sstIter != srcstates.end(); sstIter++ )
	{
		tmpl.AddSectionDictionary( "TRANS_SOURCE_SECTION" );
		tmpl.GetTemplateDictionary().SetValue( "SRC_STATE_NAME", *sstIter );
		tmpl.PopSectionDictionary();
	}

	// Destination states for this transition
	for ( vector< string >::iterator dstIter = tgtstates.begin(); dstIter != tgtstates.end(); dstIter++ )
	{
		tmpl.AddSectionDictionary( "TRANS_DEST_SECTION" );
		tmpl.GetTemplateDictionary().SetValue( "DST_STATE_NAME", *dstIter );
		tmpl.PopSectionDictionary();
	}

	tmpl.PopSectionDictionary(); // send or receive context

	// Then do the guard and action parts
	tmpl.AddSectionDictionary( "TRANS_GUARD_SECTION" );
	tmpl.GetTemplateDictionary().SetValue( "TRANSLATED_EXPRESSION", guardstr );
	tmpl.PopSectionDictionary();

	tmpl.AddSectionDictionary( "TRANS_ACTION_SECTION" );
	tmpl.GetTemplateDictionary().SetValue( "TRANSLATED_ACTIONS", actionstr );
	tmpl.PopSectionDictionary();

	tmpl.PopSectionDictionary();
}

void PNTemplateBuilder::ClosePetriNet() {

	tmpl.PopSectionDictionary();
}

void PNTemplateBuilder::WriteModuleFile( const std::string & fname, 
										 const std::string & tmplname ) {

    RegisterTemplateFilename(TEMPLATE_FN, tmplname.c_str() );
	ctemplate::Template *googleTemplate = ctemplate::Template::GetTemplate( TEMPLATE_FN, ctemplate::DO_NOT_STRIP );
	std::string output;
	//cout << "Writing file " << fname << ":" << endl;
	//cout << endl << "Potential bad templates:" << endl;
	//ctemplate::TemplateNamelist tnl = ctemplate::TemplateNamelist::GetBadSyntaxList(true,  ctemplate::DO_NOT_STRIP );
	
	//ctemplate::TemplateNamelist::GetBadSyntaxList(true,  ctemplate::DO_NOT_STRIP );
	//cout << endl << "Dictionary dump:" << endl;
	//(&tmpl.GetMainDictionary())->Dump();
	googleTemplate->Expand( &output, &tmpl.GetMainDictionary() );

	ofstream outputFile( fname.c_str() );
	outputFile << output;
	outputFile.close();

}