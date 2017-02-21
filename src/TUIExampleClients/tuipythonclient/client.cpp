#include "Python.h"

//#include "GripperWithLEDUsingTracker.h"
//#include "GripperWithLEDUsingMatrix4.h"
//#include "GripperWithLEDUsingMatrix4x4.h"
//#include "BluetoothPen.h"
#include "TUIMouse.h"

#include <tuiframework/client/client.h>
//#include "client.h"

#include <iostream>
#include <stdlib.h>
#include <cstdio>

using namespace tuiframework;
using namespace std;

class TestClientClass : public IEventSink {
public:
	virtual void push(IEvent * event) {
		if (event->getEventTypeID() == SystemMsg::EventTypeID()) {
			SystemMsg * msg = static_cast<SystemMsg *>(event);
			if (msg->getPayload() == CONNECTION_ESTABLISHED) {
				cout << "CONNECTION ESTABLISHED" << endl;

				this->tuiMouse = new TUIMouse();
				//this->bluetoothPen = new BluetoothPen() ;
				//this->gripperWithLEDUsingMatrix4x4 = new GripperWithLEDUsingMatrix4x4();
			}
		}
	}

protected:
	//GripperWithLEDUsingTracker * gripperWithLEDUsingTracker;
	//GripperWithLEDUsingMatrix4 * gripperWithLEDUsingMatrix4;
	//GripperWithLEDUsingMatrix4x4 * gripperWithLEDUsingMatrix4x4;
	//BluetoothPen * bluetoothPen;
	TUIMouse *tuiMouse;
};

PyMODINIT_FUNC PyInit_PyTUIClient(void);
static PyObject *my_callback = NULL;

static PyObject *triggerCallback(PyObject *self, PyObject *args)
{
    // die Callback-Funktion aufrufen
    PyObject_CallObject(my_callback, NULL);
    
    // return a python integer object
    return Py_BuildValue("s", "callback triggered");
}

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

static PyMethodDef PyTUIClientMethods[] = {
    {"triggerCallback",  triggerCallback, METH_VARARGS, "Trigger a Test-Callback."},
    {"registerCallback", my_set_callback, METH_VARARGS, "Register a callback function."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef PyTUIClientModule = {
    PyModuleDef_HEAD_INIT,
    "PyTUIClient",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
               or -1 if the module keeps state in global variables. */
    PyTUIClientMethods
};

PyMODINIT_FUNC PyInit_PyTUIClient(void)
{
    return PyModule_Create(&PyTUIClientModule);
}




int main(int argc, char* argv[]) {
    
    /////////////// PYTHON STUFF /////////////////////
    
    /* Add a built-in module, before Py_Initialize */
    PyImport_AppendInittab("PyTUIClient", PyInit_PyTUIClient);
    
    /* Pass argv[0] to the Python interpreter */
    //Py_SetProgramName(argv[0]);
    
    /* Initialize the Python interpreter.  Required. */
    Py_Initialize();
    
    /* Optionally import the module; alternatively,
     import can be deferred until the embedded script
     imports it. */
    PyImport_ImportModule("PyTUIClient");
    
    ///////////////////////////////////////
    
    if (argc != 4) {
        cout << "Usage tuiclient <sender port> <receiver port> <ipaddress:port>" << endl;
        return 1;
    }

    TestClientClass testClientClass;

	//initTypeRegistration(getEventFactory());
	//CommonTypeReg::registerTypes(&getEventFactory(), &getEventChannelFactory());

    //connectWithTUIServer(atoi(argv[1]), atoi(argv[2]), argv[3], &testClientClass);

//    getchar();
    return 0;
}