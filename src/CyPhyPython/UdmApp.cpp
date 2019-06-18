#include "stdafx.h"

#include "UdmConsole.h"

using namespace std;

#include "..\bin\Python27\Include\Python.h"
#include <algorithm>
#include <memory>

#define CYPHY_PYTHON_VERSION _STRINGIZE(PY_MAJOR_VERSION) _STRINGIZE(PY_MINOR_VERSION)

// n.b. can't /DELAYLOAD and import data
static PyObject* get_Py_None()
{
	PyObject* none = Py_BuildValue("");
	Py_DecRef(none);
	return none;
}

static PyObject* return_Py_None()
{
	PyObject* none = Py_BuildValue("");
	return none;
}

struct NonCopyable {
	NonCopyable() {}
private:
	NonCopyable & operator=(const NonCopyable&);
	NonCopyable(const NonCopyable&);
};

struct PyObject_RAII : private NonCopyable
{
	PyObject* p;
	PyObject_RAII() : p(NULL) { }
	PyObject_RAII(PyObject* p) : p(p) { }
	operator PyObject*() { return p; }
	~PyObject_RAII() { Py_XDECREF(p); }
};

static PyObject* get_builtin(const char* name)
{
#if PY_MAJOR_VERSION > 2
	PyObject_RAII builtins = PyImport_ImportModule("builtins");
#else
	PyObject_RAII builtins = PyImport_ImportModule("__builtin__");
#endif

	PyObject* builtin_dict = PyModule_GetDict(builtins);
	PyObject* ValueError = PyDict_GetItemString(builtin_dict, name);
	return ValueError;
}

std::string GetPythonError(PyObject* ErrorMessageException=nullptr)
{
	PyObject_RAII type, value, traceback;
	PyErr_Fetch(&type.p, &value.p, &traceback.p);
	PyErr_Clear();
	std::string error;
	bool isErrorMessageException = false;
	if (type)
	{
		isErrorMessageException = ErrorMessageException != nullptr && PyObject_IsSubclass(type, ErrorMessageException) == 1;
		PyObject_RAII type_name = PyObject_GetAttrString(type, "__name__");
		if (isErrorMessageException == false && type_name && PyString_Check(type_name))
		{
			error += PyString_AsString(type_name);
			error += ": ";
		}
	}
	PyObject_RAII message = PyObject_GetAttrString(value, "message");
	if (message && PyString_Check(message))
	{
		PyObject_RAII str_value = PyObject_Str(message);
		error += PyString_AsString(str_value);
	}
	else
	{
		// seems PyObject_GetAttrString sets error indicator on failure
		PyErr_Clear();
		if (type && value && value.p->ob_type->tp_str)
		{
			PyObject_RAII str = value.p->ob_type->tp_str(value);
			if (PyString_Check(str))
				error += PyString_AsString(str);
		}
		else
		{
			error += "Unknown exception";
		}
	}

	if (isErrorMessageException == false) {
		error += ": ";
		if (traceback)
		{
			PyObject_RAII main = PyImport_ImportModule("__main__");
			PyObject* main_namespace = PyModule_GetDict(main);
			PyObject_RAII dict = PyDict_Copy(main_namespace);
			PyDict_SetItemString(dict, "tb", traceback);
			PyObject_RAII _none = PyRun_StringFlags(
				"import traceback\n"
				"tb = ''.join(traceback.format_tb(tb))\n", Py_file_input, dict, dict, NULL);
            if (PyErr_Occurred()) {
                PyObject* formatted_traceback = PyDict_GetItemString(dict, "tb");
                error += PyString_AsString(formatted_traceback);
            }
		}
		else
			error += "Unknown traceback";
	}
	return error;
}


// PythonCOM.h: PYCOM_EXPORT PyObject *PyCom_PyObjectFromIUnknown(IUnknown *punk, REFIID riid, BOOL bAddRef = FALSE);
typedef PyObject *(*PyCom_PyObjectFromIUnknown_t)(IUnknown *punk, REFIID riid, BOOL bAddRef);
PyObject* convert(IDispatch* disp) {
	PyObject_RAII main = PyImport_ImportModule("__main__");
	PyObject* main_namespace = PyModule_GetDict(main);
	// also loads pythoncomxx.dll
	PyObject_RAII ret = PyRun_StringFlags("import win32com.client\n", Py_file_input, main_namespace, main_namespace, NULL);
	if (ret == NULL && PyErr_Occurred())
	{
		throw python_error(GetPythonError());
	}
	const char* pythoncomname = "pythoncom" CYPHY_PYTHON_VERSION ".dll";
	HMODULE pythoncom = GetModuleHandleA(pythoncomname);
	if (pythoncom == NULL)
		throw python_error(std::string("Could not load ") + pythoncomname);
	PyCom_PyObjectFromIUnknown_t PyCom_PyObjectFromIUnknown = (PyCom_PyObjectFromIUnknown_t)GetProcAddress(pythoncom, "PyCom_PyObjectFromIUnknown");
	if (PyCom_PyObjectFromIUnknown == NULL)
		throw python_error(std::string("Could not load PyCom_PyObjectFromIUnknown from ") + pythoncomname);
	PyObject* obj = (*PyCom_PyObjectFromIUnknown)(disp, IID_IDispatch, TRUE);
	return obj;
}


#ifdef _UNICODE
std::wstring openfilename(TCHAR *filter = _T("All Files (*.*)\0*.*\0"), HWND owner = NULL)
#else
std::string openfilename(TCHAR *filter = _T("All Files (*.*)\0*.*\0"), HWND owner = NULL)
#endif
{
	OPENFILENAME ofn;
	TCHAR fileName[MAX_PATH] = _T("");
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = _T("");
#ifdef _UNICODE
    std::wstring fileNameStr;
#else
    std::string fileNameStr;
#endif
	if (GetOpenFileName(&ofn))
		fileNameStr = fileName;
	return fileNameStr;
}

static PyObject *CyPhyPython_log(PyObject *self, PyObject *args)
{
	PyObject* arg1 = PyTuple_GetItem(args, 0);
	PyObject_RAII CyPhyPython = PyImport_ImportModule("CyPhyPython");
	if (CyPhyPython)
	{
		if (PyObject_HasAttrString(CyPhyPython, "_logfile"))
		{
			PyObject_RAII logfile = PyObject_GetAttrString(CyPhyPython, "_logfile");
			if (logfile.p && (PyUnicode_Check(arg1) || PyString_Check(arg1)))
			{
				PyObject_RAII write = PyObject_GetAttrString(logfile, "write");
				{
					PyObject_RAII write_args = Py_BuildValue("(O)", arg1);
					PyObject_RAII ret = PyObject_CallObject(write, write_args);
					ASSERT(ret.p);
				}
				{
					PyObject_RAII newline = PyUnicode_FromString("\n");
					PyObject_RAII write_args = Py_BuildValue("(O)", newline);
					PyObject_RAII ret = PyObject_CallObject(write, write_args);
					ASSERT(ret.p);
				}
				return return_Py_None();
			}
		}
	}
	if (PyUnicode_Check(arg1))
	{
		GMEConsole::Console::Out::writeLine(html_encode<wchar_t>(PyUnicode_AsUnicode(arg1)));
	}
	else if (PyString_Check(arg1))
	{
		GMEConsole::Console::Out::writeLine(html_encode<char>(PyString_AsString(arg1)));
	}
	else
	{
		PyErr_Format(get_builtin("ValueError"), "Argument must be str or unicode");
		return NULL;
	}
	return return_Py_None();
}

static PyObject *CyPhyPython_console_message(PyObject *self, PyObject *arg1)
{
	if (PyUnicode_Check(arg1))
	{
		GMEConsole::Console::Out::writeLine(PyUnicode_AsUnicode(arg1));
	}
	else if (PyString_Check(arg1))
	{
		GMEConsole::Console::Out::writeLine(PyString_AsString(arg1));
	}
	else
	{
		PyErr_Format(get_builtin("ValueError"), "Argument must be str or unicode");
		return NULL;
	}
	return return_Py_None();
}

static PyMethodDef CyPhyPython_methods[] = {
	{ "log",  CyPhyPython_log, METH_VARARGS, "Log in the GME console or results log file" },
	{ "console_message",  (PyCFunction)CyPhyPython_console_message, METH_O, "Log in the GME console" },
	{NULL, NULL, 0, NULL}        /* Sentinel */
};


struct PythonCleanup {
	std::unique_ptr<UnGil> gilState;
	std::unique_ptr<RAIIFreeLibrary> pythonDll;
	PythonCleanup(PyGILState_STATE gilState, HMODULE python_dll) :
		gilState(new UnGil(gilState)), pythonDll(new RAIIFreeLibrary(python_dll)) {
	}
};

PythonCleanup LoadPython() {
	// Py_NoSiteFlag = 1; // we import site after setting up sys.path
	HMODULE python_dll = LoadLibraryA("Python" CYPHY_PYTHON_VERSION ".dll");
	if (python_dll == nullptr)
		throw python_error("Failed to load Python" CYPHY_PYTHON_VERSION ".dll");

	const char* flags[] = { "Py_NoSiteFlag", "Py_IgnoreEnvironmentFlag", "Py_DontWriteBytecodeFlag", NULL };
	for (const char** flag = flags; *flag; flag++) {
		int* Py_FlagAddress = reinterpret_cast<int*>(GetProcAddress(python_dll, *flag));
		if (Py_FlagAddress == nullptr) {
			FreeLibrary(python_dll);
			throw python_error(std::string("Failed to find ") + *flag + " in Python" CYPHY_PYTHON_VERSION  ".dll");
		}
		*Py_FlagAddress = 1;
	}

	// Py_SetProgramName("GME.exe");

#if PY_MAJOR_VERSION >= 3
    bool initialized = Py_IsInitialized();
	Py_InitializeEx(0); // this calls PyEval_InitThreads iff initialized == false
	if (initialized == false) {
        PyEval_SaveThread();
    }

#else
	Py_InitializeEx(0);
	if (!PyEval_ThreadsInitialized()) {
		PyEval_InitThreads();
		PyEval_SaveThread();
	}
#endif

	// n.b. we need this to be reentrant (i.e. in case this interpreter is being called by python (e.g. via win32com.client))
	// this is because PyEval_SaveThread() was called
	PyGILState_STATE gstate = PyGILState_Ensure();
	return PythonCleanup(gstate, python_dll);
}

extern "C" __declspec(dllexport) BSTR __stdcall GetExpressionParseError(BSTR expression) {
	try {
		auto meta_path = GetMetaPath();
		auto cleanup = LoadPythonDll(meta_path);
		auto cleanup2 = LoadPython();

		// n.b. do not import site here without the setup done in Main()
		PyObject_RAII namespace_ = PyDict_New();
		PyDict_SetItemString(namespace_, "__builtins__", PyEval_GetBuiltins());

		PyObject_RAII ret = PyRun_StringFlags(
			static_cast<const char*>(bstr_t(expression)),
			Py_eval_input, namespace_, namespace_, 0);
		if (PyErr_Occurred())
		{
			throw python_error(GetPythonError());
		}
	}
	catch (const python_error& e) {
		return _bstr_t(e.what()).Detach();
	}
	return nullptr;
}

void Main(const std::string& meta_path, CComPtr<IMgaProject> project, CComPtr<IMgaObject> focusObject,
					 std::set<CComPtr<IMgaFCO> > selectedObjects,
					 long param, map<_bstr_t, _variant_t>& componentParameters, std::string workingDir)
{
	//AllocConsole();
	//Py_DebugFlag = 1;
	//Py_VerboseFlag = 2;

	auto cleanup = LoadPython();

	char *path = Py_GetPath();
	
#ifdef _WIN32
	std::string separator = ";";
#else
	std::string separator = ":";
#endif

	std::string newpath;
	if (meta_path.length())
	{
		// n.b. don't use Py_GetPath(), since it may read garbage from HKCU\Software\Python\PythonCore\2.7\PythonPath
		newpath = meta_path + "\\bin\\Python" CYPHY_PYTHON_VERSION "\\Scripts\\python" CYPHY_PYTHON_VERSION ".zip";
		newpath = newpath + separator + meta_path + "\\bin\\Python" CYPHY_PYTHON_VERSION "\\Scripts";
		newpath = newpath + separator + meta_path + "\\bin\\Python" CYPHY_PYTHON_VERSION "\\Lib";
		newpath = newpath + separator + meta_path + "\\bin";
	}
	else {
		newpath = path;
	}

	PySys_SetPath(const_cast<char*>(newpath.c_str()));

	PyObject_RAII main = PyImport_ImportModule("__main__");
	PyObject* main_namespace = PyModule_GetDict(main);
	{
		PyObject_RAII ret = PyRun_StringFlags(
			"import sys\n"
			, Py_file_input, main_namespace, main_namespace, NULL);
		if (ret == NULL && PyErr_Occurred())
		{
			throw python_error(GetPythonError());
		}
	}

	if (meta_path.length()) {
		PyObject_RAII prefix = PyString_FromString((meta_path + "\\bin\\Python" CYPHY_PYTHON_VERSION).c_str());
		PyObject* sys = PyDict_GetItemString(main_namespace, "sys");
		PyObject_SetAttrString(sys, "prefix", prefix);
		PyObject_SetAttrString(sys, "exec_prefix", prefix);
	}
	
	PyObject* CyPhyPython = Py_InitModule("CyPhyPython", CyPhyPython_methods);
	PyObject* CyPhyPython_namespace = PyModule_GetDict(CyPhyPython);
	PyDict_SetItemString(CyPhyPython_namespace, "__builtins__", PyEval_GetBuiltins());
	PyObject_RAII ret = PyRun_StringFlags(
		"class ErrorMessageException(Exception):\n"
		"    'An Exception for which CyPhyPython does not print the stack trace'\n"
		"    pass\n",
		Py_file_input, CyPhyPython_namespace, CyPhyPython_namespace, NULL);
	if (ret == NULL && PyErr_Occurred())
	{
		throw python_error(GetPythonError());
	}
	PyObject_RAII logfile;
	PyObject* ErrorMessageException = PyDict_GetItemString(CyPhyPython_namespace, "ErrorMessageException");
	auto console_messages = componentParameters.find(L"console_messages");
	if (console_messages != componentParameters.end()
		&& console_messages->second.vt == VT_BSTR
		&& console_messages->second.bstrVal
		&& _wcsicmp(console_messages->second.bstrVal, L"off") == 0)
	{
		auto output_dir = componentParameters.find(L"output_dir");
		if (output_dir != componentParameters.end()
			&& output_dir->second.vt == VT_BSTR
			&& output_dir->second.bstrVal
			&& *output_dir->second.bstrVal)
		{
			CreateDirectoryW(CStringW(output_dir->second.bstrVal) + L"\\log", NULL);
			logfile.p = PyFile_FromString(const_cast<char *>(static_cast<const char*>(CStringA(output_dir->second.bstrVal) + "\\log\\CyPhyPython.log")), "w");
			if (PyErr_Occurred())
			{
				// FIXME where to log this
				PyErr_Clear();
			}
			else
			{
				PyObject_SetAttrString(CyPhyPython, "_logfile", logfile);
			}
		}
	}
	if (logfile.p == nullptr)
	{
		// may be leftover from last run
		if (PyObject_HasAttrString(CyPhyPython, "_logfile"))
		{
			PyObject_DelAttrString(CyPhyPython, "_logfile");
		}
	}

	std::string module_name;
	auto script_file_it = componentParameters.find(_bstr_t(L"script_file"));
	if (script_file_it == componentParameters.end())
	{
		std::wstring scriptFilename;
		scriptFilename = openfilename(L"Python Scripts (*.py)\0*.py\0");
		if (scriptFilename.length() == 0)
		{
			return;
		}
		TCHAR fullpath[MAX_PATH];
		TCHAR* filepart;
		if (!GetFullPathNameW(scriptFilename.c_str(), sizeof(fullpath)/sizeof(fullpath[0]), fullpath, &filepart)) {
		} else {
			*(filepart-1) = '\0';

			newpath += separator + static_cast<const char*>(CStringA(fullpath));
			PySys_SetPath(const_cast<char*>(newpath.c_str()));

			module_name = static_cast<const char*>(CStringA(filepart));
		}
	}
	else
	{
		if (script_file_it->second.vt != VT_BSTR || SysStringLen(script_file_it->second.bstrVal) == 0)
		{
			throw python_error("No script_file specified");
		}
		module_name = _bstr_t(script_file_it->second.bstrVal);
		if (module_name != "" && PathIsRelativeA(module_name.c_str())) {
			std::replace(module_name.begin(), module_name.end(), '/', '\\');
			if (module_name.rfind('\\') != std::string::npos)
			{
				std::string path = module_name.substr(0, module_name.rfind('\\'));

				newpath = workingDir + "\\CyPhyPythonScripts\\" + path + separator + newpath;
				newpath = workingDir + "\\" + path + separator + newpath;
				PySys_SetPath(const_cast<char*>(newpath.c_str()));
				module_name = module_name.substr(module_name.rfind('\\') + 1);
			}
			else
			{
				newpath = workingDir + "\\CyPhyPythonScripts" + separator + newpath;
				newpath = workingDir + separator + newpath;
				PySys_SetPath(const_cast<char*>(newpath.c_str()));
			}

			//newpath += separator + fullpath;
			//PySys_SetPath(const_cast<char*>(newpath.c_str()));
		}
	}
	{
		PyObject_RAII ret = PyRun_StringFlags("import site\n"
			"reload(site)\n"
			"import sitecustomize\n"
			"reload(sitecustomize)\n"
			"import os.path\n"
			"import udm\n"
			, Py_file_input, main_namespace, main_namespace, NULL);
		if (ret == NULL && PyErr_Occurred())
		{
			throw python_error(GetPythonError());
		}
	}
	{
		const char* pythoncomname = "pythoncom" CYPHY_PYTHON_VERSION ".dll";
		HMODULE pythoncom = GetModuleHandleA(pythoncomname);
		if (pythoncom == nullptr) {
			PyObject_RAII ret = PyRun_StringFlags(
				// sitecustomize.py contains code to load the correct pythoncom27.dll
				"import pythoncom\n"
				, Py_file_input, main_namespace, main_namespace, NULL);
			if (ret == NULL && PyErr_Occurred())
			{
				throw python_error(GetPythonError());
			}
		}
	}
	PyObject_RAII pyRootObject;
	CComPtr<IMgaFolder> root;
	project->get_RootFolder(&root);
	pyRootObject.p = convert(root);

	PyObject_RAII pyProject = convert(project);

	PyObject_RAII pyFocusObject;
	if (focusObject)
	{
		pyFocusObject.p = convert(focusObject);
	}
	else
	{
		pyFocusObject.p = get_Py_None();
		Py_INCREF(pyFocusObject.p);
	}


	if (module_name.rfind(".py") != std::string::npos)
	{
		module_name = module_name.substr(0, module_name.rfind(".py"));
	}
	if (module_name != "")
	{
		PyObject_RAII module = PyImport_ImportModule(module_name.c_str());
		if (!module)
		{
			throw python_error(GetPythonError());
		}
		// TODO: check if this is necessary
		PyObject_RAII reloaded_module = PyImport_ReloadModule(module);
		if (!reloaded_module)
		{
			throw python_error(GetPythonError());
		}
		PyObject_RAII parameters = PyDict_New();
		for (auto it = componentParameters.begin(); it != componentParameters.end(); it++)
		{
			if (it->second.vt == VT_BSTR)
			{
				PyObject_RAII parameterString = PyUnicode_FromWideChar(it->second.bstrVal, SysStringLen(it->second.bstrVal));
				PyDict_SetItemString(parameters, static_cast<const char*>(it->first), parameterString);
			}
		}

		std::unique_ptr<python_error> invokeError;
		PyObject_RAII invoke = PyObject_GetAttrString(reloaded_module, "invoke");
		if (!invoke)
		{
			PyErr_Clear();
			PyObject_RAII invokeGME = PyObject_GetAttrString(reloaded_module, "invokeGME");
			if (!invokeGME.p)
			{
				PyErr_Clear();
				throw python_error("Error: script has no \"invoke\" or \"invokeGME\" function");
			}
			PyDict_SetItemString(main_namespace, "focusObject", pyFocusObject.p);
			PyDict_SetItemString(main_namespace, "rootObject", pyRootObject.p);
			PyDict_SetItemString(main_namespace, "project", pyProject.p);

			PyObject_RAII setup = PyRun_StringFlags(
				"import win32com.client.dynamic\n"
				"if focusObject: focusObject = win32com.client.dynamic.Dispatch(focusObject)\n"
				"if rootObject: rootObject = win32com.client.dynamic.Dispatch(rootObject)\n"
				"project = win32com.client.dynamic.Dispatch(project)\n"
				, Py_file_input, main_namespace, main_namespace, NULL);
			if (setup == nullptr && PyErr_Occurred())
			{
				throw python_error(GetPythonError());
			}

			PyObject_RAII empty_tuple = PyTuple_New(0);
			PyObject_RAII args = PyDict_New();
			PyDict_SetItemString(args, "focusObject", PyDict_GetItemString(main_namespace, "focusObject"));
			PyDict_SetItemString(args, "rootObject", PyDict_GetItemString(main_namespace, "rootObject"));
			PyDict_SetItemString(args, "project", PyDict_GetItemString(main_namespace, "project"));
			PyDict_SetItemString(args, "componentParameters", parameters);

			PyObject_RAII ret = PyObject_Call(invokeGME, empty_tuple, args);
			if (ret == nullptr && PyErr_Occurred())
			{
				invokeError = std::unique_ptr<python_error>(new python_error(GetPythonError(ErrorMessageException)));
			}
		}
		else {
			if (!PyCallable_Check(invoke))
			{
				throw python_error("Error: script \"invoke\" attribute is not callable");
			}

			PyDict_SetItemString(main_namespace, "focusObj", pyFocusObject.p);
			PyDict_SetItemString(main_namespace, "rootObj", pyRootObject.p);
			PyDict_SetItemString(main_namespace, "project", pyProject.p);

			PyObject_RAII setup = PyRun_StringFlags(
				//"import ctypes\n"
				//"ctypes.windll.kernel32.allocconsole()\n"
				//"import sys\n"
				//"sys.stdout = open('conout$', 'wt')\n"
				//"sys.stdin = open('conin$', 'rt')\n"
				//"import pdb; pdb.set_trace()\n"
				"import win32com.client.dynamic\n"
				"import udm\n"
				"import os.path\n"
				"import six\n"
				"with six.moves.winreg.OpenKey(six.moves.winreg.HKEY_LOCAL_MACHINE, r'Software\\META') as software_meta:\n"
				"    meta_path, _ = six.moves.winreg.QueryValueEx(software_meta, 'META_PATH')\n"
				"CyPhyML_udm = os.path.join(meta_path, r'generated\\CyPhyML\\models\\CyPhyML_udm.xml')\n"
				"if not os.path.isfile(CyPhyML_udm):\n"
				"	CyPhyML_udm = os.path.join(meta_path, r'meta\\CyPhyML_udm.xml')\n"
				"cyphy = udm.SmartDataNetwork(udm.uml_diagram())\n"
				"cyphy.open(CyPhyML_udm, '')\n"
				"udm_project = udm.SmartDataNetwork(cyphy.root)\n"
				"udm_project.open(project, '')\n"
				"if focusObj: focusObj = win32com.client.dynamic.Dispatch(focusObj)\n"
				"if rootObj: rootObj = win32com.client.dynamic.Dispatch(rootObj)\n"
				"if focusObj: focusObj = udm_project.convert_gme2udm(focusObj)\n"
				"if rootObj: rootObj = udm_project.convert_gme2udm(rootObj)\n"
				, Py_file_input, main_namespace, main_namespace, NULL);
			if (setup == nullptr && PyErr_Occurred())
			{
				throw python_error(GetPythonError());
			}

			PyObject_RAII empty_tuple = PyTuple_New(0);
			PyObject_RAII args = PyDict_New();
			PyDict_SetItemString(args, "focusObject", PyDict_GetItemString(main_namespace, "focusObj"));
			PyDict_SetItemString(args, "rootObject", PyDict_GetItemString(main_namespace, "rootObj"));
			PyDict_SetItemString(args, "udmProject", PyDict_GetItemString(main_namespace, "udm_project"));
			PyDict_SetItemString(args, "componentParameters", parameters);

			PyObject_RAII ret = PyObject_Call(invoke, empty_tuple, args);
			if (ret == nullptr && PyErr_Occurred())
			{
				invokeError = std::unique_ptr<python_error>(new python_error(GetPythonError(ErrorMessageException)));
			}

			PyObject_RAII dn_close;
			if (invokeError) {
				dn_close.p = PyRun_StringFlags("udm_project.close_no_update(); cyphy.close_no_update()\n", Py_file_input, main_namespace, main_namespace, NULL);
			}
			else {
				dn_close.p = PyRun_StringFlags("udm_project.close_with_update(); cyphy.close_no_update()\n", Py_file_input, main_namespace, main_namespace, NULL);
			}
			if (dn_close.p == NULL && PyErr_Occurred())
			{
				throw python_error(GetPythonError());
			}
		}

		if (PyObject_HasAttrString(CyPhyPython, "_logfile"))
		{
			PyObject_RAII logfile = PyObject_GetAttrString(CyPhyPython, "_logfile");
			if (logfile.p)
			{
				PyObject_RAII write = PyObject_GetAttrString(logfile, "close");
				PyObject_RAII ret = PyObject_CallObject(write, NULL);
				ASSERT(ret.p);
				if (ret == NULL)
				{
					throw python_error(GetPythonError());
				}
				PyObject_DelAttrString(CyPhyPython, "_logfile");
			}
		}
		if (invokeError)
		{
			throw *invokeError.get();
		}
		PyObject *key, *value;
		Py_ssize_t pos = 0;

		while (PyDict_Next(parameters, &pos, &key, &value)) {
			_bstr_t paramName;
			if (PyString_Check(key)) {
				paramName = _bstr_t(PyString_AsString(key));
			}
			else if (PyUnicode_Check(key)) {
				paramName = _bstr_t(PyUnicode_AsUnicode(key));
			}
			else {
				continue;
			}
			if (PyString_Check(value))
			{
				componentParameters[paramName] = _bstr_t(PyString_AsString(value));
			}
			if (PyUnicode_Check(value))
			{
				componentParameters[paramName] = _bstr_t(PyUnicode_AsUnicode(value));
			}
		}
	}
}
