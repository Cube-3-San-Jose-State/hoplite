/**
 * Main - Command Handler:
 * Handles communication between all processes. Executes commands escalated from Radio.
*/
#include <stdio.h>      // Used for printing for console
#include <string.h>     // Used to manage strings
#include <fcntl.h>      // fdUsed for file control
#include <sys/stat.h>   // Used for printing file info
#include <sys/types.h>  // Used for extra type capabilities
#include <unistd.h>     // Used for extra type capabilities
#include <cstdlib>
#include <signal.h>
#include "./lib/pipe.h"

Pipe incoming_radio("/tmp/radio_to_main_pipe");

void close_pipes_on_exit(int signum){
    printf("\nCMD Handler: Exit detected. Closing pipes...\n");
    close(incoming_radio.status);
    exit(signum);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, close_pipes_on_exit);

    printf("CMD Handler: waiting for radio end...\n");
    incoming_radio.status = open(incoming_radio.path, O_RDONLY | O_NONBLOCK);
    printf("CMD Handler: Radio pipe opened!\n");

    while (1)
    {
        // First open in read only and read
        if ( read(incoming_radio.status, incoming_radio.incoming, 128) > 0){
            printf("CMD Handler: received command: %s\n", incoming_radio.incoming);
        }

        // Now open in write mode and write
        // string taken from user.
        // incoming_radio.status = open(incoming_radio.path, O_WRONLY);
        // fgets(incoming_radio.outgoing, 80, stdin);
        // write(incoming_radio.status, incoming_radio.outgoing, strlen(incoming_radio.outgoing)+1);
        // close(incoming_radio.status);
        sleep(1);
    }
    return 0;
}

