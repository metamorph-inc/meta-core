/*
 *	Copyright (c) Vanderbilt University, 2006
 *	ALL RIGHTS RESERVED
 *
 *	Vanderbilt University disclaims all warranties with regard to this
 *	software, including all implied warranties of merchantability
 *	and fitness.  In no event shall Vanderbilt University be liable for
 *	any special, indirect or consequential damages or any damages
 *	whatsoever resulting from loss of use, data or profits, whether
 *	in an action of contract, negligence or other tortious action,
 *	arising out of or in connection with the use or performance of
 *	this software.
 *
 *  FILE NAME:	mdl2mga.cpp 
 *	SYNOPSIS:	Main program for translating SL/SF mdl files into GME ESMoL files.					
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */

#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <ctype.h>
#include <matrix.h>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "UdmEngine.hpp"
#include "MatLabEngine.hpp"
#include "MatLabUdmBlock.hpp"
#include "MatLabUdmMiscSimulink.hpp"
#include "MatLabUdmState.hpp"
#include "MatLabUdmTransition.hpp"
#include "MatLabUdmData.hpp"
#include "SLSF.h"
#include "MatLabUdm.hpp"
#include "ConfigKeeper.hpp"

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#include <atlbase.h>
#endif




using namespace SLSF;


typedef std::vector< std::string > DirectoryVector;
typedef std::vector< std::string > MfileVector;
typedef std::vector< std::string > StringVector;

typedef std::set< SLSF::Subsystem > SubsystemSet;
typedef std::set< SLSF::InPort > InPortSet;
typedef std::set< SLSF::OutPort > OutPortSet;


void usage( const std::string &exeName );

#if (PARADIGM == CyberComposition_PARADIGM)

typedef std::set< CyberComposition::Components > ComponentsSet;
typedef std::set< CyberComposition::LibraryFolder > LibraryFolderSet;
typedef std::set< CyberComposition::SimulinkWrapper > SimulinkWrapperSet;

CyberComposition::SimulinkWrapper getComponent( CyberComposition::RootFolder cyberParentObject, const StringVector &stringVector, StringVector::iterator stvItr );
CyberComposition::SimulinkWrapper getComponent( CyberComposition::Components cyberParentObject, const StringVector &stringVector, StringVector::iterator stvItr );
CyberComposition::SimulinkWrapper getComponent( CyberComposition::LibraryFolder cyberParentObject, const StringVector &stringVector, StringVector::iterator stvItr );
CyberComposition::SimulinkWrapper getComponent( CyberComposition::SimulinkWrapper cyberParentObject, const StringVector &stringVector, StringVector::iterator stvItr );

#define FINDDESCENDANT( CLASS )\
	{\
		typedef CLASS##Set CyberObjectSet;\
		CyberObjectSet cyberObjectSet = cyberParentObject.CLASS##_kind_children();\
		CyberObjectSet::iterator cosItr;\
		for( cosItr = cyberObjectSet.begin() ; cosItr != cyberObjectSet.end() ; ++cosItr ) {\
			if (  static_cast< std::string >( cosItr->name() ) == objectName  ) {\
				return getComponent( *cosItr, stringVector, stvItr );\
			}\
		}\
	}

CyberComposition::SimulinkWrapper getComponent( CyberComposition::RootFolder cyberParentObject, const StringVector &stringVector, StringVector::iterator stvItr ) {

	if ( stvItr == stringVector.end() ) {
		std::cerr << "ERROR: Could not find a Cyber Component in path \"" << boost::join( stringVector, "/" ) << "\"" << std::endl;
		return CyberComposition::SimulinkWrapper();
	}
	std::string objectName = *stvItr;

	FINDDESCENDANT( Components )
	FINDDESCENDANT( LibraryFolder )

	std::cerr << "ERROR: Could not find \"" << objectName << "\" in path \"" << boost::join( stringVector, "/" ) << "\"" << std::endl;
	return CyberComposition::SimulinkWrapper();
}

CyberComposition::SimulinkWrapper getComponent( CyberComposition::Components cyberParentObject, const StringVector &stringVector, StringVector::iterator stvItr ) {

	if ( ++stvItr == stringVector.end() ) {
		std::cerr << "ERROR: Could not find a Cyber Component in path \"" << boost::join( stringVector, "/" ) << "\"" << std::endl;
		return CyberComposition::SimulinkWrapper();
	}
	std::string objectName = *stvItr;

	FINDDESCENDANT( SimulinkWrapper )

	std::cerr << "ERROR: Could not find \"" << objectName << "\" in path \"" << boost::join( stringVector, "/" ) << "\"" << std::endl;
	return CyberComposition::SimulinkWrapper();
}

CyberComposition::SimulinkWrapper getComponent( CyberComposition::LibraryFolder cyberParentObject, const StringVector &stringVector, StringVector::iterator stvItr ) {

	if ( ++stvItr == stringVector.end() ) {
		std::cerr << "ERROR: Could not find a Cyber Component in path \"" << boost::join( stringVector, "/" ) << "\"" << std::endl;
		return CyberComposition::SimulinkWrapper();
	}
	std::string objectName = *stvItr;

	FINDDESCENDANT( SimulinkWrapper )

	std::cerr << "ERROR: Could not find \"" << objectName << "\" in path \"" << boost::join( stringVector, "/" ) << "\"" << std::endl;
	return CyberComposition::SimulinkWrapper();
}

CyberComposition::SimulinkWrapper getComponent( CyberComposition::SimulinkWrapper cyberComponent, const StringVector &stringVector, StringVector::iterator stvItr ) {
	if ( ( stvItr + 1 ) != stringVector.end() ) {
		std::cerr << "WARNING:  Cyber Component found at \"" << *stvItr << "\" in path \"" << boost::join( stringVector, "/" ) << "\"" << std::endl;
	}
	return cyberComponent;
}

#endif // PARADIGM == CyberComposition_PARADIGM


void constructModels( MatLabUdm::SmartPointer &matLabUdmParentSP );

#ifdef _WIN32
namespace {
std::string GetLastErrorText() {
	LPTSTR errorText = NULL;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorText, 0, NULL);
	std::string ret;
	if (NULL != errorText) {
		ret = errorText;
		LocalFree(errorText);
	}
	return ret;
}
}
#endif

/***************************************************************************************/

#if (PARADIGM == CyberComposition_PARADIGM )

void BuildCyberCompositionInterface( Udm::Object obj )
	throw ( udm_exception )
{
	std::map< string, string > InputTypes, OutputTypes, ParamTypes;
	InputTypes[ "double" ] = string( "Modelica.Blocks.Interfaces.RealInput" );
	InputTypes[ "single" ] = string( "Modelica.Blocks.Interfaces.RealInput" );
	InputTypes[ "int64" ] = string( "Modelica.Blocks.Interfaces.IntegerInput" );
	InputTypes[ "uint64" ] = string( "Modelica.Blocks.Interfaces.IntegerInput" );
	InputTypes[ "int32" ] = string( "Modelica.Blocks.Interfaces.IntegerInput" );
	InputTypes[ "uint32" ] = string( "Modelica.Blocks.Interfaces.IntegerInput" );
	InputTypes[ "int16" ] = string( "Modelica.Blocks.Interfaces.IntegerInput" );
	InputTypes[ "uint16" ] = string( "Modelica.Blocks.Interfaces.IntegerInput" );
	InputTypes[ "int8" ] = string( "Modelica.Blocks.Interfaces.IntegerInput" );
	InputTypes[ "uint8" ] = string( "Modelica.Blocks.Interfaces.IntegerInput" );
	InputTypes[ "logical" ] = string( "Modelica.Blocks.Interfaces.BooleanInput" );
	InputTypes[ "boolean" ] = string( "Modelica.Blocks.Interfaces.BooleanInput" );

	OutputTypes[ "double" ] = string( "Modelica.Blocks.Interfaces.RealOutput" );
	OutputTypes[ "single" ] = string( "Modelica.Blocks.Interfaces.RealOutput" );
	OutputTypes[ "int64" ] = string( "Modelica.Blocks.Interfaces.IntegerOutput" );
	OutputTypes[ "uint64" ] = string( "Modelica.Blocks.Interfaces.IntegerOutput" );
	OutputTypes[ "int32" ] = string( "Modelica.Blocks.Interfaces.IntegerOutput" );
	OutputTypes[ "uint32" ] = string( "Modelica.Blocks.Interfaces.IntegerOutput" );
	OutputTypes[ "int16" ] = string( "Modelica.Blocks.Interfaces.IntegerOutput" );
	OutputTypes[ "uint16" ] = string( "Modelica.Blocks.Interfaces.IntegerOutput" );
	OutputTypes[ "int8" ] = string( "Modelica.Blocks.Interfaces.IntegerOutput" );
	OutputTypes[ "uint8" ] = string( "Modelica.Blocks.Interfaces.IntegerOutput" );
	OutputTypes[ "logical" ] = string( "Modelica.Blocks.Interfaces.BooleanOutput" );
	OutputTypes[ "boolean" ] = string( "Modelica.Blocks.Interfaces.BooleanOutput" );

	ParamTypes[ "double" ] = string( "Real" );
	ParamTypes[ "single" ] = string( "Real" );
	ParamTypes[ "int64" ] = string( "Integer" );
	ParamTypes[ "uint64" ] = string( "Integer" );
	ParamTypes[ "int32" ] = string( "Integer" );
	ParamTypes[ "uint32" ] = string( "Integer" );
	ParamTypes[ "int16" ] = string( "Integer" );
	ParamTypes[ "uint16" ] = string( "Integer" );
	ParamTypes[ "int8" ] = string( "Integer" );
	ParamTypes[ "uint8" ] = string( "Integer" );
	ParamTypes[ "logical" ] = string( "Boolean" );
	ParamTypes[ "boolean" ] = string( "Boolean" );

	// Cast the target object to a SimulinkWrapper
	CyberComposition::SimulinkWrapper sw = CyberComposition::SimulinkWrapper::Cast( obj );
	cout << "Processing SimulinkWrapper: " << sw.name() << std::endl;

	// Store the Simulink model filename in the SimulinkWrapper
	sw.ModelFilename() = ConfigKeeper::getSingleton().getStringValue( "inputFile" );

	// Get the subsystem from the wrapper
	unsigned long counter = 1;
	set< CyberComposition::Simulink::Subsystem > subs = sw.Simulink_Subsystem_kind_children();
	for ( set< CyberComposition::Simulink::Subsystem >::iterator subIter = subs.begin(); subIter != subs.end(); subIter++ )
	{
		ostringstream buf;
		buf << "_" << counter;
		string suffix( buf.str() );

		cout << "Processing Subsystem: " << (*subIter).name() << std::endl;

		// Get the inner subsystem
		set< CyberComposition::Simulink::Subsystem > inner_subs = (*subIter).Subsystem_kind_children();

		if ( inner_subs.empty() )
		{
			cout << "Failed to find inner subsystem within subsystem " << (*subIter).name() << std::endl;
			continue;
		}

		CyberComposition::Simulink::Subsystem inner_sub = *(inner_subs.begin());

		// Build maps of existing connections
		map< CyberComposition::Simulink::SF_Port, CyberComposition::Simulink::SF_Port > inner_input_ports_map;
		map< CyberComposition::Simulink::SF_Port, CyberComposition::Simulink::SF_Port > inner_output_ports_map;

		set< CyberComposition::Simulink::Line > top_lines = (*subIter).Line_kind_children();
		for ( set< CyberComposition::Simulink::Line >::iterator lineIter = top_lines.begin(); lineIter != top_lines.end(); lineIter++ )
		{
			CyberComposition::Simulink::SF_Port top_end, inner_end;
			CyberComposition::Simulink::SF_Port src_end = (*lineIter).srcLine_end();
			CyberComposition::Simulink::SF_Port dst_end = (*lineIter).dstLine_end();

			Udm::Object src_parent = src_end.parent();
			Udm::Object dst_parent = dst_end.parent();
			if (src_parent.uniqueId() == (*subIter).uniqueId() )
			{
				top_end = (*lineIter).srcLine_end();
				inner_end = (*lineIter).dstLine_end();
			}
			else
			{
				top_end = (*lineIter).dstLine_end();
				inner_end = (*lineIter).srcLine_end();
			}

			if ( inner_end.type() == CyberComposition::Simulink::InputPort::meta )
			{
				inner_input_ports_map[ inner_end ] = top_end;
			}
			else if ( inner_end.type() == CyberComposition::Simulink::OutputPort::meta )
			{
				inner_output_ports_map[ inner_end ] = top_end;
			}

			// Fix missing type reference in existing model
			CyberComposition::Simulink::TypeBaseRef top_tbr = top_end.TypeBaseRef_child();
			
			if ( top_tbr == Udm::null )
			{
				top_tbr = CyberComposition::Simulink::TypeBaseRef::Create( top_end );
			}

			CyberComposition::Simulink::TypeBaseRef tbr = inner_end.TypeBaseRef_child();
			top_tbr.ref() = tbr.ref();
			top_tbr.name() = tbr.name();
			top_tbr.MemberIndex() = tbr.MemberIndex();
			
		}

		// Collect inputs from the inner subsystem
		set< CyberComposition::Simulink::InputPort > inner_inputs = inner_sub.InputPort_kind_children();
		for ( set< CyberComposition::Simulink::InputPort >::iterator inputIter = inner_inputs.begin(); inputIter != inner_inputs.end(); inputIter++ )
		{
			cout << "Visiting inner input " << inner_sub.name() << "/" << (*inputIter).name() << " " << endl;

			// See whether we've already seen this one
			map< CyberComposition::Simulink::SF_Port, CyberComposition::Simulink::SF_Port >::iterator foundIt = inner_input_ports_map.find( CyberComposition::Simulink::SF_Port::Cast( *inputIter ) );
			if ( foundIt != inner_input_ports_map.end() )
				continue;

			cout << "Duplicating input port " << inner_sub.name() << "/" << (*inputIter).name() << endl;

			CyberComposition::Simulink::SF_Port top_end = CyberComposition::Simulink::InputPort::Create( *subIter );
			CyberComposition::Simulink::InputPort inner_end = *inputIter;

			CyberComposition::Simulink::TypeBaseRef tbr = (*inputIter).TypeBaseRef_child();
			// Duplicate the data type into the new port
			CyberComposition::Simulink::TypeBaseRef new_tbr = CyberComposition::Simulink::TypeBaseRef::Create( top_end );

			new_tbr.ref() = tbr.ref();
			new_tbr.name() = tbr.name();
			new_tbr.MemberIndex() = tbr.MemberIndex();

			top_end.name() = string( (*inputIter).name() ) + suffix;
			CyberComposition::Simulink::InputPort top_input = CyberComposition::Simulink::InputPort::Cast( top_end );
			top_input.Number() = (*inputIter).Number();

			// Connect the upper level input to the current input
			CyberComposition::Simulink::Line conn = CyberComposition::Simulink::Line::Create( *subIter );
			conn.srcLine_end() = top_end;
			conn.dstLine_end() = inner_end;

			// Store both ends in the map
			inner_input_ports_map[ CyberComposition::Simulink::SF_Port::Cast( inner_end ) ] 
				= CyberComposition::Simulink::SF_Port::Cast( top_end );
		}

		// Collect outputs from the inner subsystem
		set< CyberComposition::Simulink::OutputPort > inner_outputs = inner_sub.OutputPort_kind_children();
		for ( set< CyberComposition::Simulink::OutputPort >::iterator outputIter = inner_outputs.begin(); outputIter != inner_outputs.end(); outputIter++ )
		{
			cout << "Visiting inner output " << inner_sub.name() << "/" << (*outputIter).name() << " " << endl;

			// See whether we've already seen this one
			map< CyberComposition::Simulink::SF_Port, CyberComposition::Simulink::SF_Port >::iterator foundIt = inner_output_ports_map.find( CyberComposition::Simulink::SF_Port::Cast( *outputIter ) );
			if ( foundIt != inner_output_ports_map.end() )
				continue;

			cout << "Duplicating output port " << inner_sub.name() << "/" << (*outputIter).name() << endl;

			CyberComposition::Simulink::SF_Port top_end = CyberComposition::Simulink::OutputPort::Create( *subIter );
			CyberComposition::Simulink::OutputPort inner_end = *outputIter;

			CyberComposition::Simulink::TypeBaseRef tbr = (*outputIter).TypeBaseRef_child();
			// Duplicate the data type into the new port
			CyberComposition::Simulink::TypeBaseRef new_tbr = CyberComposition::Simulink::TypeBaseRef::Create( top_end );

			new_tbr.ref() = tbr.ref();
			new_tbr.name() = tbr.name();
			new_tbr.MemberIndex() = tbr.MemberIndex();

			top_end.name() = string( (*outputIter).name() ) + suffix;
			CyberComposition::Simulink::OutputPort top_output = CyberComposition::Simulink::OutputPort::Cast( top_end );
			top_output.Number() = (*outputIter).Number();

			// Connect the upper level output to the current output
			CyberComposition::Simulink::Line conn = CyberComposition::Simulink::Line::Create( *subIter );
			conn.srcLine_end() = inner_end;
			conn.dstLine_end() = top_end;

			// Store both ends in the map
			inner_output_ports_map[ CyberComposition::Simulink::SF_Port::Cast( inner_end ) ] 
				= CyberComposition::Simulink::SF_Port::Cast( top_end );
		}

		// Iterate the input map and create wrapper-level input ports
		for ( map< CyberComposition::Simulink::SF_Port, CyberComposition::Simulink::SF_Port >::iterator pairIter = inner_input_ports_map.begin();
				pairIter != inner_input_ports_map.end();  pairIter++ )
		{

			CyberComposition::Simulink::SF_Port top_end = pairIter->second;
			CyberComposition::Simulink::SF_Port inner_end = pairIter->first;

			// Check to see whether the top end port is within a block, or in the subsystem - if block, then skip it
			Udm::Object par = top_end.parent();
			if ( par != *subIter )
				continue;

			// Also skip anything that's not an input port
			if ( top_end.type() != CyberComposition::Simulink::InputPort::meta )
				continue;

			CyberComposition::InputSignalInterface isi = CyberComposition::InputSignalInterface::Create( sw );
			CyberComposition::InputSignalInterfaceConnection conn2 = CyberComposition::InputSignalInterfaceConnection::Create( sw );
			conn2.srcInputSignalInterfaceConnection_end() = isi;
			conn2.dstInputSignalInterfaceConnection_end() = CyberComposition::Simulink::InputPort::Cast( top_end );

			isi.name() = top_end.name();
			cout << "Created top-level input port: " << isi.name() << std::endl;

			CyberComposition::Simulink::TypeBaseRef tbr = inner_end.TypeBaseRef_child();
			CyberComposition::Simulink::SF_TypeBase sft = tbr.ref();
			if ( sft.type() == CyberComposition::Simulink::SF_Matrix::meta )
			{
				CyberComposition::Simulink::SF_Matrix smtx = CyberComposition::Simulink::SF_Matrix::Cast( sft );
				string tbtype = string( smtx.Type() );
				map< string, string >::iterator itIter = InputTypes.find( tbtype );
				if ( itIter != InputTypes.end() )
				{
					isi.Class() = itIter->second; // translated port type
				}
				else
				{
					std::cerr << "ERROR: Could not translate Simulink data type " << tbtype;
					std::cerr << " to a corresponding Modelica port type.  Leaving the Class attribute for port "; 
					std::cerr << isi.name() << " empty." << endl;
				}
			}
			else
			{
				std::cerr << "ERROR: Could not translate Simulink data type";
				std::cerr << " to a corresponding Modelica port type.  Leaving the Class attribute for port "; 
				std::cerr << isi.name() << " empty." << endl;
			}
		}


		// Iterate the output map and create wrapper-level output ports
		for ( map< CyberComposition::Simulink::SF_Port, CyberComposition::Simulink::SF_Port >::iterator pairIter = inner_output_ports_map.begin();
				pairIter != inner_output_ports_map.end();  pairIter++ )
		{

			CyberComposition::Simulink::SF_Port top_end = pairIter->second;
			CyberComposition::Simulink::SF_Port inner_end = pairIter->first;

			// Check to see whether the top end port is within a block, or in the subsystem - if block, then skip it
			Udm::Object par = top_end.parent();
			if ( par != *subIter )
				continue;

			// Also skip anything that's not an output port
			if ( top_end.type() != CyberComposition::Simulink::OutputPort::meta )
				continue;

			// Look up the data type object & translate it to Modelica types
			// Create an output at the top level, and set the type
			// Connect the top level to the upper level
			CyberComposition::OutputSignalInterface osi = CyberComposition::OutputSignalInterface::Create( sw );
			CyberComposition::OutputSignalInterfaceConnection conn2 = CyberComposition::OutputSignalInterfaceConnection::Create( sw );
			conn2.srcOutputSignalInterfaceConnection_end() = CyberComposition::Simulink::OutputPort::Cast( top_end );
			conn2.dstOutputSignalInterfaceConnection_end() = osi;

			osi.name() = top_end.name();
			cout << "Created top-level output port: " << osi.name() << std::endl;

			CyberComposition::Simulink::TypeBaseRef tbr = inner_end.TypeBaseRef_child();
			CyberComposition::Simulink::SF_TypeBase sft = tbr.ref();
			if ( sft.type() == CyberComposition::Simulink::SF_Matrix::meta )
			{
				CyberComposition::Simulink::SF_Matrix smtx = CyberComposition::Simulink::SF_Matrix::Cast( sft );
				string tbtype = string( smtx.Type() );
				map< string, string >::iterator otIter = OutputTypes.find( tbtype );
				if ( otIter != OutputTypes.end() )
				{
					osi.Class() = otIter->second; // translated port type
				}
				else
				{
					std::cerr << "ERROR: Could not translate Simulink data type " << tbtype;
					std::cerr << " to a corresponding Modelica port type.  Leaving the Class attribute for port "; 
					std::cerr << osi.name() << " empty." << endl;
				}
			}
			else
			{
				std::cerr << "ERROR: Could not translate Simulink data type";
				std::cerr << " to a corresponding Modelica port type.  Leaving the Class attribute for port "; 
				std::cerr << osi.name() << " empty." << endl;
			}
		}

		counter++;
	}
}


#endif

#define MAX_KEY_LENGTH 255 // size limit for registry key
#define MAX_VALUE_NAME 16383 // size limit for the name of a value

/* Summary: Loads the registry key specified by the matlabKey parameter and enumerates through the values.
   If any of the values are named "MATLABROOT", then the value of this value is retrieved,
   and either "\bin\win32" or \bin\win64" is appended, depending on the architecture.
   We then check whether the files "libeng.dll" and "libmx.dll" exist in this directory.
   If so, we attemp to load them using LoadLibraryA. If successful, we return 0, otherwise, -1.
   Returns: 0 if libeng.dll and libmx.dll, the two MATLAB .dll files needed by MDL2MGA, can
   be loaded. -1 otherwise. */
int tryLoadLibraries(std::string matlabKey)
{
	USES_CONVERSION;
	DWORD i, retCode;
	HKEY hKey;

    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys=0;               // number of subkeys 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
 
    TCHAR  achValue[MAX_VALUE_NAME]; 
    DWORD cchValue = MAX_VALUE_NAME;
 
	retCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE, matlabKey.c_str(), 0, KEY_READ, &hKey);

	if (retCode != ERROR_SUCCESS) {
		std::cerr << "Warning: could not open registry key " << matlabKey << std::endl;
		return -1;
	}


	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		NULL,                // buffer for class name 
		NULL,           // size of class string 
		NULL,                    // reserved 
		NULL,               // number of subkeys 
		NULL,            // longest subkey size 
		NULL,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		NULL,   // security descriptor 
		NULL);       // last write time

	if (retCode != ERROR_SUCCESS) {
		std::cerr << "Warning: opened key, but could not query key info for HKLM\\" << matlabKey << std::endl;
		return -1;
	}

	if (cValues) 
	{
		for (i=0, retCode=ERROR_SUCCESS; i < cValues; i++) 
		{ 
			cchValue = MAX_VALUE_NAME; 
			achValue[0] = '\0'; 
			retCode = RegEnumValue(hKey, i,
				achValue, 
				&cchValue, 
				NULL, 
				NULL,
				NULL,
				NULL);

			if (retCode != ERROR_SUCCESS) {
				std::cerr << "Warning: obtained key info, but could not enumerate through values for HKLM\\" << matlabKey << std::endl;
				return -1;
			}

			std::string valName(T2A(achValue));
			if (valName.compare("MATLABROOT") == 0) {
				BYTE pData[10000] = "\0";
				DWORD pDataSize = sizeof(pData);

				// use the ANSI version: RegGetValueA
				retCode = RegGetValueA(hKey, NULL, TEXT("MATLABROOT"), RRF_RT_ANY, NULL, pData, &pDataSize);
				if (retCode != ERROR_SUCCESS) {
					std::cerr << "Warning: the registry key HKLM\\" << matlabKey << " contains a value named MATLABROOT, "
						      << "but its value could not be read." << std::endl;
					continue;
				}

				std::string libengPath((char*)pData);
				std::string libmxPath((char*)pData);

				std::string subDir;

#ifdef _WIN64
				subDir = "\\bin\\win64";
#else
				subDir = "\\bin\\win32";
#endif
				libengPath.append(subDir);
				libmxPath.append(subDir);
				libengPath.append("\\libeng.dll");
				libmxPath.append("\\libmx.dll");
						
				DWORD libengAttrib= GetFileAttributes(A2T(const_cast<char*>(libengPath.c_str())));
				DWORD libmxAttrib= GetFileAttributes(A2T((const_cast<char*>(libmxPath.c_str()))));

				if (libengAttrib != INVALID_FILE_ATTRIBUTES && 
					libmxAttrib != INVALID_FILE_ATTRIBUTES)
				{
					HMODULE libeng = LoadLibraryA(libengPath.c_str());
					HMODULE libmx = LoadLibraryA(libmxPath.c_str());

					if (libeng != NULL &&
						libmx  != NULL)
					{
						return 0;
					}
				}
			}
		}
	}

	return -1;
}

int checkForMatlab()
{
	std::string baseKey = "SOFTWARE\\MathWorks\\MATLAB";

	USES_CONVERSION;
	DWORD i, retCode;
	HKEY hKey;

	// Don't use KEY_WOW64_64KEY explicitly: this should be handled appropriately depending on 32 or 64-bit MDL2MGA
	retCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE, baseKey.c_str(), 0, KEY_READ, &hKey);
	if (retCode != ERROR_SUCCESS) {
		std::cerr << "Warning: could not open registry key HKLM\\SOFTWARE\\MathWorks\\MATLAB" << std::endl;
		return -1;
	}

    TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName;                   // size of name string 
    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys=0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    cValues;              // number of values for key 
 
    DWORD cchValue = MAX_VALUE_NAME; 
 
    // Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        NULL,            // longest value name 
        NULL,         // longest value data 
        NULL,   // security descriptor 
        NULL);       // last write time 
 
	if (retCode != ERROR_SUCCESS) {
		std::cerr << "Warning: could not query key info for HKLM\\SOFTWARE\\MathWorks\\MATLAB" << std::endl;
		return -1;
	}
    
    if (cSubKeys) {
        for (i = 0; i < cSubKeys; i++)
        { 
            cbName = MAX_KEY_LENGTH;
            retCode = RegEnumKeyEx(hKey, i,
                     achKey,
                     &cbName,
                     NULL, 
                     NULL, 
                     NULL, 
                     NULL);

			if (retCode != ERROR_SUCCESS) {
				std::cerr << "Warning: could not enumerate the subkeys of HKLM\\SOFTWARE\\MathWorks\\MATLAB" << std::endl;
				return -1;
			}

			std::string matlabKeyName = baseKey;
			matlabKeyName.append("\\");
			matlabKeyName.append(T2A(achKey));

			if (tryLoadLibraries(matlabKeyName) == 0) {
				return 0;
			}
        }
    }

	std::cerr << "Warning: could not locate libeng.dll and libmx.dll using the "
			  << "MATLAB path found in the registry." << std::endl;
	return -1;
}

/***************************************************************************************/

int main( int argc, char *argv[] ) {
#ifdef _WIN32
	if (checkForMatlab()) {
		HMODULE libeng = LoadLibraryA("libeng.dll");
		HMODULE libmx = LoadLibraryA("libmx.dll");
		if (libeng == NULL || libmx == NULL)
		{
			std::string missing_dll;
			if (libeng == NULL)
				missing_dll = "libeng.dll";
			if (libmx == NULL)
				missing_dll = "libmx.dll";
			HMODULE loaded_datafile = LoadLibraryExA(missing_dll.c_str(), NULL, LOAD_LIBRARY_AS_DATAFILE);
	#if defined(_M_AMD64)
			const char* arch = "Win64";
	#elif defined(_M_IX86)
			const char* arch = "Win32";
	#else
	#error Unrecognized processor
	#endif
			std::cerr << "Error: could not load " << missing_dll << ".\n";
			if (loaded_datafile == NULL) {
	#pragma warning(push)
	#pragma warning(disable: 4996)
				std::cerr << "Please confirm you have Matlab installed and " << missing_dll << " is on the %PATH%\n"
					<< "Current %PATH% is " << (getenv("PATH") ? getenv("PATH") : "empty") << std::endl;
	#pragma warning(pop)
			} else {
				std::cerr << "Please confirm " << missing_dll << " is of the correct architecture. "
					"(This is " << arch << " MDL2MGA)" << std::endl;
			}
			return ERROR_MOD_NOT_FOUND;
		}
	}
#endif /* WIN32 */

	int exit_status = 0;
	// Try to process the command line arguments
	if ( !ConfigKeeper::getSingleton().processCommandLineArguments( argc, argv ) ) {
		return 0;
	}
	// Get the input file name
	if (  !ConfigKeeper::getSingleton().getCount( "inputFile" )  ) {
		usage( argv[0] );
		return 1;
	}
	std::string matLabFilename = ConfigKeeper::getSingleton().getStringValue( "inputFile" );
	boost::filesystem::path matLabFile( matLabFilename );

#if BOOST_VERSION / 100 % 1000 < 46
	matLabFile = boost::filesystem::complete( matLabFile );
#else
	matLabFile = boost::filesystem::absolute( matLabFile );
#endif
	// CHECK IF INPUT .mdl FILE EXISTS
	if (  !boost::filesystem::exists( matLabFile )  ) {
		std::cerr << "ERROR: Could not access file \"" << matLabFile << "\"." << std::endl << std::endl;
		return 2;
	}

	boost::filesystem::path matLabFile_mdl2mga;
	try {
		matLabFile_mdl2mga = MatLabUdm::getTopLevelModelManager().registerModel( matLabFile );
	} catch (const boost::filesystem::filesystem_error& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
		return 2;
	}

#if BOOST_VERSION / 100 % 1000 < 46
	std::string udmFilename = ConfigKeeper::getSingleton().getCount( "outputFile" ) ?
	 ConfigKeeper::getSingleton().getStringValue( "outputFile" ) : boost::filesystem::change_extension( matLabFile, ".xml" ).string();
#else
	std::string udmFilename = ConfigKeeper::getSingleton().getCount( "outputFile" ) ?
	 ConfigKeeper::getSingleton().getStringValue( "outputFile" ) : boost::filesystem::change_extension( matLabFile, boost::filesystem::path( ".xml" ) ).string();
#endif

	if (  ConfigKeeper::getSingleton().getCount( "help" )  ) {
		usage( argv[0] );
		return 0;
	}

#if PARADIGM != CyberComposition_PARADIGM
	bool newUdmFile = false;
	if (  ConfigKeeper::getSingleton().getCount( "newfile" )  ) newUdmFile = true;
#endif

	std::string host = "";
	if (  ConfigKeeper::getSingleton().getCount( "host" )  ) {
		host = ConfigKeeper::getSingleton().getStringValue( "host" );
	}

	if (  ConfigKeeper::getSingleton().getCount( "toplevel" )  ) {
		MatLabUdm::onlyTopLevel = true;
	}

	DirectoryVector directoryVector;
	if (  ConfigKeeper::getSingleton().getCount( "libdir" )  ) {
		directoryVector = ConfigKeeper::getSingleton().getStringVector( "libdir" );
	}

	MfileVector mfileVector;
	if (  ConfigKeeper::getSingleton().getCount( "mfile" )  ) {
		mfileVector = ConfigKeeper::getSingleton().getStringVector( "mfile" );
		for( MfileVector::iterator mfvItr = mfileVector.begin() ; mfvItr != mfileVector.end() ; ++mfvItr ) {
			std::string &mfileName = *mfvItr;
			if (  mfileName.substr( mfileName.length() - 2 ) == ".m"  ) mfileName = mfileName.substr(  0, mfileName.length() - 2  );
		}
	}

#if PARADIGM == CyberComposition_PARADIGM
	if (  !ConfigKeeper::getSingleton().getCount( "path" )  ) {
		std::cerr << "For CyPhyML model, \"path <path-to-component>\" option MUST be specified." << std::endl;
		return 1;
	}

	std::string componentPath = ConfigKeeper::getSingleton().getStringValue( "path" );
	boost::trim( componentPath );
	if ( componentPath.empty() ) {
		std::cerr << "Path to component MUST NOT BE EMPTY." << std::endl;
		return 1;
	}
#endif

	// INITIALIZE THE DATA NETWORK
	try {
#if PARADIGM == CyberComposition_PARADIGM
		UdmEngine::get_singleton().open( udmFilename );
#else
		if ( newUdmFile ) UdmEngine::get_singleton().createNew( udmFilename );
		else              UdmEngine::get_singleton().open( udmFilename );
#endif

		// INITIALIZE MatLab ENGINE
		MatLabEngine::initGlobalEngine( host );

		if (  ConfigKeeper::getSingleton().getCount( "matlabLog" )  ) {
			std::string matLabLogFileName = ConfigKeeper::getSingleton().getStringValue( "matlabLog" );
			MatLabEngine::globalEngine().setLoggingEnabled( matLabLogFileName );
		}

		MatLabEngine::globalEngine().executeCommand( "clear all" );
		MatLabEngine::globalEngine().executeCommand( "sfpref( 'ignoreUnsafeTransitionActions', 1 )" );

		// ADD DIRECTORY OF INPUT FILE TO LIBRARY SEARCH PATH IN MATLAB
		MatLabEngine::globalEngine().executeCommand(  std::string( "addpath( '" ) + matLabFile.parent_path().string() + "' )"  );

		for( DirectoryVector::iterator drvItr = directoryVector.begin() ; drvItr != directoryVector.end() ; ++drvItr ) {
			boost::filesystem::path directory( *drvItr );
#if BOOST_VERSION / 100 % 1000 < 46
			MatLabEngine::globalEngine().executeCommand(  std::string( "addpath( '" ) + boost::filesystem::complete( directory ).string() + "' )"  );
#else
			MatLabEngine::globalEngine().executeCommand(  std::string( "addpath( '" ) + boost::filesystem::absolute( directory ).string() + "' )"  );
#endif
		}

		for( MfileVector::iterator mfvItr = mfileVector.begin() ; mfvItr != mfileVector.end() ; ++mfvItr ) {
			MatLabEngine::globalEngine().executeCommand( *mfvItr );			
		}

#ifdef _WIN32
		DWORD tmpSize = GetTempPath(0, 0);
		TCHAR* tmpPath = new TCHAR[tmpSize+1];
		if (GetTempPath(tmpSize+1, tmpPath) == 0) {
			delete[] tmpPath;
			std::string error = "Error calling GetTempPath: ";
			error += GetLastErrorText();
			throw std::runtime_error(error);
		}
		std::string cmd = "cd ";
		cmd += std::string("'") + tmpPath + "'";
		delete[] tmpPath;
		MatLabEngine::globalEngine().executeCommand( cmd );
#else
		MatLabEngine::globalEngine().executeCommand( "cd /tmp" );
#endif
		MatLabEngine::globalEngine().loadFile( matLabFile_mdl2mga.string() );
		MatLabEngine::globalEngine().executeCommand( "r = sfroot" );

		// TO ALLOW COMPILATION
		MatLabEngine::globalEngine().executeCommand( "r.SaveFormat = 'Structure'" );

#if BOOST_VERSION / 100 % 1000 < 46
		UdmEngine::get_singleton().setModelName( matLabFile.stem() );
#else
		UdmEngine::get_singleton().setModelName( matLabFile.stem().string() );
#endif

#if ( PARADIGM == CyberComposition_PARADIGM )
		StringVector stringVector;
		boost::split(  stringVector, componentPath, boost::is_any_of( "/\\" )  );

		CyberComposition::RootFolder rootFolder = UdmEngine::get_singleton().getRootFolder();
		CyberComposition::SimulinkWrapper rootComponent = getComponent( rootFolder, stringVector, stringVector.begin() );

		if ( rootComponent == Udm::null ) return 1;

		UdmEngine::get_singleton().setDataflowParent( rootComponent );

		MatLabUdm::SmartPointer factoryDataflow = MatLabUdmRoot::Factory().create( UdmEngine::get_singleton().getRootComponent() );
#else
		MatLabUdm::SmartPointer factoryDataflow = MatLabUdmRoot::Factory().create( UdmEngine::get_singleton().getDataflow() );
#endif
//		MatLabEngine::globalEngine().executeCommand( "root_diagram = r.find('-depth', 1, 'name', '" + MatLabEngine::globalEngine().getModelName() + "')" );
//		MatLabEngine::globalEngine().executeCommand("root_diagram.ModelWorkspace.save('mdl2mga_modelworkspace.mat')");
//		MatLabEngine::globalEngine().executeCommand("load('mdl2mga_modelworkspace.mat')");

		// It seems masks' boolean parameters are initialized to 'off' or 'on'. create aliases
//		MatLabEngine::globalEngine().executeCommand("off = false");
//		MatLabEngine::globalEngine().executeCommand("on = true");

		constructModels( factoryDataflow );

		MatLabUdmTransition::getTransitionParents().buildTransitions();

		// TYPE DETERMINATION
		try {
			MatLabUdm::getTopLevelModelManager().saveModels();
			MatLabUdm::getTopLevelModelManager().compileModels();
			MatLabUdmData::getAllDataTypes();
			MatLabUdmPort::getAllPortTypes();
			MatLabUdmBlock::getAllPortTypes();
			MatLabUdmBlock::getAllSampleTimes();
			std::cerr << MatLabUdmBlock::checkAllValues();
			MatLabUdm::getTopLevelModelManager().stopCompileModels();
			MatLabUdm::getTopLevelModelManager().saveModels();
		} catch( ... ) {
			std::cerr << "CAUGHT EXCEPTION ACQUIRING TYPES!!" << std::endl;
			MatLabUdm::getTopLevelModelManager().stopCompileModels();
			MatLabUdm::getTopLevelModelManager().saveModels();
			throw;
		}

		MatLabUdmBlock::kludgeMFileSFuncs();

		MatLabUdmBlock::processGotoFromBlocks();

		MatLabUdmBlock::propagateTypes();
		MatLabUdmBlock::propagateArchetypeOutPorts();

		MatLabUdmBlock::processCollectors();

		MatLabUdmBlock::checkTypes();

#if (PARADIGM == CyberComposition_PARADIGM)

		/*SubsystemSet subsystemSet = rootComponent.Simulink_Subsystem_kind_children();
		SLSF::Subsystem subsystem;
		for( SubsystemSet::iterator sssItr = subsystemSet.begin() ; sssItr != subsystemSet.end() ; ++sssItr ) {
			if ( !sssItr->HasObjectInstances() ) {
				subsystem = *sssItr;
				break;
			}
		}

		if ( subsystem == Udm::null ) {
			std::cerr << "ERROR: Could not find Subsystem to link to SimulinkWrapper model!" << std::endl;
		} else {
			InPortSet inPortSet = subsystem.InPort_kind_children();
			for( InPortSet::iterator ipsItr = inPortSet.begin() ; ipsItr != inPortSet.end() ; ++ipsItr ) {
				SLSF_ROOT::InputSignalInterface inSignal = SLSF_ROOT::InputSignalInterface::Create( rootComponent );
				inSignal.name() = (*ipsItr).name();

				SLSF_ROOT::InputSignalInterfaceConnection inSignal2inPort = SLSF_ROOT::InputSignalInterfaceConnection::Create( rootComponent ); 
				inSignal2inPort.srcInputSignalInterfaceConnection_end() = inSignal;
				inSignal2inPort.dstInputSignalInterfaceConnection_end() = *ipsItr;
			}
			OutPortSet outPortSet = subsystem.OutPort_kind_children();
			for( OutPortSet::iterator opsItr = outPortSet.begin() ; opsItr != outPortSet.end() ; ++opsItr ) {
				SLSF_ROOT::OutputSignalInterface outSignal = SLSF_ROOT::OutputSignalInterface::Create( rootComponent );
				outSignal.name() = (*opsItr).name();

				SLSF_ROOT::OutputSignalInterfaceConnection outPort2outSignal = SLSF_ROOT::OutputSignalInterfaceConnection::Create( rootComponent );
				outPort2outSignal.srcOutputSignalInterfaceConnection_end() = *opsItr;
				outPort2outSignal.dstOutputSignalInterfaceConnection_end() = outSignal;
			}
		}*/

		std::cout << "Building component interface..." << std::endl;
		BuildCyberCompositionInterface( rootComponent );
#endif
	} catch ( udm_exception &err ) {

		std::cerr << "The following UDM exception occurred:" << std::endl;
		std::cerr << err.what() << std::endl << std::endl;

		int length = (int) udmFilename.length();
		if (  udmFilename.substr( length - 3, length ) == "mga"  ) {
			std::cerr << "As you are using the GME backend of UDM, please make sure that the" << std::endl;
			std::cerr << "ESMoL paradigm is properly registered in GME." << std::endl << std::endl;
		}
		exit_status = 1;

	} catch ( MatLabUdm::Exception &err ) {

		std::cerr << "The following MatLab exception occurred:" << std::endl;
		std::cerr << err.what() << std::endl << std::endl;

		exit_status = 2;

	} catch ( std::exception &err ) {

		std::cerr << "The following std::exception occurred:" << std::endl;
		std::cerr << err.what() << std::endl << std::endl;
		exit_status = 3;

	} catch ( ... ) {

		std::cerr << "Unknown exception caught" << std::endl;
		exit_status = 4;
	}

	try {

#if defined(LINUX) && defined(DEVEL)
//		MatLabEngine::globalEngine().executeInteractiveCommands();
#endif
		UdmEngine::close();
		MatLabUdm::getTopLevelModelManager().deleteModels();

	} catch ( udm_exception &err ) {

		std::cerr << "The following UDM exception occurred on closing the UDM file \"" << udmFilename << "\":" << std::endl;
		std::cerr << err.what() << std::endl << std::endl;

		exit_status = 5;

	} catch( ... ) {

		std::cerr << "Unknown exception caught on close of UDM file \"" << udmFilename << "\"." << std::endl;
		exit_status = 6;
	}

	return exit_status;
}


void usage( const std::string &exeName ) {

	std::string::size_type lastForwardSlash = exeName.rfind("/");
//	std::cerr << "lastForwardSlash = " << lastForwardSlash << std::endl;
	std::string::size_type lastBackSlash = exeName.rfind("\\");
//	std::cerr << "lastBackSlash = " << lastBackSlash << std::endl;

	std::string::size_type lastSlash = lastForwardSlash == std::string::npos ? lastBackSlash : lastForwardSlash;
	std::string baseName = exeName.substr( lastSlash + 1 );

	std::cerr << "Usage:" << std::endl;
	std::cerr << ConfigKeeper::getSingleton().getDescription();
	std::cerr << baseName << " [options] <mdl-file> [ <mga-file> | <xml-file> ]" << std::endl << std::endl;
	std::cerr << "Required arguments:" << std::endl;
	std::cerr << "  <mdl-file> is the name of a MatLab .mdl file." << std::endl << std::endl;
	std::cerr << "  If the second argument is not specified, it defaults to a file that" << std::endl;
	std::cerr << "   has the same name as the .mdl file, but ending in .xml -- see below" << std::endl;
	std::cerr << "   for a description of this argument." << std::endl << std::endl;
	std::cerr << "Optional arguments:" << std::endl;
	std::cerr << "  <mga-file> is the name of an GME-based .mga model file that will be" << std::endl;
	std::cerr << "   created or overwritten, and will conform to the ESMoL paradigm in GME." << std::endl << std::endl;
	std::cerr << "  <xml-file> is the name of an XML-based (.xml) model file that will be" << std::endl;
	std::cerr << "   created or overwritten, and will conform to the ESMoL paradigm as" << std::endl;
	std::cerr << "   specified in the ESMoL.xsd XML schema file.  This schema file is embedded" << std::endl;
	std::cerr << "   in this program and so does not have to be explicitly specified." << std::endl << std::endl;
	std::cerr << "  <directory> is the name of a directory that is given to MatLab to search for" << std::endl;
	std::cerr << "   SL/SF library files.  Such library files may be used by the <mdl-file> in" << std::endl;
	std::cerr << "   specification of its SL/SF model.  As many of these directories may be" << std::endl;
	std::cerr << "   specified as needed, though each with its own '-L' option specifier." << std::endl;
	std::cerr << "   NOTE:  the directory in which the <mdl-file> resides is always implicitly" << std::endl;
	std::cerr << "   specified by this program as a library search path to MatLab." << std::endl << std::endl;
	std::cerr << "  <host> is the hostname or ip-address of a machine with a MatLab installation." << std::endl;
	std::cerr << "   MDL2MGA will direct this machine to process the <mdl-file>, and will then query" << std::endl;
	std::cerr << "   this same machine about the model therein.  By default, MDL2MGA uses the local machine." << std::endl << std::endl;
}

void constructModels( MatLabUdm::SmartPointer &matLabUdmParentSP ) {

	MatLabUdm::SPVector recurseVector;

	// GET MODELS
	recurseVector = MatLabUdm::buildUdm( matLabUdmParentSP, MatLabUdmModel::Factory() );
	for( MatLabUdm::SPVector::iterator spvItr = recurseVector.begin() ; spvItr != recurseVector.end() ; ++spvItr ) {
		MatLabUdmBlock::constructBlocks( *spvItr );
		(*spvItr)->cleanup();
	}

}


/***************************************************************************************/

