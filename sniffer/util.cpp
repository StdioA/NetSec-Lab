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
