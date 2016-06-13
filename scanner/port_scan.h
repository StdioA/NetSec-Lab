#ifndef PORT_SCAN_H
#define PORT_SCAN_H
#include <arpa/inet.h>

void connect_alarm(int sig);
int connect_timeout(int sockfd, const sockaddr* paddr, socklen_t salen, int nsec);
int scan_port(char *host, int port);
#endif
