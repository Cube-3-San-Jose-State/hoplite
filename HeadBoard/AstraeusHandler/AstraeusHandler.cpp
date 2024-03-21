/**
 * Handles data received from the Astraeus board.
 * Responsiblities: Receives Astraeus packages. Monitor 
*/

#include <stdio.h>      // Used for printing for console
#include <string>     // Used to manage strings
#include <fcntl.h>      // fdUsed for file control
#include <sys/stat.h>   // Used for printing file info
#include <sys/types.h>  // Used for extra type capabilities
#include <unistd.h>     // Used for extra type capabilities
#include <cstdlib>
#include <signal.h>
#include "../lib/pipe.h"

Pipe incoming_astraeus("/tmp/astraeus_to_main_pipe");

void close_pipes_on_exit(int signum){
    printf("\nCMD Handler: Exit detected. Closing pipes...\n");
    close(incoming_astraeus.status);
    exit(signum);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, close_pipes_on_exit);

    /* Temp code for testing pipes... */
    printf("Astraeus Handler: waiting for Astraeus end...\n");
    incoming_astraeus.status = open(incoming_astraeus.path, O_RDONLY | O_NONBLOCK);
    printf("Astraeus Handler: Astraeus pipe opened!\n");

    while(1) {
        if ( read(incoming_astraeus.status, incoming_astraeus.incoming, 128) > 0){
            printf("Astraeus Handler: from CMD Handler: received command: %s\n", incoming_astraeus.incoming);
        }
    }
    return 0;
}