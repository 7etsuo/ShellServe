#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int
main(int argc, char **argv)
{
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    if (argc != 2) { 
        fprintf(stderr, "Usage: %s hostname\n", argv[0]);
        exit(EXIT_FAILURE);
    } 

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // IP6 or IP4
    hints.ai_socktype = SOCK_STREAM; // TCP


    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo failed: %s", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    for (p = res; p; p=p->ai_next)   { 
        void *addr; 
        char *ipver;

        if (p->ai_family == AF_INET) { 
            struct sockaddr_in *sa4 = (struct sockaddr_in*) p->ai_addr;
            addr = &(sa4->sin_addr);
            
        } else {
            struct sockaddr_in6 *sa6 = (struct sockaddr_in6*) p->ai_addr;
            addr = &(sa6->sin6_addr);
        }

        const char *ip = inet_ntop(p->ai_family, addr,  ipstr, sizeof ipstr);
        if (!ip) {
            perror("inet_ntop");
            exit(EXIT_FAILURE);
        }

        puts(ip);
    }
    
    freeaddrinfo(res);

    return 0;
}
