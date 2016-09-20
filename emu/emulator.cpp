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
  return Py_None;
}

static PyObject* loop_linkage(PyObject *self, PyObject *args) {
  loop();
  return Py_None;
}

static PyMethodDef SketchMethods[] = {
  {"setup", setup_linkage, METH_NOARGS, "Sketch's setup() function."}, 
  {"loop", loop_linkage, METH_NOARGS, "Sketch's loop() function."}, 
  {NULL, NULL, 0, NULL}
};

static PyModuleDef SketchModule = {
  PyModuleDef_HEAD_INIT, "", NULL, -1, SketchMethods, 
  NULL, NULL, NULL, NULL
};

static PyObject *PyInit_Sketch() {
  return PyModule_Create(&SketchModule);
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
  PyObject *rval = PyObject_Call(sub, tuple, dict);
  PyErr_Print();
  Py_DECREF(tuple);
  Py_DECREF(dict);
  Py_DECREF(sub);
  return rval;
}

string emu_get_property(const string &key) {
  PyObject *value = __call_emu("get_property", {{"key", key}});
  string rval = PyUnicode_AsUTF8(value);
  Py_DECREF(value);
  return rval;
}

void emu_set_property(const string &key, const string &val) {
  PyObject *none = __call_emu("set_property", {
      {"key", key}, {"value", val}
    });
  Py_DECREF(none);
}

void emu_set_pinmode(int pin, PinMode mode) {
  PyObject *none = __call_emu("set_pinmode", {
      {"pin", std::to_string(pin)}, 
	{"mode", std::to_string(mode)},
	  });	  
  Py_DECREF(none);
}

void emu_set_pinvalue(int pin, int value) {
  PyObject *none = __call_emu("set_pinvalue", {
      {"pin", std::to_string(pin)}, 
	{"value", std::to_string(value)},
	  });	  
  Py_DECREF(none);
}

PinMode emu_get_pinmode(int pin) {
  PyObject *mode = __call_emu("get_pinmode", {
      {"pin", std::to_string(pin)}, 
	});	  
  int rval = PyLong_AsLong(mode);
  Py_DECREF(mode);
  return kPullup;
}

int emu_get_pinvalue(int pin) {
  PyObject *value = __call_emu("get_pinvalue", {
      {"pin", std::to_string(pin)}, 
	});	  
  int rval = PyLong_AsLong(value);
  Py_DECREF(value);
  return rval;
}

#define STACKTRACE() {							\
    void *_sf[100];							\
    backtrace_symbols_fd(_sf, backtrace(_sf, 100), fileno(stdout));	\
  }

int main(int argc, char *argv[])
{
  PyObject *name, *test_mod, *emu_mod;

  if (argc < 1) {
    fprintf(stderr,"Usage: test test_module\n");
    return 1;
  }

  PyImport_AppendInittab("sketch", &PyInit_Sketch);
  Py_Initialize();
  name = PyUnicode_FromString(argv[1]);
  test_mod = PyImport_Import(name);
  Py_DECREF(name);

  // Make a reference to the emulator class instance
  name = PyUnicode_FromString("pytest.emu");
  emu_mod = PyImport_Import(name);
  __emu = PyObject_GetAttrString(emu_mod, "emu");
  Py_DECREF(name);
  
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
