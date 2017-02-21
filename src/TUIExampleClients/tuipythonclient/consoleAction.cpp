//
//  main.cpp
//  PythonPlugin
//
//  Created by Julian Adenauer on 19.08.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "Python.h"
#include <iostream>
#include "math.h"

PyMODINIT_FUNC PyInit_consoleAction(void);
static PyObject *my_callback = NULL;


// Funktion zum Registrieren von Callbacks
static PyObject *my_set_callback(PyObject *dummy, PyObject *args)
{
	PyObject *result = NULL;
	PyObject *temp;

	if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
		if (!PyCallable_Check(temp)) {
			PyErr_SetString(PyExc_TypeError, "parameter must be callable");
			return NULL;
		}
		Py_XINCREF(temp);         /* Add a reference to new callback */
		Py_XDECREF(my_callback);  /* Dispose of previous callback */
		my_callback = temp;       /* Remember new callback */
								  /* Boilerplate to return "None" */
		Py_INCREF(Py_None);
		result = Py_None;
	}
	return result;
}

static PyObject *consoleAction(PyObject *self, PyObject *args)
{


	
    int number;
	//printf("Test");
    // Argumente in einen String wandeln
    if (!PyArg_ParseTuple(args, "i", &number))
        return NULL;
	//printf("Test2");
    // die Callback-Funktion aufrufen
    //PyObject_CallObject(my_callback, args);
	//printf("Test3");
    // return a python integer object
    return Py_BuildValue("i", number);
	
}


static PyMethodDef ConsoleActionMethods[] = {
    {"action",  consoleAction, METH_VARARGS, "Print something in the console."},
    {"registerCallback", my_set_callback, METH_VARARGS, "Register a callback function."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


static struct PyModuleDef consoleActionmodule = {
    PyModuleDef_HEAD_INIT,
    "consoleAction",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
               or -1 if the module keeps state in global variables. */
    ConsoleActionMethods
};

PyMODINIT_FUNC PyInit_consoleAction(void)
{
    return PyModule_Create(&consoleActionmodule);
}

int main(int argc, char *argv[])
{    
    /* Add a built-in module, before Py_Initialize */
    PyImport_AppendInittab("consoleAction", PyInit_consoleAction);
    
    /* Pass argv[0] to the Python interpreter */
    //Py_SetProgramName(argv[0]);
    
    /* Initialize the Python interpreter.  Required. */
    Py_Initialize();
    
    /* Optionally import the module; alternatively,
     import can be deferred until the embedded script
     imports it. */
    PyImport_ImportModule("consoleAction");
    
}