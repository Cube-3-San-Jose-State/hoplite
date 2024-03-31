#! /bin/bash

# exit immediately upon error
set -e

BUILD_VENV=".build_env"

# check if venv was already created, create if not
if [ ! -d "$BUILD_VENV" ]; then
	echo "python virtual environment does not exist, creating..."
	virtualenv $BUILD_VENV
	source $BUILD_VENV/bin/activate
	pip install pybind11
else
	echo "Entering python environment..."
	source $BUILD_VENV/bin/activate
fi


# compile IPCQueue module
echo "compiling module..."
g++ -O3 -Wall -shared -std=c++17 -fPIC $(python3 -m pybind11 --includes) IPCQueue_py_bindings.cpp IPCQueue.cpp -o IPCQueue$(python3-config --extension-suffix)

# finish
GREEN='\033[0;32m'
NC='\033[0m'
deactivate
printf "\n\n${GREEN}All done!${NC}\n"
