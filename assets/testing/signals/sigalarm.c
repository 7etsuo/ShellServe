#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void
handler(int sig)
{
    static int beeps = 0;
    printf("BEEP\n");
    if(++beeps < 5)
       alarm(1);
    else { 
        printf("BOOM!\n");        
        _exit(0);
    } 
} 


int
main(int argc, char *argv[])
{
    void(*signalhandler_t)(int) = handler;    

    signal(SIGALRM, handler);
    alarm(1);
    while (1) {
        ;
    }

    _exit(0); 
}

