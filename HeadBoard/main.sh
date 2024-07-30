#!/bin/bash

# Command that will compile the main files of CommandHandling, FlightData and PowerSystem
# into an executable. It will then run that executable.

# May need to be converted to make/cmake later on depending on how complicated this gets

# Clear all pipes
rm -rf /tmp/hoplite/

# Activate for Radio virtual environment
source ./hoplite_env/bin/activate

# Compile IPCQueue python module
# cd ./lib/IPCQueue
# chmod u+x compile_bindings.sh
# ./compile_bindings.sh
# cd ../../

# Start cmd handler
echo "Main: initializing CMD Handler..." 
g++ CommandHandler.cpp ./lib/IPCQueue/IPCQueue.cpp -o CommandHandler && ./CommandHandler &

sleep 1

# Start astraeus
cd ./AstraeusHandler
echo "Main: initializing Astraeus Handler..."
g++ AstraeusHandler.cpp ../lib/IPCQueue/IPCQueue.cpp -lwiringPi -o AstraeusHandler && ./AstraeusHandler &
cd ..

sleep 1

# Start radio
cd ./Radio
echo "Main: initializing Radio..."
python3 Radio.py &
cd ..

# Kill all processes on exit
trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM EXIT

# Don't exit till ctrl+c
sleep infinity