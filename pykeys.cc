#include <map>
#include <string>

#ifdef __APPLE__
    // OS X platform. No support for iOS
    #include <ApplicationServices/ApplicationServices.h>
    #include <Carbon/Carbon.h>

#elif defined _WIN32 || defined _WIN64
    // Windows platform
#endif

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define VERSION "2021.09.12-2227"

extern "C" {

class ModifierParams {
  public:
    bool shift_key;
    bool command_win_key;
    bool control_key;
    bool option_alt_key;

    ModifierParams() :
      shift_key(false),
      command_win_key(false),
      control_key(false),
      option_alt_key(false) {}
};

////////// HELPER FUNCTIONS ////////////////////

void inject_key(CGKeyCode keycode, const ModifierParams &modifiers, bool is_down) {
  CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
  CGEventRef keyevent = CGEventCreateKeyboardEvent(source, keycode, is_down);

  if (modifiers.shift_key) {
    CGEventSetFlags(keyevent, kCGEventFlagMaskShift);
  }

  if (modifiers.command_win_key) {
    CGEventSetFlags(keyevent, kCGEventFlagMaskCommand);
  }

  if (modifiers.control_key) {
    CGEventSetFlags(keyevent, kCGEventFlagMaskControl);
  }

  if (modifiers.option_alt_key) {
    CGEventSetFlags(keyevent, kCGEventFlagMaskAlternate);
  }

  CGEventPost(kCGAnnotatedSessionEventTap, keyevent);
  CFRelease(keyevent);
  CFRelease(source);
}


///////// PYTHON METHODS ////////////////////////
static PyObject *method_version(PyObject *self, PyObject *args) {
  // Returns a string containing the current version
  return PyUnicode_FromString(VERSION);
}

const std::map<std::string, CGKeyCode> LOOKUP_TABLE = {
  {"return", kVK_Return},
  {"tab", kVK_Tab},
  {"space", kVK_Space},
  {"backspace", kVK_Delete},
  {"bksp", kVK_Delete},
  {"delete", kVK_ForwardDelete},
  {"del", kVK_ForwardDelete},
  {"escape", kVK_Escape},
  {"esc", kVK_Escape},
  {"volumeup", kVK_VolumeUp},
  {"volup", kVK_VolumeUp},
  {"volumedown", kVK_VolumeDown},
  {"voldown", kVK_VolumeDown},
  {"mute", kVK_Mute},
  {"f1", kVK_F1},
  {"f2", kVK_F2},
  {"f3", kVK_F3},
  {"f4", kVK_F4},
  {"f5", kVK_F5},
  {"f6", kVK_F6},
  {"f7", kVK_F7},
  {"f8", kVK_F8},
  {"f9", kVK_F9},
  {"f10", kVK_F10},
  {"f11", kVK_F11},
  {"f12", kVK_F12},
  {"home", kVK_Home},
  {"pageup", kVK_PageUp},
  {"pgup", kVK_PageUp},
  {"end", kVK_End},
  {"pagedown", kVK_PageDown},
  {"pgdn", kVK_PageDown},
  {"leftarrow", kVK_LeftArrow},
  {"left", kVK_LeftArrow},
  {"rightarrow", kVK_RightArrow},
  {"right", kVK_RightArrow},
  {"downarrow", kVK_DownArrow},
  {"down", kVK_DownArrow},
  {"uparrow", kVK_UpArrow},
  {"up", kVK_UpArrow},
  {"a", kVK_ANSI_A},
  {"b", kVK_ANSI_B},
  {"c", kVK_ANSI_C},
  {"d", kVK_ANSI_D},
  {"e", kVK_ANSI_E},
  {"f", kVK_ANSI_F},
  {"g", kVK_ANSI_G},
  {"h", kVK_ANSI_H},
  {"i", kVK_ANSI_I},
  {"j", kVK_ANSI_J},
  {"k", kVK_ANSI_K},
  {"l", kVK_ANSI_L},
  {"m", kVK_ANSI_M},
  {"n", kVK_ANSI_N},
  {"o", kVK_ANSI_O},
  {"p", kVK_ANSI_P},
  {"q", kVK_ANSI_Q},
  {"r", kVK_ANSI_R},
  {"s", kVK_ANSI_S},
  {"t", kVK_ANSI_T},
  {"u", kVK_ANSI_U},
  {"v", kVK_ANSI_V},
  {"w", kVK_ANSI_W},
  {"x", kVK_ANSI_X},
  {"y", kVK_ANSI_Y},
  {"z", kVK_ANSI_Z},
  {"1", kVK_ANSI_1},
  {"2", kVK_ANSI_2},
  {"3", kVK_ANSI_3},
  {"4", kVK_ANSI_4},
  {"5", kVK_ANSI_5},
  {"6", kVK_ANSI_6},
  {"7", kVK_ANSI_7},
  {"8", kVK_ANSI_8},
  {"9", kVK_ANSI_9},
  {"0", kVK_ANSI_0},
  {"=", kVK_ANSI_Equal},
  {"-", kVK_ANSI_Minus},
  {"minus", kVK_ANSI_Minus},
  {"+", kVK_ANSI_KeypadPlus},
  {"plus", kVK_ANSI_KeypadPlus},
  {"/", kVK_ANSI_Slash},
  {"[", kVK_ANSI_LeftBracket},
  {"]", kVK_ANSI_RightBracket},
  {".", kVK_ANSI_Period},
  {",", kVK_ANSI_Comma},
  {"'", kVK_ANSI_Quote},
  {";", kVK_ANSI_Semicolon},
  {"\\", kVK_ANSI_Backslash},
  {"backslash", kVK_ANSI_Backslash},
  {"`", kVK_ANSI_Grave},
};

static PyObject *method_send(PyObject *self, PyObject *args) {
  char *key = nullptr;
  int shift = 0;
  int command_win = 0;
  int control = 0;
  int option_alt = 0;

  if (!PyArg_ParseTuple(args, "spppp", &key, &shift, &command_win, &control, &option_alt)) {
    return NULL;
  }
  std::string keystr = key;
  if (LOOKUP_TABLE.count(keystr) == 0) {
    return NULL;
  }
  CGKeyCode keycode = LOOKUP_TABLE.at(keystr);
  ModifierParams modifiers;
  modifiers.shift_key = shift > 0;
  modifiers.command_win_key = command_win > 0;
  modifiers.control_key = control > 0;
  modifiers.option_alt_key = option_alt > 0;
  PySys_WriteStdout(
    "Sending 0x%x [shift=%d, cmd=%d, ctrl=%d, alt=%d]\n",
    keycode, shift, command_win, control, option_alt);

  inject_key(keycode, modifiers, true); 
  inject_key(keycode, modifiers, false); 
  return PyLong_FromLong(1);
}


////// MODULE DEFINITION ///////
static PyMethodDef moduleMethods[] = {
  {"version", method_version, METH_VARARGS, "Returns the current version number of the library."},
  {"send", method_send, METH_VARARGS, "Injects key down/up events to simulate a key press."},
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


