/*** Included Header Files ***/
#include "motivate.h"

//Work around Python _DEBUG issue
#ifdef _DEBUG
#define _DEBUG_PYTHON_WORKAROUND
#undef _DEBUG
#endif
// Include Python
#include <Python.h>
#ifdef _DEBUG_PYTHON_WORKAROUND
#define _DEBUG
#undef _DEBUG_PYTHON_WORKAROUND
#endif


/************************************************************************************/

extern "C" {

static PyObject* Motivate_Console(PyObject *self, PyObject *args) {
	Console::Out::WriteLine( "Executing Console from Python." );
	Py_INCREF(Py_None);
	return Py_None;
}


static PyObject* Motivate_SetValue(PyObject *self, PyObject *args) {
	Console::Out::WriteLine( "Executing Console from Python." );
	Py_INCREF(Py_None);
	return Py_None;
}


static PyObject* Motivate_SetStatus(PyObject *self, PyObject *args) {
	Console::Out::WriteLine( "Executing Console from Python." );
	Py_INCREF(Py_None);
	return Py_None;
}


static PyMethodDef MotivateMethods[] = {
	{"Console", Motivate_Console, METH_VARARGS, "Write output to the GME console."},
    {"SetValue",  Motivate_SetValue, METH_VARARGS, "Set a Motivate variable value."},
	{"SetStatus",  Motivate_SetStatus, METH_VARARGS, "Set the return status for the current action."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


static struct PyModuleDef MotivateModule = {
   PyModuleDef_HEAD_INIT,
   "Motivate",		/* name of module */
   NULL,			/* module documentation, may be NULL */
   -1,				/* size of per-interpreter state of the module,
						or -1 if the module keeps state in global variables. */
   MotivateMethods
};


PyObject* PyInit_Motivate(void) {
    return PyModule_Create( &MotivateModule );
}


}	// end of extern "C"


/************************************************************************************/

