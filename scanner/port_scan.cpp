#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <signal.h>
#include <arpa/inet.h>
#include "port_scan.h"

void connect_alarm(int sig)
{
	return;
}

int connect_timeout(int sockfd, const sockaddr* paddr, socklen_t salen, int nsec)
{
	int retval = 0;

	signal(SIGALRM, connect_alarm);
	if (alarm(nsec) != 0)
	{
		perror("Alarm was already set");   
	}

	if ((retval = connect(sockfd, paddr, salen)) < 0)
	{
		close(sockfd);
		if (errno == EINTR) {
			errno = ETIMEDOUT;
		}
	}
	alarm(0);
	signal(SIGALRM, connect_alarm);

	return retval;
}

int scan_port(char *host, int port)
{
	int status;
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));

	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0) {
		perror("Create socket error");
		return 1;
	}

	addr.sin_family = AF_INET;
	inet_pton(AF_INET, host, &addr.sin_addr);
	addr.sin_port = htons(port);

	status = connect_timeout(sfd, (struct sockaddr*)&addr, sizeof(addr), 1);

	close(sfd);

	return status;
}

