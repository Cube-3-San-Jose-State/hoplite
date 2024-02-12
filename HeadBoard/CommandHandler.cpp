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

struct Pipe {
    int status;
    const char *path;
    char incoming[128], outgoing[128];
    Pipe(const char *p) : path(p){
        mkfifo(path, 0666);
    }
};

int main(int argc, char const *argv[])
{
    Pipe radio_to_main("/tmp/radio_to_main_pipe");

    while (1)
    {
        // First open in read only and read
        radio_to_main.status = open(radio_to_main.path, O_RDONLY);
        read(radio_to_main.status, radio_to_main.incoming, 128);
 
        // Print the read string and close
        printf("From Radio: %s\n", radio_to_main.incoming);
        close(radio_to_main.status);
 
        // Now open in write mode and write
        // string taken from user.
        radio_to_main.status = open(radio_to_main.path, O_WRONLY);
        fgets(radio_to_main.outgoing, 80, stdin);
        write(radio_to_main.status, radio_to_main.outgoing, strlen(radio_to_main.outgoing)+1);
        close(radio_to_main.status);
    }
    return 0;
}

