#include <map>
#include <string>
#include <vector>

#ifdef __APPLE__
    // OS X platform. No support for iOS
    #include <ApplicationServices/ApplicationServices.h>
    #include <Carbon/Carbon.h>

    #define PLATFORM_DESC "mac"
#elif defined _WIN32 || defined _WIN64
    // Windows platform
    #include <windows.h>
    #include <winuser.h>
    #define PLATFORM_DESC "win"
    #define __WINDOWS__
    #define PYTHONPATH L".\\DLLs;.\\lib;..\\"
    #define Py_BUILD_CORE
    #pragma comment(lib,"PyBridge_x64.lib")
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
#ifdef __APPLE__
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
#endif
#ifdef __WINDOWS__
INPUT get_key(WORD keycode, bool is_down) {
    INPUT key;
    key.type = INPUT_KEYBOARD;
    key.ki.wVk = keycode;
    if (!is_down) {
      key.ki.dwFlags = KEYEVENTF_KEYUP;
    }
    return key;
}

bool inject_keypress(WORD keycode, const ModifierParams &modifiers) {
  int numKeys = 1;
  numKeys += int(modifiers.shift_key);
  numKeys += int(modifiers.command_win_key);
  numKeys += int(modifiers.control_key);
  numKeys += int(modifiers.option_alt_key);
  std::vector<INPUT> inputs(2 * numKeys);

  if (modifiers.shift_key) {
    inputs.push_back(get_key(VK_LSHIFT, true));
  }

  if (modifiers.command_win_key) {
    inputs.push_back(get_key(VK_LWIN, true));
  }

  if (modifiers.control_key) {
    inputs.push_back(get_key(VK_LCONTROL, true));
  }

  if (modifiers.option_alt_key) {
    inputs.push_back(get_key(VK_MENU, true));
  }

  inputs.push_back(get_key(keycode, true));
  inputs.push_back(get_key(keycode, false));

  if (modifiers.shift_key) {
    inputs.push_back(get_key(VK_LSHIFT, false));
  }

  if (modifiers.command_win_key) {
    inputs.push_back(get_key(VK_LWIN, false));
  }

  if (modifiers.control_key) {
    inputs.push_back(get_key(VK_LCONTROL, false));
  }

  if (modifiers.option_alt_key) {
    inputs.push_back(get_key(VK_MENU, false));
  }
  UINT numSent = SendInput(inputs.size(), &inputs[0], sizeof(INPUT));
  return numSent == inputs.size();
}
#endif

///////// PYTHON METHODS ////////////////////////
static PyObject *method_version(PyObject *self, PyObject *args) {
  // Returns a string containing the current version
  return PyUnicode_FromString(VERSION);
}

#ifdef __APPLE__
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
#endif
#ifdef __WINDOWS__
const std::map<std::string, WORD> LOOKUP_TABLE = {
  {"return", VK_RETURN},
  {"tab", VK_TAB},
  {"space", VK_SPACE},
  {"backspace", VK_BACK},
  {"bksp", VK_BACK},
  {"delete", VK_DELETE},
  {"del", VK_DELETE},
  {"escape", VK_ESCAPE},
  {"esc", VK_ESCAPE},
  {"volumeup", VK_VOLUME_UP},
  {"volup", VK_VOLUME_UP},
  {"volumedown", VK_VOLUME_DOWN},
  {"voldown", VK_VOLUME_DOWN},
  {"mute", VK_VOLUME_MUTE},
  {"f1", VK_F1},
  {"f2", VK_F2},
  {"f3", VK_F3},
  {"f4", VK_F4},
  {"f5", VK_F5},
  {"f6", VK_F6},
  {"f7", VK_F7},
  {"f8", VK_F8},
  {"f9", VK_F9},
  {"f10", VK_F10},
  {"f11", VK_F11},
  {"f12", VK_F12},
  {"home", VK_HOME},
  {"pageup", VK_PRIOR},
  {"pgup", VK_PRIOR},
  {"end", VK_END},
  {"pagedown", VK_NEXT},
  {"pgdn", VK_NEXT},
  {"leftarrow", VK_LEFT},
  {"left", VK_LEFT},
  {"rightarrow", VK_RIGHT},
  {"right", VK_RIGHT},
  {"downarrow", VK_DOWN},
  {"down", VK_DOWN},
  {"uparrow", VK_UP},
  {"up", VK_UP},
  {"a", 0x41},
  {"b", 0x42},
  {"c", 0x43},
  {"d", 0x44},
  {"e", 0x45},
  {"f", 0x46},
  {"g", 0x47},
  {"h", 0x48},
  {"i", 0x49},
  {"j", 0x4A},
  {"k", 0x4B},
  {"l", 0x4C},
  {"m", 0x4D},
  {"n", 0x4E},
  {"o", 0x4F},
  {"p", 0x50},
  {"q", 0x51},
  {"r", 0x52},
  {"s", 0x53},
  {"t", 0x54},
  {"u", 0x55},
  {"v", 0x56},
  {"w", 0x57},
  {"x", 0x58},
  {"y", 0x59},
  {"z", 0x5A},
  {"1", 0x30},
  {"2", 0x31},
  {"3", 0x32},
  {"4", 0x33},
  {"5", 0x34},
  {"6", 0x35},
  {"7", 0x36},
  {"8", 0x37},
  {"9", 0x38},
  {"0", 0x39},
  {"=", VK_OEM_PLUS},
  {"-", VK_OEM_MINUS},
  {"minus", VK_OEM_MINUS},
  {"+", VK_OEM_PLUS},
  {"plus", VK_OEM_PLUS},
  {"/", VK_OEM_2},
  {"[", VK_OEM_4},
  {"]", VK_OEM_6},
  {".", VK_OEM_PERIOD},
  {",", VK_OEM_COMMA},
  {"'", VK_OEM_7},
  {";", VK_OEM_1},
  {"\\", VK_OEM_5},
  {"backslash", VK_OEM_5},
  {"`", VK_OEM_3},
};
#endif

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

  #ifdef __APPLE__
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
  int return_value = 1;
  #endif

  #ifdef __WINDOWS__
  WORD keycode = LOOKUP_TABLE.at(keystr);
  ModifierParams modifiers;
  modifiers.shift_key = shift > 0;
  modifiers.command_win_key = command_win > 0;
  modifiers.control_key = control > 0;
  modifiers.option_alt_key = option_alt > 0;
  PySys_WriteStdout(
    "Sending 0x%x [shift=%d, cmd=%d, ctrl=%d, alt=%d]\n",
    keycode, shift, command_win, control, option_alt);
  int return_value = inject_keypress(keycode, modifiers); 
  #endif

  return PyLong_FromLong(return_value);
}

static PyObject *method_platform(PyObject *self, PyObject *args) {
  return PyUnicode_FromString(PLATFORM_DESC);
}


////// MODULE DEFINITION ///////
static PyMethodDef moduleMethods[] = {
  {"version", method_version, METH_VARARGS, "Returns the current version number of the library."},
  {"platform", method_platform, METH_VARARGS, "Returns a string specifying the OS platform."},
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
  return PyModule_Create2(&moduleDef, PYTHON_API_VERSION);
}
}


