#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
 
int main()
{
    int fd;
    struct ifreq ifr;
     
    char iface[] = "enp0s8";
     
    fd = socket(AF_INET, SOCK_DGRAM, 0);
 
    //Type of address to retrieve - IPv4 IP address
    ifr.ifr_addr.sa_family = AF_INET;
 
    //Copy the interface name in the ifreq structure
    strncpy(ifr.ifr_name , iface , IFNAMSIZ-1);
     
    //get the ip address
    ioctl(fd, SIOCGIFADDR, &ifr);
     
    //display ip
    printf("IP address of %s - %d\n" , iface , inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr) );
     
    //get the netmask ip
    ioctl(fd, SIOCGIFNETMASK, &ifr);
     
    //display netmask
    printf("Netmask of %s - %d\n" , iface , inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr) );
     
    close(fd);
     
    return 0;
}
