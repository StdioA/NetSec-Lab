#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "util.h"

#define MAC_ADDR_LEN 17

char *mac_to_str(const unsigned char *mac)
{
	int i;
	char *mac_str;

	mac_str = (char *)malloc(MAC_ADDR_LEN);
	sprintf(mac_str, "%02X-%02X-%02X-%02X-%02X-%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	return mac_str;
}

char *ip_to_str(const unsigned char *ip)
{
	char *ip_str;
	// unsigned char *ip_p = (unsigned char *)&ip;

	ip_str = (char *)malloc(20);
	sprintf(ip_str, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

	return ip_str;
}
