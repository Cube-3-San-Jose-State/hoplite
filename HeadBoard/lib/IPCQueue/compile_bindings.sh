#! /bin/bash

# exit immediately upon error
set -e

# compile IPCQueue module
echo "compiling module..."
g++ -O3 -Wall -shared -std=c++17 -fPIC $(python3 -m pybind11 --includes) IPCQueue_py_bindings.cpp IPCQueue.cpp -o IPCQueue$(python3-config --extension-suffix)