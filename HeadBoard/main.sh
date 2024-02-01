# Command that will compile the main files of CommandHandling, FlightData and PowerSystem
# into an executable. It will then run that executable.

# May need to be converted to make/cmake later on depending on how complicated this gets

echo "Initializing command handler..."
g++ CommandHandler.cpp -o CommandHandler && ./CommandHandler

# Initialize virtualenv to work with radio
# cd ~/cube/hoplite/HeadBoard/CommandHandling
# source ./RadioVirtualEnv/bin/activate
# python3 CommandHandling.py

