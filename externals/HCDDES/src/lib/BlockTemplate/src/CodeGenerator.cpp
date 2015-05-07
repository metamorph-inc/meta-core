#include "CodeGenerator.hpp"
#include "BlockTemplate.hpp"
#include "SFCUdmEngine.hpp"
#include "mfile2SFC.hpp"
#include "SLSF.h"
#ifdef _WIN32
#include "Windows.h"
#endif

CodeGenerator::CodeGenerator( void ) : _error( false ) {
	std::string newpath;
	try {
		Py_NoSiteFlag = 1; // we import site after setting up sys.path
		Py_InitializeEx(0);

		char *path = Py_GetPath();
#ifdef _WIN32
		std::string separator = ";";
#else
		std::string separator = ":";
#endif
		newpath = path;

#ifdef USE_CyPhyML
  #define IS_META
#endif
#ifdef USE_CyberComposition
  #define IS_META
#endif

#ifdef IS_META
		std::string metapath;
		HKEY software_meta;
		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\META", 0, KEY_READ, &software_meta) == ERROR_SUCCESS)
		{
			BYTE data[MAX_PATH];
			DWORD type, size = sizeof(data) / sizeof(data[0]);
			if (RegQueryValueExA(software_meta, "META_PATH", 0, &type, data, &size) == ERROR_SUCCESS)
			{
				metapath = std::string(data, data + strnlen((const char*)data, size));
			}
		}
		if (metapath.length())
		{
			newpath += separator + "c:\\Python27\\Lib";
			newpath += separator + metapath + "/externals/HCDDES/src/lib/BlockTemplate/Python"
				+ separator + metapath + "/externals/HCDDES/generated/BlockTemplate/Python";
			newpath += separator + metapath + "/bin/CodeGenPython";
			newpath += separator + metapath + "/bin/Python27/Lib/site-packages";
		}
		else
		{
			MessageBoxA(NULL, "Warning: META_PATH registry value not set. Python initialization may fail.", NULL, MB_OK);
		}
#else		
		// Don't use std::string here, as getenv may return NULL
		char* VCP_PATH = getenv("VCP_PATH");
		if (VCP_PATH == NULL) {
			std::cerr << "Warning: Environment variable VCP_PATH is not set. Python initialization may fail." << std::endl;
		}
		else {
			newpath += separator + VCP_PATH + "/src/lib/BlockTemplate/Python"
				+ separator + VCP_PATH + "/generated/BlockTemplate/Python"
				+ separator + VCP_PATH + "/Python";
		}
#endif

		// PYTHON_HOME is set on developers' machines
		char* PYTHON_HOME = getenv("PYTHON_HOME");
		if (PYTHON_HOME != NULL) {
			// This is where Cheetah lives
			newpath += separator + PYTHON_HOME + "/lib/site-packages";
		}
		newpath += separator + "/usr/share/pyshared";

		PySys_SetPath(const_cast<char*>(newpath.c_str()));


		BlockTemplateInit();
		_blockTemplateModule = boost::python::import( "BlockTemplate" );
		_blockTemplateNameSpace = _blockTemplateModule.attr( "__dict__" );
		boost::python::object main_module = boost::python::import( "__main__" );
		boost::python::object main_namespace = main_module.attr( "__dict__" );
		boost::python::exec(
		 "import sys\n"
		 "import os\n"
		 "try: import site\n" // no site on installed version
		 "except ImportError: pass\n",
		 main_namespace, main_namespace
		);
	} catch(boost::python::error_already_set const &) {
		if (PyErr_Occurred() != NULL)
			PyErr_Print();
        //throw std::runtime_error("Error initializing Python. sys.path=" + newpath);
		throw udm_exception("Error initializing Python. sys.path=" + newpath);
    }
}

boost::python::object CodeGenerator::getPrimitiveNamespace( const std::string &primitiveName ) {
	boost::python::object primitiveNamespace;

	PrimitiveNameModuleMap::iterator pnmItr = _primitiveNameModuleMap.find( primitiveName );
	if ( pnmItr == _primitiveNameModuleMap.end() ) {
		boost::python::object primitiveModule = boost::python::import( primitiveName.c_str() );
		primitiveNamespace = primitiveModule.attr( "__dict__" );
		primitiveNamespace[ "BlockTemplate" ] = _blockTemplateModule;
		primitiveNamespace[ "BasicType" ] = _blockTemplateNameSpace[ "BasicType" ];
		primitiveNamespace[ "Array" ] = _blockTemplateNameSpace[ "Array" ];
		primitiveNamespace[ "Struct" ] = _blockTemplateNameSpace[ "Struct" ];
		_primitiveNameModuleMap.insert(  std::make_pair( primitiveName, primitiveNamespace )  );
	} else {
		primitiveNamespace = pnmItr->second;
	}

	return primitiveNamespace;
}

std::string CodeGenerator::generateMatLabCode(
 const std::string &primitiveName, boost::python::list inputs, boost::python::list outputs, boost::python::dict parameters
) {
	std::string matLabCode;

	try {

		std::string normalizedPrimitiveName = normalizePrimitiveName( primitiveName );

		boost::python::object primitiveNamespace = getPrimitiveNamespace( normalizedPrimitiveName );
		boost::python::object primitiveClass = primitiveNamespace[ normalizedPrimitiveName ];

		boost::python::dict blockDataDict;
		if ( _primitiveBlockSP == 0 ) blockDataDict[ "block" ] = boost::python::object();
		else                          blockDataDict[ "block" ] = *_primitiveBlockSP;

		blockDataDict[ "inputs" ] = inputs;
		blockDataDict[ "input" ] = boost::python::len( inputs ) > 0 ? inputs[0] : boost::python::object();

		blockDataDict[ "outputs" ] = outputs;
		blockDataDict[ "output" ] = boost::python::len( outputs ) > 0 ? outputs[0] : boost::python::object();

		blockDataDict[ "parameters" ] = parameters;

		boost::python::list searchList;
		searchList.append( blockDataDict );

		primitiveNamespace[ "searchList" ] = searchList;

		std::string pythonScript =
		 std::string( "object = " ) + normalizedPrimitiveName + "( searchList=searchList )\n" +
		 "result = object.respond()\n";

		boost::python::exec( pythonScript.c_str(), primitiveNamespace, primitiveNamespace );

		matLabCode = boost::python::extract< std::string >(
		 static_cast< boost::python::str >( primitiveNamespace[ "result" ] ).encode( "ascii" )
		);

	} catch( ... ) {
		matLabCode = "";
		if (  PyErr_ExceptionMatches( PyExc_ImportError )  ) {
			std::cerr << "ERROR:  Problem loading template for block type \"" << primitiveName << "\"" << std::endl;			
			std::cerr << "Skipping \"" << primitiveName << "\" block." << std::endl;
			PyErr_Print();
			_error = true;
		} else {
			std::cerr << "ERROR:  Block \"" << primitiveName << "\":" << std::endl;
			PyErr_Print();
			_error = true;
		}
	}

	return matLabCode;
}

void CodeGenerator::generateSFC( SLSF::Block block ) {

	try {
		BlockSetup blockSetup = getBlockSetup( block );

		std::string primitiveName( blockSetup.getPrimitiveType() );

		_primitiveBlockSP = PrimitiveBlockSP( new PrimitiveBlock(
		 blockSetup.getBlock(), blockSetup.getContext(), blockSetup.getInputs(), blockSetup.getOutputs(), blockSetup.getParameters()
		) );

		boost::python::list inputs = _primitiveBlockSP->getInputs();
		boost::python::list outputs = _primitiveBlockSP->getOutputs();
		boost::python::dict parameters = _primitiveBlockSP->getParameters();

		SFC::Arg contextArg = SFCManager::getFirstFunctionArg( blockSetup.getContext() );
		std::string contextArgName = contextArg.name();
		getSFCSymbolTable().push( contextArgName, contextArg );

		std::string result = generateMatLabCode( primitiveName, inputs, outputs, parameters );

		// MUST BE DONE SEPARATELY -- PRE- AND POST-STATEMENTS ARE GENERATED IN "generateMatLabCode"!!
		result = _primitiveBlockSP->getPreStatements() + result + _primitiveBlockSP->getPostStatements();
		std::cout << "RESULT:\n" << result << std::endl << std::endl;

		mstat2SFC( _primitiveBlockSP->getContext(), result, _primitiveBlockSP->getZeroIndexingFlag(), _primitiveBlockSP->getAcceptCSyntaxFlag() );

		getSFCSymbolTable().clear();

	} catch( std::exception &E ) {

		std::cerr << "Caught std::exception: " << E.what() << std::endl;

	} catch( ... ) {
		std::cerr << "EXCEPTION" << std::endl;
		PyErr_Print();
	}

}
