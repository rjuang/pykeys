# pykeys
Python Module (C-extension based) for injecting Keyboard events. This is purposely a C-extension based plugin so that it can be easily included in FL Studio Midi Scripts. 

# Goals of this Project

To create self-contained libraries that can be included in FL Studio (Mac and Windows) midi scripts (and possibly distributed with FL Studio if Image-Line chooses) to enable properly injecting keyboard events.

# How to Use

Copy the pyd files in the lib.* folders into your midiscript folder. 

To use, your could would do the following:
```
import pykeys


# Assign the modifier key 0 for not pressed, and 1 if pressed.
shift = 0
cmd_win = 0    # Win key on windows and Command key on Macs
ctrl = 0       # Ctrl key on both win/mac
opt_alt = 0    # Alt key on windows and Option key on Macs

retValue = pykeys.send('f6', shift, cmd_win, ctrl, opt_alt)
# Returns true if successfully sent.

```

# Building
The command to build is:
```
python setup.py build_ext -f
```

To specify a specific platform, use `--plat-name`.

# Windows Notes
If PyBridge define files are updated, you can update the library with the following commands:

```
lib /def:PyBridge.def /out:PyBridge.lib /machine:x86
lib /def:PyBridge_x64.def /out:PyBridge_x64.lib /machine:x64
```

See [reference](https://stackoverflow.com/questions/9360280/how-to-make-a-lib-file-when-have-a-dll-file-and-a-header-file)

- FL's PyBridge is missing the symbol `PyModule_Create`. Need to use `PyModule_Create2` instead
