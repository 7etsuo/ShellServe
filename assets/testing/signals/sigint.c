#include <stdio.h> 
#include <signal.h>
#include <unistd.h>

void
handler(int signal)
{
    printf("Caught SIGINT\n");
    _exit(0);
} 

int
main(int argc, char *argv[])
{
    void (*sighandler_t) (int) = handler;
    if(signal(SIGINT, handler) == SIG_ERR)
        fprintf(stderr, "signal error");
    
    pause();

    return 0;
}
