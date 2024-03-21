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
#include "./lib/pipe.h"

Pipe incoming_radio("/tmp/radio_to_main_pipe");
Pipe outgoing_astraeus("/tmp/astraeus_to_main_pipe");
Pipe outgoing_scheduler("/tmp/scheduler_to_main_pipe");

// Opens each necessary pipe end.
void connectPipes(){
    incoming_radio.status = open(incoming_radio.path, O_RDONLY | O_NONBLOCK);

    outgoing_astraeus.status = open(outgoing_astraeus.path, O_WRONLY | O_NONBLOCK);
    outgoing_scheduler.status  = open(outgoing_scheduler.path, O_WRONLY | O_NONBLOCK);

    printf("CMD Handler: All pipes up and running.");
}

void close_pipes_on_exit(int signum){
    printf("\nCMD Handler: Exit detected. Closing pipes...\n");
    close(incoming_radio.status);
    exit(signum);
}

int main(int argc, char const *argv[])
{
    // Closes pipes when interrupted.
    signal(SIGINT, close_pipes_on_exit);

    // Open all neccessary pipe ends.
    connectPipes();

    std::string astraeus_test_request = "{process:\"ASTRAEUS\",dir:\"OUTGOING\",function:\"REQ_TEL\",arguments:[11010111]}";
    // TODO: make this send a proper time
    std::string scheduler_test_request = "{process:\"SCHEDULER\",dir:\"OUTGOING\",function:\"SCHED_ONCE\",arguments:[]}";
    while (1)
    {
        // Listen for any input from radio.
        if ( read(incoming_radio.status, incoming_radio.incoming, 128) > 0){
            printf("CMD Handler: from Radio: received command: %s\n", incoming_radio.incoming);
        }

        write(outgoing_scheduler.status, )
        write(outgoing_astraeus.status, astraeus_test_requesttest_request.c_str(), astraeus_test_requesttest_request.size()+1);
        sleep(1);
    }
    return 0;
}
