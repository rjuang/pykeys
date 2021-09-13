from distutils.core import setup, Extension

def main():
    setup(name="pykeys",
          version="2021.09.12-2227",
          description="Python module for injecting keyboard events.",
          author="Ray Juang",
          author_email="rayjuang@gmail.com",
          ext_modules=[
            Extension(
              "pykeys",
               sources=["pykeys.cc"],
               extra_compile_args=["-arch", "x86_64", "-arch", "arm64"],
               extra_link_args=["-arch", "x86_64", "-arch", "arm64"],
            )])

if __name__ == "__main__":
    main()
