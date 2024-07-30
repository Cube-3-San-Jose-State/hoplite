/**
 * Main - Command Handler:
 * Handles communication between all processes. Executes commands escalated from Radio.
 */
#include <stdio.h>     // Used for printing for console
#include <string>      // Used to manage strings
#include <fcntl.h>     // fdUsed for file control
#include <sys/stat.h>  // Used for printing file info
#include <sys/types.h> // Used for extra type capabilities
#include <unistd.h>    // Used for extra type capabilities
#include <cstdlib>
#include <signal.h>
#include <iostream>
#include "./lib/json.hpp"
#include "./lib/pipe.h"
#include "./lib/IPCQueue/IPCQueue.h"

IPCQueue radio("radio");
IPCQueue astraeus("astraeus");
IPCQueue scheduler("scheduler");

// Opens each necessary pipe end.
void handlePacket(std::string received)
{
    nlohmann::json parsed = nlohmann::json::parse(received.c_str());
    std::string parsed_data = parsed.dump();
    std::cout << "CMD Handler: parsed command packet: " << parsed_data << '\n';

    std::string process = parsed["process"];
    if (process == "ASTRAEUS")
    {
        if (parsed["dir"] == "INCOMING")
        { // CMD never gets a direct incoming from Astraeus Handler
            ;
        }
        else if (parsed["dir"] == "OUTGOING")
        { // Sends a telemetry request to Astraeus Handler
            astraeus.write(parsed_data);
        }
    }
    else if (process == "SCHEDULER")
    {
        if (parsed["dir"] == "INCOMING")
        { // CMD never gets a direct incoming from Scheduler
            ;
        }
        else if (parsed["dir"] == "OUTGOING")
        { // Sends scheduling setting request to Scheduler
            scheduler.write(parsed_data);
        }
    }
    else if (process == "RADIO")
    {
        if (parsed["dir"] == "INCOMING")
        { // CMD never gets a direct incoming from Radio
            ;
        }
        else if (parsed["dir"] == "OUTGOING")
        { // Sends a downlink data request to Radio
            radio.write(parsed_data);
        }
    }
    else
    {
        std::cout << "CMD Handler: invalid command";
    }
}

int main(int argc, char const *argv[])
{
    while (1)
    {
        // Listen for any input from radio.
        std::string incoming_radio = radio.read();
        if (incoming_radio.length() > 0)
        {
            handlePacket(incoming_radio);
        }
        sleep(1);
    }
    return 0;
}
