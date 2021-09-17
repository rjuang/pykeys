import sys
import struct

from distutils.core import setup, Extension

def main():
    if sys.platform == "win32":
        extra_compile_args = []
        extra_linker_args = []
        libraries=["user32"] 
    else:
        extra_compile_args = [
            "-arch", "x86_64",
            "-arch", "arm64",
            "-std=c++11",
        ]
        extra_linker_args = [
            "-arch", "x86_64",
            "-arch", "arm64",
            "-framework", "ApplicationServices",
            "-framework", "Carbon",
        ]
        libraries=[]

    setup(name="pykeys",
          version="2021.09.12-2227",
          description="Python module for injecting keyboard events.",
          author="Ray Juang",
          author_email="rayjuang@gmail.com",
          ext_modules=[
            Extension(
              "pykeys",
               sources=["pykeys.cc"],
               libraries=libraries,
               extra_compile_args=extra_compile_args,
               extra_link_args=extra_linker_args,
            )])

if __name__ == "__main__":
    main()
