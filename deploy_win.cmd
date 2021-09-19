@echo off

echo "Cleaning up old builds..."
del /f /s /q build

echo "Building for win64 platform"
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
python setup.py build -f --plat-name win-amd64

echo "Building for win32 platform"
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars32.bat"
python setup.py build -f --plat-name win32

xcopy build\lib.win32-3.9 lib.win32-3.9 /EY
xcopy build\lib.win-amd64-3.9 lib.win-amd64-3.9 /EY
copy build\lib.win-amd64-3.9\* dist
