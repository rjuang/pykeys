# pykeys
Python Module (C-extension based) for injecting Keyboard events. This is purposely a C-extension based plugin so that it can be easily included in FL Studio Midi Scripts.

# Goals of this Project

To create a self-contained libraries that can be included in FL Studio (Mac and Windows) midi scripts (and possibly distributed with FL Studio) to enable properly injecting keyboard events.

# Schedule
- Code to inject keyboard events in Mac OS X.
- Code to inject keyboard events in Windows.
- Documentation on how to use.


# Windows Notes
- Copy Python's lib file in `$PYTHON_DIR\libs\python39.lib` to `PyBridge_x64.lib`


```
lib /def:PyBridge.def /out:PyBridge.lib /machine:x86
lib /def:PyBridge_x64.def /out:PyBridge_x64.lib /machine:x64
```

See [reference](https://stackoverflow.com/questions/9360280/how-to-make-a-lib-file-when-have-a-dll-file-and-a-header-file)

- FL's PyBridge is missing the symbol `PyModule_Create`. Need to use `PyModule_Create2` instead
