/**
 * Listens for commands from GC. Ensure messages are received as expected using checksum, 
 * then send an OK back. Send data back, and listen for OK from GC.
 * 
*/
#include <stdio.h>      // Used for printing for console
#include <string.h>     // Used to manage strings
#include <fcntl.h>      // Used for file control
#include <sys/stat.h>   // Used for printing file info
#include <sys/types.h>  // Used for extra type capabilities
#include <unistd.h>     // Used for extra type capabilities
#include <iostream>

//TODO: get this incorporated w/ the radios.

int main(int argc, char const *argv[])
{    
    int radio_to_main_pipe;
 
    // FIFO file path
    const char * pipe_path = "/tmp/radio_to_main_pipe";
 
    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(pipe_path, 0666);
 
    char arr1[80], arr2[80];
    while (1)
    {
        // Open FIFO for write only
        radio_to_main_pipe = open(pipe_path, O_WRONLY);
 
        // Take an input arr2ing from user.
        // 80 is maximum length
        fgets(arr2, 80, stdin);
 
        // Write the input arr2ing on FIFO
        // and close it
        write(radio_to_main_pipe, arr2, strlen(arr2)+1);
        close(radio_to_main_pipe);
 
        // Open FIFO for Read only
        radio_to_main_pipe = open(pipe_path, O_RDONLY);
 
        // Read from FIFO
        read(radio_to_main_pipe, arr1, sizeof(arr1));
 
        // Print the read message
        printf("From Command Handler: %s\n", arr1);
        close(radio_to_main_pipe);
    }
    return 0;
}
