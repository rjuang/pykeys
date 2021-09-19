#!/bin/bash
# Script to build and copy output to the correct locations.
python3 setup.py build -f
cp build/lib.*/*.so dist/
cp -r build/lib.* .
