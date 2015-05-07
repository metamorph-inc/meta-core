#include "Uml.h"
#include "UdmDom.h"
#include "SFCprint.h"
#include "CExpr2SFC.hpp"
#include <fstream>
#include <time.h>
#include <algorithm>
#include <map>

#pragma warning(disable:4244)
#pragma setlocale("enu")

using namespace SFC;

static std::string gRootStateName;

static ofstream ofs;
static ofstream hfs;

static int il = 0;
static char *indents[] = {
  "",
  "  ",
  "    ",
  "      ",
  "        ",
  "          ",
  "            ",
  "              ",
  "                ",
  "                  ",
  "                    ",
  "                      ",
  "                        ",
  "                          ",
  0
};

// ordering functions
struct StatementOrder {
	bool operator()(const SFC::Statement &a, const SFC::Statement &b) {
		return a.statementIndex() < b.statementIndex();  // based on index
	}
};

template< class ArgType >
struct ArgOrder {  
	bool operator()(const ArgType& a, const ArgType& b) {
	return a.argIndex() < b.argIndex();  // based on index
	}
};

typedef std::set< SFC::Declaration, StatementOrder > DeclarationSet;

typedef std::map< std::string, std::string > DTKeyNameMap;

static DTKeyNameMap &getDTKeyNameMap( void ) {
	static DTKeyNameMap dtKeyNameMap;
	return dtKeyNameMap;
}

std::string getDTKey( SFC::DT dt );
std::string getTypeName( SFC::DT dt );
void makeTypedef( SFC::Struct sfcStruct );


bool is_empty(const UserCode& this_)
{
  string expr = this_.expr();
  set< Exprs > exprsSet = this_.Exprs_kind_children();

  return (exprsSet.empty() && expr.empty());
}

bool is_empty(const CompoundStatement& this_)
{
  if (Udm::IsDerivedFrom(this_.type(),Function::meta ))
    return false;

  int sc = this_.statementCount();
  if (sc == 0)
    return true;
 
  bool ret=true;
  const set<Statement>& ss = this_.stmnt();
  for(set<Statement>::const_iterator si=ss.begin(); ret && si!=ss.end(); si++)
  {
    const Statement& s = *si;
    if (Udm::IsDerivedFrom(s.type(), Function::meta))
      ret = false;
    else if (Udm::IsDerivedFrom(s.type(), UserCode::meta))
      ret = is_empty( UserCode::Cast(s) );
    else if (Udm::IsDerivedFrom(s.type(), CompoundStatement::meta))
      ret = is_empty( CompoundStatement::Cast(s) );
    else
      ret = false;
  }
  return ret;
}

template<class PT,class CT>
void sort_print(const PT& this_, const CT& dummy, const std::string &tagPrefix)
{
  std::set< CT, StatementOrder > ss = Udm::ChildrenAttr<CT, StatementOrder >( this_.__impl(), CompoundStatement::meta_stmnt );
  for(typename std::set< CT, StatementOrder >::const_iterator si=ss.begin(); si!=ss.end(); si++)
    Print( Statement::Cast(*si), tagPrefix );
}

struct StringComparator {
	bool operator()(const std::string& a, const std::string& b) const { return a.compare( b ) < 0; }
};
// these are the variables that live in a context data structure in the generated state machine
map< string, Var, StringComparator > contextVars;

std::string dropExtension( const std::string& filename)
{
	std::string nameNoExt( filename);
	std::string::size_type findExt= nameNoExt.rfind( '.');
	if ( findExt != std::string::npos)
		if( *(nameNoExt.begin()+ findExt+ 1) != '\\')
			nameNoExt.erase( nameNoExt.begin()+ findExt, nameNoExt.end());	// delete '.' too.
	return nameNoExt;
}

std::string noParens( const std::string &rhs ) {
	std::string lhs = rhs;
	std::string::size_type pos = 0;
	while(   (  pos = lhs.find( "(", pos )  ) != std::string::npos   ) lhs.erase( pos, 1 );
	pos = 0;
	while(   (  pos = lhs.find( ")", pos )  ) != std::string::npos   ) lhs.erase( pos, 1 );
	return lhs;
}

std::string getDTKey( SFC::DT dt ) {
	std::string retval;

	if ( dt.type() == SFC::Matrix::meta ) {

		SFC::Matrix matrix = SFC::Matrix::Cast( dt );
		retval = std::string( "M" ) + noParens(  static_cast< std::string >( matrix.type() )  ) +
		 "C" + matrix.rows() + "C" + matrix.columns() + "M";

	} else {

		retval = std::string( "S" );
		SFC::Struct sfcStruct = SFC::Struct::Cast( dt );

		bool noComma = true;
		DeclarationSet declarationSet = sfcStruct.Declaration_kind_children_sorted( StatementOrder() );
		for(
		 DeclarationSet::iterator dcsItr = declarationSet.begin() ;
		 dcsItr != declarationSet.end() ;
		 (void)++dcsItr
		) {
			if ( noComma ) { noComma = false; retval += "C"; }
			retval += getDTKey( dcsItr->dt() );
		}
		retval += "S";

	}

	return retval;
}

std::string getTypeName( SFC::DT dt ) {

	std::string typeName;

	if ( dt.type() == SFC::Matrix::meta ) {
		typeName = SFC::Matrix::Cast( dt ).type();
		// FIXME: should this ever be empty?
		if ( typeName.empty() ) typeName = "double";
		else                    typeName = TypeMap::lookup( typeName );
	} else { // SFC::Struct
		makeTypedef(  SFC::Struct::Cast( dt )  );
		typeName = getDTKeyNameMap()[ getDTKey( dt ) ];
	}

	return typeName;
}

void makeTypedef( SFC::Struct sfcStruct ) {

	std::string dtKey = getDTKey( sfcStruct );

	DTKeyNameMap::iterator knmItr = getDTKeyNameMap().find( dtKey );
	if ( knmItr != getDTKeyNameMap().end() ) return;

	// MAKE SURE ALL STRUCTURE MEMBERS OF THIS STRUCT HAVE A typedef
	DeclarationSet declarationSet = sfcStruct.Declaration_kind_children_sorted( StatementOrder() );

	for(
	 DeclarationSet::iterator dcsItr = declarationSet.begin() ;
	 dcsItr != declarationSet.end() ;
	 (void)++dcsItr
	) {
		SFC::DT dt = dcsItr->dt();
		if ( dt.type() == SFC::Struct::meta ) makeTypedef(  SFC::Struct::Cast( dt )  );
	}


	// WRITE OUT THE NEW typedef FOR THIS STRUCT
	hfs << "typedef struct" << std::endl << "{" << std::endl;

	int memberNo = 0;
	for(
	 DeclarationSet::iterator dcsItr = declarationSet.begin() ;
	 dcsItr != declarationSet.end() ;
	 (void)++dcsItr
	) {
		SFC::DT dt = dcsItr->dt();

		if ( dt.type() == SFC::Matrix::meta ) {
			SFC::Matrix matrix = SFC::Matrix::Cast( dt );
			string cTypeName = TypeMap::lookup( matrix.type() );
			int size1 = matrix.rows();
			int size2 = matrix.columns();
			hfs << "  " << cTypeName << " member" << memberNo;
			if ( size1 > 1 ) hfs << "[" << size1 << "]";
			if ( size2 > 1 ) hfs << "[" << size2 << "]";
			hfs << ";" << std::endl;
		} else {
			hfs << "  " << getDTKeyNameMap()[ getDTKey( dt ) ] << " member" << memberNo << std::endl;
		}

		(void)++memberNo;
	}

	static int structNo = 0;

	std::string structName = std::string( "struct" ) + structNo;
	(void)++structNo;

	hfs << "} " << structName << ";" << std::endl << std::endl;

	getDTKeyNameMap().insert(  std::make_pair( dtKey, structName )  );

	return;
}

void Print(const Program& this_)
{
  gRootStateName =  std::string( this_.filename()).substr( 0,  std::string( this_.filename()).size() - 2);
  // initialize the type map

  int dimSV= 0;
  // add the local vars to a hash table - needed for searching variables later
  {
	  const std::set<Var> cvars = this_.Var_kind_children();
	  for(std::set<Var>::const_iterator ci = cvars.begin(); ci != cvars.end(); ci++)
	  {
		  const Var& v = *ci;
		  string vn = v.name();
		  contextVars.insert( make_pair( vn, v ) );
	  }
  }

  const string& fname = this_.filename();
  string header_fname= dropExtension( fname) + ".h";
  ofs.open( fname.c_str(), ios::binary );
  hfs.open( header_fname.c_str());

  il = 0;

  time_t ltime;
  time( &ltime );

  ofs << "/* " << fname.c_str() << " generated on "
      << ctime( &ltime )  << " */"
      << endl << endl;

  hfs << "/* " << header_fname.c_str() << " generated on "
      << ctime( &ltime )  << " */"
      << endl << endl;
  
  {
	ofs << "#include \"" << header_fname << "\"" << endl << endl;
	ofs << "typedef struct __context {" << endl;
	il++;

	// print StateVar
	sort_print<Program,StateVar>(this_,StateVar(), "");
	ofs << endl;
		// dimension of statevar:
	std::set< SFC::StateVar> svs= this_.StateVar_kind_children();
	if ( !svs.empty())
		dimSV= svs.begin()->size();

	// print LocalVars
	sort_print<Program,LocalVar>(this_,LocalVar(), "");
	il--;
	ofs << "} " << gRootStateName << "_context;" << endl;
	ofs << gRootStateName << "_context contexts[" << (__int64)this_.numInstance() << "];" << endl << endl;
  }

  {	// Print Logfunc
	ofs << "#include <stdio.h>" << endl << endl;
	ofs << "void logfunc( " << gRootStateName << "_context* __ctx, const char* msg) {\n";
	ofs << "\tFILE* logf= fopen( \"" << gRootStateName<< ".log\", \"a\");" << endl;
	ofs << "\tfputs( msg, logf);" << endl;
	ofs << "\tfprintf( logf, \"Active substates:";
	for ( int i= 0; i < dimSV; ++i) { 
		if (i != 0)
			ofs << ",";
		ofs << " %d";
	}
	ofs << "\\n\",";
	for ( int j= 0; j < dimSV; ++j) { 
		if (j != 0)
			ofs << ",";
		ofs << " __ctx->activeSubStates[ " << std::dec << j << "]";
	}
	ofs << ");" << endl;
	ofs << "\tfclose( logf);" << endl;
	ofs << "}" << endl << endl;
  }

  // print StateLabels
  sort_print<Program,StateLabel>(this_,StateLabel(), "");
  ofs << endl << endl;

  // print Function prototypes
  {
    std::set<Function, StatementOrder > fn_s = this_.Function_kind_children_sorted( StatementOrder() );
    for(std::set<Function, StatementOrder >::const_iterator fn_i=fn_s.begin(); fn_i!=fn_s.end(); fn_i++)
    {
      Print( *fn_i, true );	// this prints the function header
      ofs << ";" << endl;
    }
  }
  ofs << endl << endl;

  // print Functions
  sort_print<Program,OperationalStatement>(this_,CompoundStatement(), "");
  ofs << endl << endl;

  ofs.flush();
  ofs.close();
  hfs.close();
}

void Print(const Statement& this_, const std::string &tagPrefix)
{
  if (Udm::IsDerivedFrom(this_.type(), OperationalStatement::meta))
    Print( OperationalStatement::Cast(this_), tagPrefix );
  else if (Udm::IsDerivedFrom(this_.type(), Declaration::meta))
    Print( Declaration::Cast(this_), tagPrefix );
  else
    ASSERT(false);
}

void Print(const OperationalStatement& this_, const std::string &tagPrefix)
{
  if (Udm::IsDerivedFrom(this_.type(), CompoundStatement::meta))
    Print( CompoundStatement::Cast(this_), tagPrefix );
  else if (Udm::IsDerivedFrom(this_.type(), SimpleStatement::meta))
    Print( SimpleStatement::Cast(this_), tagPrefix );
  else
    ASSERT(false);
}

void Print(const Declaration& this_, const std::string &tagPrefix)
{
  if (Udm::IsDerivedFrom(this_.type(), StateLabel::meta))
    Print( StateLabel::Cast(this_) );
  else if (Udm::IsDerivedFrom(this_.type(), Var::meta))
    Print( Var::Cast(this_), tagPrefix );
  else
    ASSERT(false);
}

void Print(const CompoundStatement& this_, const std::string &tagPrefix)
{
  if (is_empty(this_))
    return;

  if (Udm::IsDerivedFrom(this_.type(), Block::meta))
    Print( Block::Cast(this_), tagPrefix );
  else if (Udm::IsDerivedFrom(this_.type(), ConditionalGroup::meta))
    Print( ConditionalGroup::Cast(this_), tagPrefix );
  else
    ASSERT(false);
}

void Print(const SimpleStatement& this_, const std::string &tagPrefix)
{
  if (Udm::IsDerivedFrom(this_.type(), Return::meta))
    Print( Return::Cast(this_) );
  else if (Udm::IsDerivedFrom(this_.type(), FunctionCall::meta))
    Print( FunctionCall::Cast(this_) );
  else if (Udm::IsDerivedFrom(this_.type(), SetState::meta))
    Print( SetState::Cast(this_) );
  else if (Udm::IsDerivedFrom(this_.type(), SetVar::meta))
    Print( SetVar::Cast(this_) );
  else if (Udm::IsDerivedFrom(this_.type(), UserCode::meta)) {
    UserCode userCode = UserCode::Cast(this_);
    if (is_empty(userCode))
       return;
    Print( userCode );
  } else
    ASSERT(false);

  ofs << ";" << " /* " << tagPrefix + '#' + this_.statementIndex() << " */" << std::endl;
}

void Print(const StateLabel& this_) /* NO "tagPrefix" NECESSARY DUE TO comment() ATTRIBUTE */
{
  string nm = this_.name();
  string cmt = this_.comment();
  int id = this_.value();
  
  hex(ofs);
  ofs << indents[il] << "const int " << nm << "= 0x" << id << "; /* STATELABEL/" << cmt << " */" << endl;
}

void Print(const Var& this_, const std::string &tagPrefix)
{
	std::string newTagPrefix = tagPrefix.empty() ? "CONTEXT" : tagPrefix;
	SFC::DT dt = this_.dt();

	string name= this_.name();

	std::string typeName;
	int size1 = 1;
	int size2 = 1;

	if ( dt == Udm::null ) {
		typeName = TypeMap::lookup( this_.type() );
		size1 = this_.size();
	} else {
		typeName = getTypeName( dt );
	}

	if ( typeName.empty() ) typeName = "double";

	ofs << indents[il] << typeName.c_str() << " " << name;
	if ( size1 > 1 || name == "activeSubStates" ) {
		ofs << "[" << std::dec << size1 << "]";
	}
	if ( size2 > 1 ) {
		ofs << "[" << std::dec << size2 << "]";
	}

	ofs << "; /* " + newTagPrefix + '#' + this_.statementIndex() << " */" << endl;
}

void Print(const Block& this_, const std::string &tagPrefix)
{
  std::string tag = tagPrefix + '#' + this_.statementIndex();
  if (Udm::IsDerivedFrom(this_.type(), Function::meta))
  {
    ofs << endl;

	Function function = Function::Cast( this_ );
    Print( function );

	tag = function.scope();
	if ( tag.empty() ) tag = function.name();
  }
  else if (Udm::IsDerivedFrom(this_.type(), ConditionalBlock::meta))
    Print( ConditionalBlock::Cast(this_) );
  else if (Udm::IsDerivedFrom(this_.type(), IterativeBlock::meta))
    Print( IterativeBlock::Cast(this_) );
  else
    ASSERT(false);

  ofs << " /* " << tag << " */";
  ofs << endl << indents[il++] << "{" << endl;

  // first print all declarations
  sort_print<Block,Declaration>(this_,Declaration(), tag);

  // print debug info
  if (Udm::IsDerivedFrom(this_.type(), Function::meta))
  {
	  std::string funcname= Function::Cast( this_).name();
	  if ( (funcname == gRootStateName + "_init") || (funcname == gRootStateName + "_main") )
		  ofs << indents[il] << gRootStateName << "_context *__ctx = &contexts[__instance];" << endl << endl;
	  ofs << "logfunc( __ctx, \"" << funcname << "\\n\");" << endl << endl;
  }

  // then print all op statements
  sort_print<Block,OperationalStatement>(this_,OperationalStatement(), tag);
  ofs << indents[--il] << "}" << endl;
}

void Print(const ConditionalGroup& this_, const std::string &tagPrefix)
{
  std::string newTagPrefix = tagPrefix + '#' + this_.statementIndex();

  std::set< ConditionalBlock, StatementOrder > cbs = this_.ConditionalBlock_kind_children_sorted( StatementOrder() );
  int n=0;
  for(std::set<ConditionalBlock, StatementOrder >::const_iterator cbi=cbs.begin(); cbi!=cbs.end(); cbi++)
  {
    const ConditionalBlock& cb = *cbi;
	if ( !is_empty(cb) ) {
		if ( n > 0 ) ofs << indents[il] << "else ";
		++n;
	}
    Print( Statement::Cast(cb), newTagPrefix );
  }
}

void Print(const Return& this_)
{
  ofs << indents[il] << "return";

  std::set< Exprs > exprsSet = this_.Exprs_kind_children();
  if ( exprsSet.empty() ) {
	  std::string val = this_.val();
	  if ( val != "" ) ofs << " " << val;
	  return;
  }

  ofs << " ";
  Print(  Exprs::Cast( this_.retexpr() )  );
}

void Print(const FunctionCall& this_)
{
  ofs << indents[il];
  bool isLibFunc = false;

  std::set< Udm::Object > functionSet = this_.getAssociation( FunctionCall::meta_callee );
  if ( functionSet.empty() ) {
    std::string libFuncName( this_.libFuncName() );
	if ( libFuncName.empty() ) {
	   ofs << "BAD_FUNCTION";
	} else {
	   ofs << libFuncName;
	}
	isLibFunc = true;
  } else {
    Function fn = this_.callee();
    ofs << fn.name();
  }

  ofs << "(";

  int n = 0;
  if (!isLibFunc) {
	  ofs << "__ctx";
      (void)++n;
  }

  std::set<ArgVal,ArgOrder<ArgVal> > val_s = this_.ArgVal_kind_children_sorted(ArgOrder<ArgVal>());

  for( std::set<ArgVal,ArgOrder<ArgVal> >::const_iterator val_i = val_s.begin(); val_i != val_s.end(); val_i++, n++) {
    if ( n > 0 ) ofs << ", ";

	ArgVal val = *val_i;
	std::set< Exprs > exprsSet = val.Exprs_kind_children();
	if ( exprsSet.empty() ) ofs << val.val();
	else                    Print(  Exprs::Cast( val.argexpr() )  );
  }
  ofs << ")";
}

void Print(const SetState& this_)
{
  const StateLabel& slv=this_.value();
  const StateLabel& sli=this_.index();
  const StateVar& sls = this_.svar();
  bool andState = this_.andState();
  bool reset = this_.invert();

  string vn = slv.name();
  string in = sli.name();
  string sn = sls.name();

  ofs << indents[il] << "__ctx->" << sn << "[" << in << " & 0xFFFF]"; 
  if (andState)
    ofs << (reset ? " &= ~" : " |= ") << "( " << vn << ">>16 )";
  else
    ofs << (reset ? " = -" : " = ") << vn;
}

void Print(const SetVar& this_)
{
	const Arg& arg=this_.arg_end();
	const LocalVar& lv=this_.lvar_end();
	bool invert = this_.invert();

	string an = arg.name();
	string lvn = lv.name();

	SFC::DT dt = arg.dt();

	string typeName;
	int size = 1;

	if ( dt == Udm::null ) {

		typeName = TypeMap::lookup( arg.type() );
		size = arg.size();

	} else {

		typeName = getTypeName( dt );
		if ( dt.type() == SFC::Matrix::meta ) {
			SFC::Matrix matrix = SFC::Matrix::Cast( dt );
			size = matrix.rows() * matrix.columns();
		} else {
			size = 1;
		}

	}

	if ( typeName.empty() ) typeName = "double";
	if ( size < 1 ) size = 1;

	bool is_ptr = arg.ptr();

	map< string, Var, StringComparator >::const_iterator cvi = contextVars.find( lvn );
	bool isContextVar = cvi != contextVars.end();

	if ( size > 1 )
	{
		if (!invert) {
			ofs << indents[il] << "memcpy(";
			if ( isContextVar ) ofs << "__ctx->";
			ofs << lvn << ", " << an << ", " << (__int64)size << "*sizeof(" << typeName << ") )";
		} else {
			ofs << indents[il] << "memcpy(" << an << ", ";
			if ( isContextVar ) ofs << "__ctx->";
			ofs << lvn << ", " << (__int64)size << "*sizeof(" << typeName << ") )";
		}
	} else {
		if (!invert) {
			ofs << indents[il];
			if ( isContextVar ) ofs << "__ctx->";
			ofs << lvn << (is_ptr ? " = *" : " = ") << an;
		} else {
			ofs << indents[il] << (is_ptr ? "*" : "") << an << " = ";
			if ( isContextVar ) ofs << "__ctx->";
			ofs << lvn;
		}
	}
}

void Print(const UserCode& this_, bool isCond)
{
  std::set< Exprs > exprsSet = this_.Exprs_kind_children();
  string expr = this_.expr();

  if (exprsSet.empty() && expr.empty() )
	  return;

  if ( !isCond ) ofs << indents[il];

  if ( exprsSet.empty() ) {
	  int pos = expr.find("\\n");
	  while( pos != string::npos ) {
	   expr.replace(pos, 2, "  ");
	    pos = expr.find("\\n", pos);
	  }

	  if (  expr.substr( expr.size() - 1, 1 )  ==  ";"  ) {
	    expr = expr.substr( 0, expr.size() - 1 );
	  }

	  ofs << expr;
	  return;
  }

//std::cout << "UserCode expression = " << std::string( this_.expr() ) << std::endl;
//std::cout << "exprsSet.size() = " << exprsSet.size() << std::endl;
  Print(  Exprs::Cast( this_.codeexpr() )  );
  return;
}

void Print(const LocalVar& this_)
{
  string init = this_.initial();
  int size = this_.size();
  if ( size < 1 ) size = 1;

  if (init.empty())
    return;
  ofs << " = " << (size>1 ? "{" : "");
  for(int i=0; i<size; i++)
    ofs << ( i==0 ? "" : "," ) << init;
  ofs << (size>1 ? "}" : "");
}

void Print(const Function& this_, bool prototype)
{
	string nm = this_.name();
	bool isMain= ( nm == gRootStateName + "_main");
	bool isInit= ( nm == gRootStateName + "_init");
	bool isMainOrInit= isMain || isInit;
	bool printHeader= prototype && isMainOrInit;

	if ( isMainOrInit) {
		ofs << indents[il] << "void " << nm << "(int __instance";
	} else {
		std::string returnType( this_.returnType() );
		if ( !prototype ) ofs << "/** Generated from state '" << this_.scope() << "' **/" << std::endl;
		ofs << indents[il] << "static " << TypeMap::lookup( returnType.c_str() ) << " " << nm << "( ";
		ofs << gRootStateName << "_context* __ctx";
	}
	if ( printHeader) {
		hfs << "extern void " << nm << "(";
		if (isMainOrInit) hfs << "int __instance";
	}
	std::set<Arg,ArgOrder<Arg> > arg_s = this_.Arg_kind_children_sorted(ArgOrder<Arg>());
	int n=1;
	for(std::set<Arg,ArgOrder<Arg> >::const_iterator arg_i=arg_s.begin(); arg_i!=arg_s.end(); arg_i++,n++)
	{
		Arg arg = *arg_i;
		string an = arg.name();

		std::string typeName;
		bool isScalar = true;

		SFC::DT dt = arg.dt();
		if ( dt == Udm::null ) {
			typeName = TypeMap::lookup( arg.type() );
		} else {
			typeName = getTypeName( dt );
			if ( dt.type() == SFC::Matrix::meta ) {
				SFC::Matrix matrix = SFC::Matrix::Cast( dt );
				if ( matrix.rows() > 1 || matrix.columns() > 1 ) isScalar = false;
			}
		}
		if ( typeName.empty() ) typeName = "double";

		bool is_ptr = arg.ptr() || !isScalar;

		ofs << (n>0 ? ", " : "") << typeName << (is_ptr ? " *" : " ") << an;
		if ( printHeader) {
			hfs << (n>0 ? ", " : "") << typeName << (is_ptr ? " *" : " ") << an;
		}
	}
	ofs << ")";
	if ( printHeader) {
		hfs << ");" << endl;
	}
}

void Print(const ConditionalBlock& this_)
{
  ofs << indents[il] << "if (";
  const std::set<Condition>& cs = this_.cond();
  int n=0;
  for(std::set<Condition>::const_iterator ci=cs.begin(); ci!=cs.end(); ci++, n++)
  {
    if (n>0) ofs << " && ";
    Print( *ci );
  }
  ofs << ")";
}

void Print(const IterativeBlock& this_)
{
  ofs << indents[il] << "while (";
  const std::set<Condition>& cs = this_.cond();
  int n=0;
  for(std::set<Condition>::const_iterator ci=cs.begin(); ci!=cs.end(); ci++, n++)
  {
    if (n>0) ofs << " && ";
    Print( *ci );
  }
  ofs << ")";
}

void Print(const Condition& this_)
{
  if (Udm::IsDerivedFrom(this_.type(), UserCode::meta))
    Print( UserCode::Cast(this_), true );
  else if (Udm::IsDerivedFrom(this_.type(), CheckArg::meta))
    Print( CheckArg::Cast(this_) );
  else if (Udm::IsDerivedFrom(this_.type(), CheckState::meta))
    Print( CheckState::Cast(this_) );
}

void Print(const CheckArg& this_)
{
  const StateLabel& sl=this_.slab_end();
  const Arg& arg=this_.arg_end();

  string sn = sl.name();
  string an = arg.name();

  ofs << sn << " != " << an;
}

void Print(const CheckState& this_)
{
  const StateLabel& slv=this_.value();
  const StateLabel& sli=this_.index();
  const StateVar& sls = this_.svar();
  bool andState = this_.andState();
  bool invert = this_.invert();

  string vn = slv.name();
  string in = sli.name();
  string sn = sls.name();

  if ( invert ) ofs << "!";
  ofs << "(__ctx->" << sn << "[" << in << " & 0xFFFF]"; 
  if ( andState ) ofs << " & ( " << vn << ">>16 ))";
  else            ofs << " == " << vn << ")";
}


void Print( const Exprs& this_ ) {

	if (  Udm::IsDerivedFrom( this_.type(), UnaryExprs::meta )  ) {
		Print(  UnaryExprs::Cast( this_ )  ); return;
	}
	if (  Udm::IsDerivedFrom( this_.type(), BinaryExprs::meta )  ) {
		Print(  BinaryExprs::Cast( this_ )  ); return;
	}

	if (  Udm::IsDerivedFrom( this_.type(), Int::meta )  ) {
		int val = Int::Cast( this_ ).val();
		ofs << std::dec << val; return;
	}
	if (  Udm::IsDerivedFrom( this_.type(), Double::meta )  ) {
		// TODO: check this: truncated digits?
		ofs << Double::Cast( this_ ).val(); return;
	}
	if (  Udm::IsDerivedFrom( this_.type(), Str::meta )  ) {
		ofs << Str::Cast( this_ ).val(); return;
	}

	if (  Udm::IsDerivedFrom( this_.type(), ArgDeclRef::meta )  ) {

		ArgDeclRef argDeclRef = ArgDeclRef::Cast( this_ );
		ArgDeclBase argDeclBase = ArgDeclBase::Cast( argDeclRef.argdecl() );

		string lvn;
		if ( argDeclBase != Udm::null ) {
			if ( argDeclBase.type() == Arg::meta ) {
				Arg arg = SFC::Arg::Cast( argDeclBase );
				if (arg.ptr() == true) ofs << "*";		// TBD SKN HACK - Harmon fix this
				ofs << arg.name();
				return;
			}
			if (  Udm::IsDerivedFrom( argDeclBase.type(), Declaration::meta )  ) {
				Declaration declaration = Declaration::Cast( argDeclRef.argdecl() );
				if (  Udm::IsDerivedFrom( declaration.type(), SFC::LocalVar::meta )  ) {
					lvn = LocalVar::Cast( declaration ).name();
				} else if (  Udm::IsDerivedFrom( declaration.type(), SFC::StateVar::meta )  ) {
					lvn = StateVar::Cast( declaration ).name();
				} else {
					lvn = StateLabel::Cast( declaration ).name();
				}
			}
		} else {
			lvn = argDeclRef.unres();
		}

		// find if the variable is a global (context) variable 
		// the var is a global variable - access it via ctx ptr
		map< string, Var, StringComparator >::const_iterator cvi = contextVars.find( lvn );
		if (cvi != contextVars.end() ) // found
			ofs << "(__ctx->" << lvn << ")";
		else
			ofs << lvn; 
		return;
	}


	if (  Udm::IsDerivedFrom( this_.type(), FunctionCall::meta )  ) {
		Print(  FunctionCall::Cast( this_ )  );
		return;
	}

}

void Print(const UnaryExprs& this_) {
	std::string op( this_.op() );
	int this_precedence = CExpr2SFC::getUnaryPrecedence( op );

	Exprs subExprs = Exprs::Cast( this_.subexpr() );
	int subExpr_precedence = CExpr2SFC::getOperatorPrecedence( subExprs );

	bool parensNeeded = this_precedence > subExpr_precedence;

	if ( op == "post++" ) {
		if ( parensNeeded ) ofs << "( "; Print( subExprs ); if ( parensNeeded ) ofs << " )"; ofs << "++";
		return;
	}
	if ( op == "post--" ) {
		if ( parensNeeded ) ofs << "( "; Print( subExprs ); if ( parensNeeded ) ofs << " )"; ofs << "--";
		return;
	}

	ofs << op; if ( parensNeeded ) ofs << "( "; Print( subExprs ); if ( parensNeeded ) ofs << " )";

}

void Print(const BinaryExprs& this_) {
	std::string op( this_.op() );
	int this_precedence = CExpr2SFC::getBinaryPrecedence( op );

	Exprs leftExprs = Exprs::Cast( this_.leftexpr() );
	int leftExprs_precedence = CExpr2SFC::getOperatorPrecedence( leftExprs );

	bool parensNeeded = this_precedence > leftExprs_precedence;
	if ( parensNeeded ) ofs << "( "; Print( leftExprs ); if ( parensNeeded ) ofs << " )";


	Exprs rightExprs = Exprs::Cast( this_.rightexpr() );

	if ( op == "[" ) {
		ofs << "[ (int)"; Print( rightExprs ); ofs << " ]"; return;
	}

	ofs << " " << op << " ";

	int rightExprs_precedence = CExpr2SFC::getOperatorPrecedence( rightExprs );
	parensNeeded = this_precedence > rightExprs_precedence;
	if ( parensNeeded ) ofs << "( "; Print( rightExprs ); if ( parensNeeded ) ofs << " )";

	return;
}

