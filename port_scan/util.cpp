#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "util.h"

sigfunc* signal(int signo, sigfunc* func)
{
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    } else {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }

    if (sigaction(signo, &act, &oact) < 0) {
        return SIG_ERR;
    }

    return oact.sa_handler;
}

netstat *get_stat(char *iface)
{
	int fd;
    struct ifreq ifr;
	struct netstat *status = new struct netstat;
     
    fd = socket(AF_INET, SOCK_DGRAM, 0);
 
    // Type of address to retrieve - IPv4 IP address
    ifr.ifr_addr.sa_family = AF_INET;
 
    // Copy the interface name in the ifreq structure
    strncpy(ifr.ifr_name , iface , IFNAMSIZ-1);
     
    // get the ip address
    ioctl(fd, SIOCGIFADDR, &ifr);
    // strcpy(status->ip, inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr));
    status->ip = htonl(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr.s_addr);
     
    // get the netmask ip
    ioctl(fd, SIOCGIFNETMASK, &ifr);
    // strcpy(status->netmask, inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr));
	status->netmask = htonl(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr.s_addr);
     
    close(fd);
	
	return status;
}
