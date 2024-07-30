#include <stdio.h>     // Used for printing for console
#include <fcntl.h>     // fdUsed for file control
#include <sys/stat.h>  // Used for printing file info
#include <sys/types.h> // Used for extra type capabilities
#include <unistd.h>    // Used for extra type capabilities

struct Pipe
{
    int status;
    const char *path;
    char incoming[128], outgoing[128];
    Pipe(const char *p) : path(p)
    {
        mkfifo(path, 0666);
    }
};