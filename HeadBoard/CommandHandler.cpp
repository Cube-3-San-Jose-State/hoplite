/**
 * Main - Command Handler:
 * Handles communication between all processes. Executes commands escalated from Radio.
*/
#include <stdio.h>      // Used for printing for console
#include <string>     // Used to manage strings
#include <fcntl.h>      // fdUsed for file control
#include <sys/stat.h>   // Used for printing file info
#include <sys/types.h>  // Used for extra type capabilities
#include <unistd.h>     // Used for extra type capabilities
#include <cstdlib>
#include <signal.h>
#include <iostream>
#include "./lib/json.hpp"
#include "./lib/pipe.h"
#include "./lib/IPCQueue/IPCQueue.h"

IPCQueue radio("radio");
IPCQueue astraeus("astraeus");
Pipe outgoing_scheduler("/tmp/scheduler_to_main_pipe");

// Opens each necessary pipe end.
void connectPipes(){
    // incoming_radio.status = open(incoming_radio.path, O_RDONLY | O_NONBLOCK);
    // outgoing_radio.status = open(outgoing_radio.path, O_WRONLY | O_NONBLOCK);
    outgoing_scheduler.status  = open(outgoing_scheduler.path, O_WRONLY | O_NONBLOCK);
}

void close_pipes_on_exit(int signum){
    std::cout << "CMD Handler: Exit detected. Closing pipes...\n";
    // close(incoming_radio.status);
    // close(outgoing_radio.status);
    close(outgoing_scheduler.status);
    exit(signum);
}

void handlePacket( std::string received ) {
    nlohmann::json parsed = nlohmann::json::parse(received.c_str());
    std::string parsed_data = parsed.dump();
    std::cout << "CMD Handler: parsed command packet: " << parsed_data <<'\n';
    
    std::string process = parsed["process"];
    if (process == "ASTRAEUS") {
        if (parsed["dir"] == "INCOMING") {
            // TODO: forward data to radio

        } else if (parsed["dir"] == "OUTGOING") {
            // TODO: send request to astraeus handler
            astraeus.write(parsed_data);
        }
    } else if (process == "SCHEDULER") {
        if (parsed["dir"] == "INCOMING") {
            // TODO: does the scheduler ever send us anything directly?
        } else if (parsed["dir"] == "OUTGOING") {
            // TODO: forward setting to scheduler
        }
    } else if (process == "RADIO") {
        if (parsed["dir"] == "INCOMING") {
            // TODO: does the radio ever send us anything directly?
        } else if (parsed["dir"] == "OUTGOING") {
            // TODO: send data to downlink
            // write(outgoing_radio.status, joe.c_str(), joe.size()+1);
        }
    } else {
        std::cout << "CMD Handler: command process not found";
    }
}

int main(int argc, char const *argv[])
{
    // Closes pipes when interrupted.
    signal(SIGINT, close_pipes_on_exit);

    // Open all neccessary pipe ends.
    connectPipes();
    std::cout << "CMD Handler: All pipes up and running.\n";
    while (1)
    {
        // Listen for any input from radio.
        std::string incoming_radio = radio.read();
        if ( incoming_radio.length() > 0 ) {
            handlePacket( incoming_radio );
        }
        sleep(1);
    }
    return 0;
}
