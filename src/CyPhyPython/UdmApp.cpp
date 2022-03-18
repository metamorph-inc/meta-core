#include "stdafx.h"

#include "UdmConsole.h"

using namespace std;

#include "..\bin\Python310\Include\Python.h"
#include <algorithm>
#include <memory>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

#define CYPHY_PYTHON_VERSION _T(_STRINGIZE(PY_MAJOR_VERSION)) _T(_STRINGIZE(PY_MINOR_VERSION))

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
		if (isErrorMessageException == false && type_name && PyUnicode_Check(type_name))
		{
			error += PyUnicode_AsUTF8(type_name);
			error += ": ";
		}
	}
	PyObject_RAII message = PyObject_GetAttrString(value, "message");
	if (message && PyUnicode_Check(message))
	{
		PyObject_RAII str_value = PyObject_Str(message);
		error += PyUnicode_AsUTF8(str_value);
	}
	else
	{
		// seems PyObject_GetAttrString sets error indicator on failure
		PyErr_Clear();
		if (type && value && value.p->ob_type->tp_str)
		{
			PyObject_RAII str = value.p->ob_type->tp_str(value);
			if (PyUnicode_Check(str))
				error += PyUnicode_AsUTF8(str);
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
			PyDict_SetItemString(dict, "etype", type);
			PyDict_SetItemString(dict, "value", value);
			PyDict_SetItemString(dict, "tb", traceback);
			PyObject_RAII _none = PyRun_StringFlags(
				"import traceback\n"
				"tb = ''.join(traceback.format_exception(etype, value, tb))\n", Py_file_input, dict, dict, NULL);
            if (!PyErr_Occurred()) {
                PyObject* formatted_traceback = PyDict_GetItemString(dict, "tb");
                error = PyUnicode_AsUTF8(formatted_traceback);
            }
			PyErr_Clear();
		}
		else
			error += "Unknown traceback";
	}
	return error;
}


// PythonCOM.h: PYCOM_EXPORT PyObject *PyCom_PyObjectFromIUnknown(IUnknown *punk, REFIID riid, BOOL bAddRef = FALSE);
typedef PyObject *(*PyCom_PyObjectFromIUnknown_t)(IUnknown *punk, REFIID riid, BOOL bAddRef);
PyObject* convert(IDispatch* disp) {
	PyObject_RAII namespace_ = PyDict_New();
	// also loads pythoncomxx.dll
	PyObject_RAII ret = PyRun_StringFlags("import win32com.client\n", Py_file_input, namespace_, namespace_, NULL);
	if (ret == NULL && PyErr_Occurred())
	{
		throw python_error(GetPythonError());
	}
	const wchar_t* pythoncomname = "pythoncom" CYPHY_PYTHON_VERSION ".dll";
	HMODULE pythoncom = GetModuleHandleW(pythoncomname);
	if (pythoncom == NULL) {
		throw python_error(std::wstring(L"Could not load ") + pythoncomname);
	}
	PyCom_PyObjectFromIUnknown_t PyCom_PyObjectFromIUnknown = (PyCom_PyObjectFromIUnknown_t)GetProcAddress(pythoncom, "PyCom_PyObjectFromIUnknown");
	if (PyCom_PyObjectFromIUnknown == NULL)
		throw python_error(std::wstring(L"Could not load PyCom_PyObjectFromIUnknown from ") + pythoncomname);
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
			if (logfile.p && PyUnicode_Check(arg1)))
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
	else
	{
		PyErr_Clear();
	}
	if (PyUnicode_Check(arg1))
	{
		GMEConsole::Console::Out::writeLine(html_encode<wchar_t>(PyUnicode_AsUnicode(arg1)));
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
	else
	{
		PyErr_Format(get_builtin("ValueError"), "Argument must be str");
		return NULL;
	}
	return return_Py_None();
}

void CyPhyPython_free(void*) {
	// we called LoadLibraryW in init routine
	FreeLibrary((HMODULE)HINST_THISCOMPONENT);
}

static PyMethodDef CyPhyPython_methods[] = {
	{ "log",  CyPhyPython_log, METH_VARARGS, "Log in the GME console or results log file" },
	{ "console_message",  (PyCFunction)CyPhyPython_console_message, METH_O, "Log in the GME console" },
	{NULL, NULL, 0, NULL}        /* Sentinel */
};

static PyModuleDef CyPhyPython_module = {
	PyModuleDef_HEAD_INIT, "CyPhyPython", "CyPhyPython methods", -1, CyPhyPython_methods,
	NULL, NULL, NULL,
	// .m_free = 
	&CyPhyPython_free
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
	HMODULE python_dll = LoadLibraryW(L"Python" CYPHY_PYTHON_VERSION L".dll");
	if (python_dll == nullptr) {
		throw python_error(L"Failed to load Python" CYPHY_PYTHON_VERSION L".dll");
	}

	const char* flags[] = { "Py_NoSiteFlag", "Py_IgnoreEnvironmentFlag", "Py_DontWriteBytecodeFlag", NULL };
	for (const char** flag = flags; *flag; flag++) {
		int* Py_FlagAddress = reinterpret_cast<int*>(GetProcAddress(python_dll, *flag));
		if (Py_FlagAddress == nullptr) {
			FreeLibrary(python_dll);
			throw python_error(std::wstring(L"Failed to find ") + static_cast<const wchar_t*>(_bstr_t(*flag)) + L" in Python" CYPHY_PYTHON_VERSION L".dll");
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

template<typename F>
auto __stdcall EvalPython(BSTR expression, BSTR* error, F return_value_processor) {
	try {
		auto meta_path = GetMetaPath();
		auto cleanup = LoadPythonDll(meta_path);
		auto cleanup2 = LoadPython();

		// n.b. do not import site here without the setup done in Main()
// #define NO_SITE_IMPORTED
#ifdef NO_SITE_IMPORTED
		PyObject* site;
		{
			PyObject_RAII sys = PyImport_ImportModule("sys");
			PyObject* sys_namespace = PyModule_GetDict(sys);
			PyObject* modules_dict = PyDict_GetItemString(sys_namespace, "modules");
			site = PyDict_GetItemString(modules_dict, "site");
		}
#endif

		PyObject_RAII namespace_ = PyDict_New();
		PyDict_SetItemString(namespace_, "__builtins__", PyEval_GetBuiltins());

		PyObject_RAII ret = PyRun_StringFlags(
			static_cast<const char*>(bstr_t(expression)),
			Py_eval_input, namespace_, namespace_, 0);
		if (PyErr_Occurred())
		{
			throw python_error(GetPythonError());
		}
		return_value_processor(static_cast<PyObject*>(ret));

#ifdef NO_SITE_IMPORTED
		PyObject* new_site;
		{
			PyObject_RAII sys = PyImport_ImportModule("sys");
			PyObject* sys_namespace = PyModule_GetDict(sys);
			PyObject* modules_dict = PyDict_GetItemString(sys_namespace, "modules");
			new_site = PyDict_GetItemString(modules_dict, "site");
		}
		if (new_site != site) {
			DebugBreak();
		}
#endif
	}
	catch (const python_error& e) {
		*error = _bstr_t(e.what()).Detach();
	}
	return nullptr;
}

extern "C" __declspec(dllexport) BSTR __stdcall GetExpressionParseError(BSTR expression) {
	BSTR error = NULL;
	EvalPython(expression, &error, [](PyObject* o) { return 1; });
	return error;
}

extern "C" __declspec(dllexport) double __stdcall GetDoubleValue(BSTR expression, BSTR * error) {
	*error = NULL;
	double ret = std::nan("1");
	EvalPython(expression, error, [&](PyObject* o) {
		// PyFloat_Check is PyObject_TypeCheck(o, &PyFloat_Type); but we can't /DELAYLOAD and reference data
		PyObject_RAII pyzero;
		pyzero.p = PyFloat_FromDouble(0.0);
		PyTypeObject* pyFloat_Type = (PyTypeObject*)pyzero.p->ob_type;
		if (PyObject_TypeCheck(o, pyFloat_Type)) {
			ret = PyFloat_AsDouble(o);
		}
		});
	return ret;
}

PyObject* _GetOrCreateCyPhyPythonModule() {
	PyObject* CyPhyPython = PyImport_ImportModule("CyPhyPython");
	if (CyPhyPython)
	{
		return CyPhyPython;
	}
	else
	{
		PyErr_Clear();
		wchar_t moduleFileName[MAX_PATH];
		int size = GetModuleFileNameW((HMODULE)HINST_THISCOMPONENT, moduleFileName, _countof(moduleFileName));
		if (size == _countof(moduleFileName) || size == 0) {
			throw python_error(std::string("Could not get name"));
		}
		// increment .dll refcount; we dont' want COM to unload us then Python call one of our methods
		LoadLibraryW(moduleFileName);

		PyObject_RAII CyPhyPython = PyModule_Create(&CyPhyPython_module);
		PyObject* CyPhyPython_namespace = PyModule_GetDict(CyPhyPython);
		PyModule_AddObjectRef(CyPhyPython, "spam", PyLong_FromLong(4242)); // xxx dont commit

		PyObject_RAII sys = PyImport_ImportModule("sys");
		PyObject_RAII sys_modules = PyObject_GetAttrString(sys, "modules");
		// n.b. can't use PyImport_AppendInittab because we may be called from COM under Python.exe
		PyDict_SetItemString(sys_modules, "CyPhyPython", CyPhyPython);

		// PyDict_SetItemString(CyPhyPython_namespace, "__builtins__", PyEval_GetBuiltins());
		PyObject_RAII pyrun_ret = PyRun_StringFlags(
			"class ErrorMessageException(Exception):\n"
			"    'An Exception for which CyPhyPython does not print the stack trace'\n"
			"    pass\n",
			Py_file_input, CyPhyPython_namespace, CyPhyPython_namespace, NULL);
		if (pyrun_ret == NULL && PyErr_Occurred())
		{
			throw python_error(GetPythonError());
		}
		PyObject* ret = CyPhyPython.p;
		CyPhyPython.p = NULL;
		return ret;
	}
}

void Main(const std::wstring& meta_path, CComPtr<IMgaProject> project, CComPtr<IMgaObject> focusObject,
	std::set<CComPtr<IMgaFCO> > selectedObjects,
	long param, map<_bstr_t, _variant_t>& componentParameters, std::wstring workingDir)
{
	//AllocConsole();
	//Py_DebugFlag = 1;
	//Py_VerboseFlag = 2;

	auto cleanup = LoadPython();

#ifdef _WIN32
	std::wstring separator = L";";
#else
	std::wstring separator = L":";
#endif

	if (meta_path.length()) {
		auto cprefix = meta_path + L"bin\\Python" CYPHY_PYTHON_VERSION;
		PyObject_RAII prefix = PyUnicode_FromWideChar(cprefix.c_str(), -1));
		PyObject_RAII sys = PyImport_ImportModule("sys");
		PyObject_SetAttrString(sys, "prefix", prefix);
		PyObject_SetAttrString(sys, "exec_prefix", prefix);
	}

	PyObject_RAII CyPhyPython = _GetOrCreateCyPhyPythonModule();

	PyObject_RAII logfile;
	PyObject* ErrorMessageException = PyObject_GetAttrString(CyPhyPython, "ErrorMessageException");
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

			PyObject_RAII io_module = PyImport_ImportModule("io");
			std::wstring cfilename = std::wstring(static_cast<const wchar_t*>(output_dir->second.bstrVal)) + L"\\log\\CyPhyPython.log";

			PyObject* filename = PyUnicode_FromWideChar(cfilename.c_str(), cfilename.length());
			PyObject* mode = PyUnicode_FromWideChar(L"wt", -1);
			PyObject_RAII args = Py_BuildValue("OO", filename, mode);

			PyObject_RAII open = PyObject_GetAttrString(io_module, "open");

			logfile.p = PyObject_Call(open, args, NULL);

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

	std::wstring module_name;
	std::wstring script_path1;
	std::wstring script_path2;
	auto script_file_it = componentParameters.find(_bstr_t(L"script_file"));
	if (script_file_it == componentParameters.end())
	{
		std::wstring scriptFilename;
		// FIXME need to set parent hwnd
		scriptFilename = openfilename(L"Python Scripts (*.py)\0*.py\0");
		if (scriptFilename.length() == 0)
		{
			return;
		}
		TCHAR fullpath[MAX_PATH];
		TCHAR* filepart;
		if (!GetFullPathNameW(scriptFilename.c_str(), sizeof(fullpath) / sizeof(fullpath[0]), fullpath, &filepart)) {
		}
		else {
			*(filepart - 1) = '\0';

			script_path1 = fullpath;
			module_name = filepart;
		}
	}
	else
	{
		if (script_file_it->second.vt != VT_BSTR || SysStringLen(script_file_it->second.bstrVal) == 0)
		{
			throw python_error("No script_file specified");
		}
		module_name = _bstr_t(script_file_it->second.bstrVal);
		if (module_name != L"" && PathIsRelativeW(module_name.c_str())) {
			std::replace(module_name.begin(), module_name.end(), '/', '\\');
			if (module_name.rfind('\\') != std::string::npos)
			{
				std::wstring path = module_name.substr(0, module_name.rfind('\\'));

				script_path1 = workingDir + L"\\CyPhyPythonScripts\\" + path;
				script_path2 = workingDir + L"\\" + path;
				module_name = module_name.substr(module_name.rfind('\\') + 1);
			}
			else
			{
				script_path1 = workingDir + L"\\CyPhyPythonScripts";
				script_path2 = workingDir;
			}
		}
		else if (module_name != L"") {
			TCHAR fullpath[MAX_PATH];
			TCHAR* filepart;
			if (!GetFullPathNameW(module_name.c_str(), sizeof(fullpath) / sizeof(fullpath[0]), fullpath, &filepart)) {
			}
			else {
				*(filepart - 1) = '\0';

				script_path1 = fullpath;
				module_name = filepart;
			}

		}
	}

	{
		PyObject_RAII namespace_ = PyDict_New();
		PyObject_RAII ret = PyRun_StringFlags(
			"import importlib\n"
			"import sys\n"
			"if sys.modules.get('site') is None:\n"
			"    import site\n"
			"    site.main()\n"
			// "import pdb; pdb.set_trace()\n"
			"import udm\n"
			, Py_file_input, namespace_, namespace_, NULL);
		if (ret == NULL && PyErr_Occurred())
		{
			throw python_error(GetPythonError());
		}
	}

	if (0)
	{
		Py_XDECREF(CyPhyPython.p);
		CyPhyPython.p = NULL;

		PyObject_RAII namespace_ = PyDict_New();

		PyObject_RAII ret = PyRun_StringFlags(
			"import sys\n"
			"x = [sys.modules['CyPhyPython']]\n"
			"import udm\n"
			"del sys.modules['CyPhyPython']\n"
			"import ctypes\n"
			"ctypes.windll.kernel32.AllocConsole()\n"
			"sys.stdout = open('CONOUT$', 'wt')\n"
			"sys.stderr = open('CONOUT$', 'wt')\n"
			"sys.stdin = open('CONIN$', 'rt')\n"
			// "path = udm.findPathToNearestRoot(x)\n"
			"import pdb\n"
			"pdb.set_trace()\n"
			, Py_file_input, namespace_, namespace_, NULL);
		if (ret == NULL && PyErr_Occurred())
		{
			throw python_error(GetPythonError());
		}
		return;

	}
	{
		const wchar_t* pythoncomname = L"pythoncom" CYPHY_PYTHON_VERSION L".dll";
		HMODULE pythoncom = GetModuleHandleW(pythoncomname);
		if (pythoncom == nullptr) {
			PyObject_RAII namespace_ = PyDict_New();

			PyObject_RAII ret = PyRun_StringFlags(
				// sitecustomize.py contains code to load the correct pythoncom3x.dll
				"import pythoncom\n"
				, Py_file_input, namespace_, namespace_, NULL);
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


	if (module_name.rfind(L".py") != std::string::npos)
	{
		module_name = module_name.substr(0, module_name.rfind(L".py"));
	}
	if (module_name != L"")
	{
		PyObject_RAII namespace_ = PyDict_New();
		PyObject_RAII script_paths;

		if (script_path2.length() == 0) {
			script_paths.p = Py_BuildValue("(u)", script_path1.c_str());
		}
		else {
			script_paths.p = Py_BuildValue("uu", script_path1.c_str(), script_path2.c_str());
		}
		PyDict_SetItemString(namespace_, "script_paths", script_paths);

		PyObject_RAII ret = PyRun_StringFlags(
			(std::string(
				"import sys\n"
				"sys.path[0:0] = script_paths\n"
				"try:\n"
				"    import ") + static_cast<const char*>(CStringA(module_name.c_str())) + "\n"
				"finally:\n"
				"    sys.path[0:len(script_paths)] = []\n"
				"    del script_paths\n").c_str()
			, Py_file_input, namespace_, namespace_, NULL);
		if (ret == NULL && PyErr_Occurred())
		{
			throw python_error(GetPythonError());
		}

		PyObject_RAII module = PyImport_ImportModule(static_cast<const char*>(CStringA(module_name.c_str())));
		if (!module)
		{
			throw python_error(GetPythonError());
		}

		PyObject_RAII reloaded_module;
		reloaded_module.p = module.p;
		Py_IncRef(reloaded_module.p);
		// TODO: check if this is necessary
		// PyObject_RAII reloaded_module = PyImport_ReloadModule(module);
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
			PyObject_RAII invoke_namespace = PyDict_New();
			PyObject_RAII invokeGME = PyObject_GetAttrString(reloaded_module, "invokeGME");
			if (!invokeGME.p)
			{
				PyErr_Clear();
				throw python_error("Error: script has no \"invoke\" or \"invokeGME\" function");
			}
			PyDict_SetItemString(invoke_namespace, "focusObject", pyFocusObject.p);
			PyDict_SetItemString(invoke_namespace, "rootObject", pyRootObject.p);
			PyDict_SetItemString(invoke_namespace, "project", pyProject.p);

			PyObject_RAII setup = PyRun_StringFlags(
				"import win32com.client.dynamic\n"
				"if focusObject: focusObject = win32com.client.dynamic.Dispatch(focusObject)\n"
				"if rootObject: rootObject = win32com.client.dynamic.Dispatch(rootObject)\n"
				"project = win32com.client.dynamic.Dispatch(project)\n"
				, Py_file_input, invoke_namespace, invoke_namespace, NULL);
			if (setup == nullptr && PyErr_Occurred())
			{
				throw python_error(GetPythonError());
			}

			PyObject_RAII empty_tuple = PyTuple_New(0);
			PyObject_RAII args = PyDict_New();
			PyDict_SetItemString(args, "focusObject", PyDict_GetItemString(invoke_namespace, "focusObject"));
			PyDict_SetItemString(args, "rootObject", PyDict_GetItemString(invoke_namespace, "rootObject"));
			PyDict_SetItemString(args, "project", PyDict_GetItemString(invoke_namespace, "project"));
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

			PyObject_RAII invoke_namespace = PyDict_New();
			PyDict_SetItemString(invoke_namespace, "focusObj", pyFocusObject.p);
			PyDict_SetItemString(invoke_namespace, "rootObj", pyRootObject.p);
			PyDict_SetItemString(invoke_namespace, "project", pyProject.p);

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
				, Py_file_input, invoke_namespace, invoke_namespace, NULL);
			if (setup == nullptr && PyErr_Occurred())
			{
				throw python_error(GetPythonError());
			}

			PyObject_RAII empty_tuple = PyTuple_New(0);
			PyObject_RAII args = PyDict_New();
			PyDict_SetItemString(args, "focusObject", PyDict_GetItemString(invoke_namespace, "focusObj"));
			PyDict_SetItemString(args, "rootObject", PyDict_GetItemString(invoke_namespace, "rootObj"));
			PyDict_SetItemString(args, "udmProject", PyDict_GetItemString(invoke_namespace, "udm_project"));
			PyDict_SetItemString(args, "componentParameters", parameters);

			PyObject_RAII ret = PyObject_Call(invoke, empty_tuple, args);
			if (ret == nullptr && PyErr_Occurred())
			{
				invokeError = std::unique_ptr<python_error>(new python_error(GetPythonError(ErrorMessageException)));
			}

			PyObject_RAII dn_close;
			if (invokeError) {
				dn_close.p = PyRun_StringFlags("udm_project.close_no_update(); cyphy.close_no_update()\n", Py_file_input, invoke_namespace, invoke_namespace, NULL);
			}
			else {
				dn_close.p = PyRun_StringFlags("udm_project.close_with_update(); cyphy.close_no_update()\n", Py_file_input, invoke_namespace, invoke_namespace, NULL);
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
			if (PyUnicode_Check(key)) {
				paramName = _bstr_t(PyUnicode_AsUnicode(key));
			}
			else {
				continue;
			}
			if (PyUnicode_Check(value))
			{
				componentParameters[paramName] = _bstr_t(PyUnicode_AsUnicode(value));
			}
		}
	}

// #define CyPhyPython_Cleanup 1
#ifdef CyPhyPython_Cleanup
	{
		PyObject_RAII sys = PyImport_ImportModule("sys");
		PyObject_RAII sys_modules = PyObject_GetAttrString(sys, "modules");
		PyObject_RAII py_module_name = PyUnicode_FromWideChar(module_name.c_str(), -1);
		PyDict_DelItem(sys_modules, py_module_name);
		if (PyErr_Occurred())
		{
			throw python_error(GetPythonError());
		}
	}

	Py_XDECREF(CyPhyPython.p);
	CyPhyPython.p = NULL;

	// PyObject_RAII main = PyImport_ImportModule("__main__");
	// if (main.p == NULL) {
	// 	throw python_error(GetPythonError());
	// }
	// Py_XDECREF(main.p);
	// main.p = NULL;

	PyObject_RAII cleanup_namespace = PyDict_New();
	PyObject_RAII ret = PyRun_StringFlags(
		"import sys\n"
		"del sys.modules['CyPhyPython']\n"
		// "del sys.modules['__main__']\n"
		"import gc; gc.collect();"
		, Py_file_input, cleanup_namespace, cleanup_namespace, NULL);
	if (ret == NULL && PyErr_Occurred())
	{
		throw python_error(GetPythonError());
	}
#endif

}
