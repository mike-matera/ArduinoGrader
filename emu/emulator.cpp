#include <Python.h>

#include "emulator.h" 
#include "Arduino.h"

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <fcntl.h>
#include <stdarg.h>

#include <string>
#include <map>

extern "C" {
#include "stdlib.h"
}

static PyObject *__emu; 
static PyObject *__test;

static PyObject* setup_linkage(PyObject *self, PyObject *args) {
  setup();
  return NULL;
}

static PyObject* loop_linkage(PyObject *self, PyObject *args) {
  loop();
  return NULL;
}

static PyMethodDef ArduinoMethods[] = {
  {"setup", setup_linkage, METH_NOARGS, "Arduino's setup() function."}, 
  {"loop", loop_linkage, METH_NOARGS, "Arduino's loop() function."}, 
  {NULL, NULL, 0, NULL}
};

static PyModuleDef ArduinoModule = {
  PyModuleDef_HEAD_INIT, "avr", NULL, -1, ArduinoMethods, 
  NULL, NULL, NULL, NULL
};

static PyObject *PyInit_arduino() {
  return PyModule_Create(&ArduinoModule);
}

string emu_get_property(const string &key) {
  return "";
}

void emu_set_property(const string &key, const string &val) {
}

void emu_set_pinmode(int pin, PinMode mode) {
}

void emu_set_pinvalue(int pin, int value) {
}

PinMode emu_get_pinmode(int pin) {
  return kPullup;
}

int emu_get_pinvalue(int pin) {
  return 0;
}

PyObject *__call_emu(string member, std::map<string,string> kwargs) {
  PyObject *sub = PyObject_GetAttrString(__emu, member.c_str());
  PyObject *dict = PyDict_New(); 
  for ( auto ent : kwargs ) {
    PyObject *val = PyUnicode_FromString(ent.second.c_str());
    PyDict_SetItemString(dict, ent.first.c_str(), val);    
    Py_DECREF(val);
  }
  PyObject *tuple = PyTuple_New(0);
  PyObject_Call(sub, tuple, dict);
  PyErr_Print();
  Py_DECREF(tuple);
  Py_DECREF(dict);
  Py_DECREF(sub);
  return NULL;
}

void test_run() { 
}

#define STACKTRACE() {							\
    void *_sf[100];							\
    backtrace_symbols_fd(_sf, backtrace(_sf, 100), fileno(stdout));	\
  }

int main(int argc, char *argv[])
{
  PyObject *name, *test_mod, *emu_mod;

  if (argc < 1) {
    fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
    return 1;
  }

  PyImport_AppendInittab("arduino", &PyInit_arduino);
  Py_Initialize();
  name = PyUnicode_FromString(argv[1]);
  test_mod = PyImport_Import(name);
  Py_DECREF(name);

  // Make a reference to the emulator class instance
  name = PyUnicode_FromString("pytest.emu");
  emu_mod = PyImport_Import(name);
  __emu = PyObject_GetAttrString(emu_mod, "emu");
  Py_DECREF(name);
  
  // Test
  __call_emu("get_property", {{"fuck", "you" }});
        
  // Retreive the emulator functions.
  __test = PyObject_GetAttrString(test_mod, "test_run");
  PyErr_Clear();
  if (__test && PyCallable_Check(__test)) {
    PyObject *rval = PyObject_CallObject(__test, NULL);
    PyErr_Print();
    PyErr_Clear();
    Py_XDECREF(rval);
  }
  PyErr_Print();
    
  Py_XDECREF(__test);
  Py_XDECREF(__emu);
  
  Py_XDECREF(test_mod);
  Py_XDECREF(emu_mod);
  
  Py_Finalize();
  
  return 0;
}
