#include <iostream>
#include <cstdio>
#include <cstring>
#include "port_scan.h"
#include "util.h"

using namespace std;


int main(int argc, char *argv[])
{
	if (argc < 2) {
        printf("Usage: %s <host> <start_port> <end_port>\n", argv[0]);
        printf("%s -d <device name> <start_port> <end_port>\n", argv[0]);
        printf("For example:\n%s 127.0.0.1 75 85\n", argv[0]);
        printf("%s -d eth0 75 85\n", argv[0]);
        return 1;
    }

	if (strcmp(argv[1], "-d") == 0)
	{
		struct netstat *status;
		char *iface = argv[2];
		int start_port = atoi(argv[3]);
		int end_port = atoi(argv[4]);
		char *host;
		unsigned long maskip;
		int negmask;
		struct in_addr in;

		status = get_stat(iface);
		in.s_addr = ntohl(status->ip);
		printf("IP: %s\n", inet_ntoa(in));
		in.s_addr = ntohl(status->netmask);
		printf("Netmask: %s\n", inet_ntoa(in));
		
		maskip = status->ip & status->netmask;
		negmask = (int)~(status->netmask);
		for (int i = 1; i<negmask; i++)
		{
			in.s_addr = ntohl(maskip+i);
			host = inet_ntoa(in);
			// printf("%s\n", host);
			for (int p = start_port; p <= end_port; p++)
			{
				if (scan_port(host, p) == 0)
					printf("%s:%d is open\n", host, p);
				// else
				//	printf("%s:%d timeout\n", host, p);
			}
		}
	}
	else
	{
		char *host = argv[1];
		char buf[32];
		int start_port = atoi(argv[2]);
		int end_port = atoi(argv[3]);
		// host处理
		if (inet_pton(AF_INET, host, buf) != 1)
		{
			fprintf(stderr, "Host address error %s\n", host);
			return 1;
		}

		for (int p = start_port; p <= end_port; p++)
		{
			if (scan_port(host, p) == 0)
				printf("%s:%d is open\n", host, p);
			else
				printf("%s:%d timeout\n", host, p);
		}
	}

	return 0;
}
