# pykeys
Python Module (C-extension based) for injecting Keyboard events. This is purposely a C-extension based plugin so that it can be easily included in FL Studio Midi Scripts. 

# Goals of this Project

To create self-contained libraries that can be included in FL Studio (Mac and Windows) midi scripts (and possibly distributed with FL Studio if Image-Line chooses) to enable properly injecting keyboard events.

# Buildining
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
