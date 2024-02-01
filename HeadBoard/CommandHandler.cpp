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

// TODO: make a class for pipes

int main(int argc, char const *argv[])
{
    int radio_to_main_pipe;
    // FIFO file path
    const char * pipe_path = "/tmp/radio_to_main_pipe";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(pipe_path, 0666);
     
    char str1[80], str2[80];
    while (1)
    {
        // First open in read only and read
        radio_to_main_pipe = open(pipe_path,O_RDONLY);
        read(radio_to_main_pipe, str1, 80);
 
        // Print the read string and close
        printf("From Radio: %s\n", str1);
        close(radio_to_main_pipe);
 
        // Now open in write mode and write
        // string taken from user.
        radio_to_main_pipe = open(pipe_path,O_WRONLY);
        fgets(str2, 80, stdin);
        write(radio_to_main_pipe, str2, strlen(str2)+1);
        close(radio_to_main_pipe);
    }
    return 0;
}

