#include "stdafx.h"
#include "UdmApp.h"
#include "UdmConfig.h"
#include "Uml.h"
#include "UdmUtil.h"

#include "UdmConsole.h"
#include "CyPhyToolbox.h"
#include "boost\filesystem.hpp"

using namespace std;

/*********************************************************************************/
/* Initialization function. The framework calls it before preparing the backend. */
/* Initialize here the settings in the config global object.					 */
/* Return 0 if successful.														 */
/*********************************************************************************/
int CUdmApp::Initialize()
{
	return 0;
}



/* 
Remarks to CUdmApp::UdmMain(...):
0.	The p_backend points to an already open backend, and the framework 
	closes it automatically. DO NOT OPEN OR CLOSE IT!
	To commit changes use p_backend->CommitEditSequence().
	To abort changes use p_backend->AbortEditSequence().
	To save changes to a different file use p_backend->SaveAs() or p_backend->CloseAs().

1.	Focus is the currently open model.

2.	The possible values for param (from GME DecoratorLib.h component_startmode_enum):
	GME_MAIN_START			=   0,
	GME_BROWSER_START		=   1,
	GME_CONTEXT_START		=   2,
	GME_EMBEDDED_START		=   3,
	GME_MENU_START			=  16,
	GME_BGCONTEXT_START		=  18,
	GME_ICON_START			=  32,
	METAMODEL_CHECK_SYNTAX	= 101

 3. The framework catches all the exceptions and reports the error in a message box,
	clean up and close the transactions aborting the changes. You can override this 
	behavior by catching udm_exception. Use udm_exception::what() to form an error 
	message.
*/

/***********************************************/
/* Main entry point for Udm-based Interpreter  */
/***********************************************/

void CUdmApp::UdmMain(
					 Udm::DataNetwork* p_backend,		// Backend pointer(already open!)
					 Udm::Object focusObject,			// Focus object
					 std::set<Udm::Object> selectedObjects,	// Selected objects
					 long param)						// Parameters
{	
	{
		// GMEConsole::Console::writeLine("Inside FormulaEvaluator", MSG_INFO);

		if (focusObject == Udm::null)
		{
			std::string msg = "No model open. Please open a Component, TestComponent, ComponentAssembly, or Testbench and try again.";

			throw udm_exception(msg);
		}

		if (focusObject.isLibObject() == true)
		{
			std::string msg = "Cannot run FormulaEvaluator on Library Objects.";

			throw udm_exception(msg);
		}

		NewTraverser traverser;
		traverser.traceability = traceability;
		traverser.Traverse(focusObject);
		numericLeafNodes = traverser.numericLeafNodes;

		string projectRoot;
		if (projectDir_SOT != "")
			projectRoot = projectDir_SOT;
		else
			projectRoot = projectDir.substr(string("MGA=").length()).substr(0, projectDir.find_last_of('\\') - string("MGA=").length() + 1);

		if (OutputDir != "" && DoNotGeneratePostProcessing == false)
		{
			GeneratePostProcessingPython(focusObject, projectRoot);
		}

	}
}

// META-2961
void CUdmApp::CopyDirectory(string src,
							string dest)
{
	namespace bfs = boost::filesystem;
	bfs::path srcPath(src);
	bfs::path dstPath(dest);

	try
	{
		if (!bfs::exists(srcPath) ||
			!bfs::is_directory(srcPath))
			return;

		if (bfs::exists(dstPath))
			return;

		if(!bfs::create_directory(dstPath))
			return;

		for(bfs::directory_iterator file(srcPath); file != bfs::directory_iterator(); ++file)
		{
			bfs::path current(file->path());
            if(bfs::is_directory(current))
            {
                // Found directory: Recursion
				CopyDirectory(current.string(),
							  (dstPath / current.filename()).string());

            }
            else
            {
                bfs::copy_file(
                    current,
                    dstPath / current.filename()
                );
            }
		}
	}
	catch(bfs::filesystem_error const & e)
    {
		GMEConsole::Console::writeLine("Failed to copy post process common directory - " + string(e.what()), MSG_WARNING);
        return;
    }

}

void CUdmApp::GeneratePostProcessingPython(const Udm::Object &focusObject_in,
										   string projectRootDir_in)
{
	if (focusObject_in != Udm::null && 
		focusObject_in.type().name() == "TestBench")
	{		

		CyPhyML::TestBench test_bench = CyPhyML::TestBench::Cast(focusObject_in);
		set<CyPhyML::PostProcessing> post_process_set_in = test_bench.PostProcessing_children();
		vector<string> scripts_to_copy;
		set<string> common_script_dirs;

		ofstream outputFile;
		outputFile.open(OutputDir + "/main_post_process.py");

		/*
		for line in open('main_post_process.py', 'rt'): print 'outputFile << "' + line[:-1].replace("\\", "\\\\") + '" << endl;'
		*/
		outputFile << "import os" << endl;
		outputFile << "import sys" << endl;
		outputFile << "import subprocess" << endl;
		outputFile << "" << endl;
		outputFile << "def get_venv_python():" << endl;
		outputFile << "    if os.name == 'nt':" << endl;
		outputFile << "        import _winreg as wr" << endl;
		outputFile << "        try:" << endl;
		outputFile << "            # try to obtain META_PATH from the registry" << endl;
		outputFile << "            key = wr.OpenKey(wr.HKEY_LOCAL_MACHINE, r'software\\meta', 0, wr.KEY_READ)" << endl;
		outputFile << "            meta_path = wr.QueryValueEx(key, 'META_PATH')[0]" << endl;
		outputFile << "            return os.path.join(meta_path, r'bin\\Python27\\Scripts\\python.exe')" << endl;
		outputFile << "        except WindowsError:" << endl;
		outputFile << "            sys.stderr.write('Could not find META_PATH in registry, attempting to use default python...\\n')" << endl;
		outputFile << "            return 'python'" << endl;
		outputFile << "    elif os.name == 'posix':" << endl;
		outputFile << "        return '~/openmdao-0.2.7/bin/python'" << endl;
		outputFile << "    else:" << endl;
		outputFile << "        sys.stderr.write('ERROR: Only Windows or Linux supported.\\n')" << endl;
		outputFile << "        sys.exit(5)" << endl;
		outputFile << "" << endl;
		outputFile << "def run_in_venv(script, args):" << endl;
		outputFile << "    subprocess.check_call([get_venv_python(), script] + args)" << endl;
		outputFile << "" << endl;
		outputFile << "if __name__ == '__main__':" << endl;
		outputFile << "    if len(sys.argv) < 2:" << endl;
		outputFile << "        sys.stderr.write('Usage: %s testbench_manifest.json\\n' % sys.argv[0])" << endl;
		outputFile << "        sys.exit(1)" << endl;

		int i = 0;
		for (set<CyPhyML::PostProcessing>::const_iterator ci = post_process_set_in.begin(); ci != post_process_set_in.end(); ci++)
		{
			i++;
			string scriptPath = (string)ci->ScriptPath();
			if (scriptPath.find("/") == string::npos && scriptPath.find("\\") == string::npos) // script lives in virtual env
			{
				outputFile << "    print 'calling script'" << endl;
				char buffer[256];
				sprintf_s(buffer, "log%i.log", i);
				//outputFile << "    with open('" << buffer << "', 'wb') as logfile:" << endl;
				//outputFile << "        subprocess.check_call([get_venv_python(), '-m', r'" << scriptPath << "'] + sys.argv[1:], stdout=logfile, stderr=logfile)" << endl;
				outputFile << "    subprocess.check_call([get_venv_python(), '-m', r'" << scriptPath << "'] + sys.argv[1:])" << endl;
			}
			else
			{
				string fullpath = projectRootDir_in + "//" + scriptPath;
				boost::filesystem::path p(fullpath);
				fullpath = p.make_preferred().string();

				if (boost::filesystem::exists(p))
				{
					outputFile << "    run_in_venv(r'" <<  p.make_preferred().filename().string() << "', sys.argv[1:])" << endl;
					scripts_to_copy.push_back(fullpath);
				}
				else
					outputFile << "    print 'Script (" << p.make_preferred().filename().string() << ") does not exist.'" << endl;
			}
		}

		if (post_process_set_in.empty())
		{
			outputFile << "    print 'No post processing in the model. Nothing to be done.'" << endl;
		}
		else
		{
			outputFile << "    print 'Post processing is done.'" << endl;
		}

		outputFile.close();

		if (!scripts_to_copy.empty())
		{
			for (unsigned int i = 0; i < scripts_to_copy.size(); i++)
			{
				string copycmd = "\"xcopy \"" + scripts_to_copy[i] + "\" \"" + OutputDir + "\" /Y /I\"";
				int failed = system(copycmd.c_str());
				if (failed != 0)
					GMEConsole::Console::writeLine("Failed to copy post process script - " + scripts_to_copy[i], MSG_WARNING);

				boost::filesystem::path tmp(scripts_to_copy[i]);
				CopyDirectory(tmp.parent_path().string() + "//common",
							  OutputDir + "//common");

			}

			/*
			// META-2961
			boost::filesystem::path p (projectRootDir_in + "//Post_Processing/common");
			if (boost::filesystem::exists(p))
			{
				boost::filesystem::path dest(OutputDir + "//common");
				string copycmd = "\"xcopy \"" + p.make_preferred().string() + "\" \"" + dest.make_preferred().string() + "\" /Y /I\"";
				int failed = system(copycmd.c_str());
				if (failed != 0)
					GMEConsole::Console::writeLine("Failed to copy post process common directory - " + p.make_preferred().string(), MSG_WARNING);
			}
			else
				GMEConsole::Console::writeLine("Post process common directory does not exist - " + p.make_preferred().string(), MSG_WARNING);
			*/
		}
	}
}