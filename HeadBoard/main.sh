#!/bin/bash

# Command that will compile the main files of CommandHandling, FlightData and PowerSystem
# into an executable. It will then run that executable.

# May need to be converted to make/cmake later on depending on how complicated this gets

#Activate for Radio virtual environment
source ./hoplite_env/bin/activate

# Start cmd handler
echo "Main: initializing CMD Handler..." 
g++ CommandHandler.cpp -o CommandHandler && ./CommandHandler &
echo "Main: CMD Handler OK!"

# Start astraeus
cd ./AstraeusHandler
echo "Main: initializing Astraeus Handler..."
g++ AstraeusHandler.cpp -o AstraeusHandler && ./AstraeusHandler &
echo "Main: Astraeus Handler OK!"
cd ..

# Start radio
cd ./Radio
echo "Main: initializing Radio..."
python3 Radio.py &
echo "Main: Radio OK!"
cd ..

# Kill all processes on exit
trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM EXIT

# Don't exit till ctrl+c
sleep infinity