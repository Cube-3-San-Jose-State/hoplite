/**
 * Handles data received from the Astraeus board.
 * Responsiblities: Receives Astraeus packages. Monitor
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
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "../lib/pipe.h"
#include "../lib/IPCQueue/IPCQueue.h"
#include <string.h>

IPCQueue cmd_handler("astraeus");

int main(int argc, char const *argv[])
{
    int fd;

    if ((fd = serialOpen("/dev/ttyACM0", 115200)) < 0)
    {
        fprintf(stderr, "Astraeus handler: Unable to open serial device: %s\n", strerror(errno));
        return 1;
    }

    if (wiringPiSetup() == -1)
    {
        fprintf(stdout, "Astraeus handler: unable to start writingpi");
        return 1;
    }

    printf("Astraeus Handler: OK!\n");
    while (1)
    {
        if (serialDataAvail(fd))
        {
            printf("Astraeus handler: got: ");
            while (serialDataAvail(fd))
            {
                std::cout << serialGetchar(fd);
                fflush(stdout);
            }
        }

        std::string received = cmd_handler.read();

        if (received.length() > 0)
        {
            printf("Astraeus handler: received request for data: CHANGE ME");
        }

        sleep(1);
    }

    return 0;
}