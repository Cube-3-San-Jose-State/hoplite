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
#include <iostream>
#include "../lib/pipe.h"
#include "../lib/IPCQueue/IPCQueue.h"

IPCQueue cmd_handler("astraeus");

int main(int argc, char const *argv[])
{
    printf("Astraeus Handler: OK!\n");
    while(1) {
        std::string received = cmd_handler.read();        
        if ( received.length() > 0) {
            printf("Astraeus handler: received: %s\n", received.c_str());
        }
        sleep(1);
    }

    return 0;
}