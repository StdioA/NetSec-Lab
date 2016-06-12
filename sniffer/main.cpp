#include <cstdio>
#include <pcap.h>
#include <cstring>
#include "util.h"

void packet_callback(u_char *args, const struct pcap_pkthdr *header, const u_char *data)
{
	const arp_packet *packet;
	const arp_head *arph;

	packet = (arp_packet *)data;

	arph = &(packet->arp);

	if (arph->option == 256)
		printf("ARP Request: %s -> %s\n", ip_to_str(arph->src_ip), ip_to_str(arph->dest_ip));
	else if (arph->option == 512)
		printf("ARP Response: %s's MAC address is %s\n", ip_to_str(arph->src_ip), mac_to_str(arph->src_addr));
}

int main(int argc, char *argv[])
{
	char dev[20], errbuf[PCAP_ERRBUF_SIZE];
	pcap_t *handle;
	struct bpf_program fp;
	char filter_exp[] = "arp";
	bpf_u_int32 mask, net;
	struct pcap_pkthdr header;
	const u_char *packet;
	int num_packets = 10;
	
	strcpy(dev, "enp0s8");

	if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1)
	{
		fprintf(stderr, "Can't get netmask for device %s\n", dev);
		net = 0;
		mask = 0;
	}

	handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
	if (handle == NULL)
	{
		fprintf(stderr, "Couldn't open the device. %s\n", errbuf);
		return 2;
	}

	if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1)
	{
		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return 2;
	}
	if (pcap_setfilter(handle, &fp) == -1)
	{
		fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return 2;
	}

	pcap_loop(handle, -1, packet_callback, NULL);
	// pcap_loop(handle, num_packets, packet_callback, NULL);
	pcap_close(handle);

	return 0;
}
