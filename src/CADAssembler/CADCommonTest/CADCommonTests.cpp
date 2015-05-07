// MathTrigLib.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include "isis_application_exception.h"
#include "TestFunctions.h"
#include "cppunit\XmlOutputter.h"
#include <fstream>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPPUNIT_TEST_SUITE_REGISTRATION( Tests );

int _tmain(int argc, _TCHAR* argv[])
{
	std::ofstream o("cadcommontestresult.xml");
	try
	{
		// Get the top level suite from the registry
		CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

		// Adds the test to the list of test to run
		CppUnit::TextUi::TestRunner runner;
		runner.addTest( suite );

		// Change the default outputter to a compiler error format outputter
		/*runner.setOutputter( new CppUnit::CompilerOutputter( &runner.result(),
															std::cerr ) );*/

		runner.setOutputter( new CppUnit::XmlOutputter( &runner.result(),
															o ) );

		// Run the tests.
		bool wasSucessful = runner.run();


		o.close();

		// Return error code 1 if the one of test failed.
		return wasSucessful ? 0 : -11;


	}
	catch ( isis::application_exception& ex )
	{
		o.close();
		return -1;
	}
} 