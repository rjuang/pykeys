#include <iostream>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define VERSION "2021.09.12-2227"

extern "C" {

static PyObject *method_version(PyObject *self, PyObject *args) {
  // Returns a string containing the current version
  return PyUnicode_FromString(VERSION);
}


////// MODULE DEFINITION ///////
static PyMethodDef moduleMethods[] = {
  {"version", method_version, METH_VARARGS, "Returns the current version number of the library."},
  {NULL, NULL, 0, NULL},
};

static struct PyModuleDef moduleDef = {
  PyModuleDef_HEAD_INIT,
  "pykeys",
  "Python library to inject keyboard events",
  -1,
  moduleMethods,
};

PyMODINIT_FUNC
PyInit_pykeys(void) {
  return PyModule_Create(&moduleDef);
}
}


