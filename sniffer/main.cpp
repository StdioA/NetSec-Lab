#include <cstdio>
#include <pcap.h>
#include <cstring>
#include "util.h"

void packet_callback(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
	const ethernet_head *etherh;
	unsigned short eth_type;

	etherh = (ethernet_head *)packet;

	eth_type = ntohs(etherh->ether_type);
	printf("Ether type: %#06x\n", eth_type);

	/* 解析 ARP 报文 */
	if (eth_type == 0x0806)
	{
		const arp_head *arph = (arp_head *)(packet + sizeof(ethernet_head));
		if (arph->option == 256)
			printf("    ARP Request: %s -> %s\n",
					inet_ntoa(arph->src_ip), inet_ntoa(arph->dest_ip));
		else if (arph->option == 512)
			printf("    ARP Response: %s's MAC address is %s\n",
					inet_ntoa(arph->src_ip), mac_to_str(arph->src_addr));
	}

	/* 解析 IPv4 报文 */
	else if (eth_type == 0x0800)
	{
		const ip_head *iph = (ip_head *)(packet + sizeof(ethernet_head));
		printf("IP packet:\n");
		printf("    From: %s\n", inet_ntoa(iph->ip_src));
		printf("    To: %s\n", inet_ntoa(iph->ip_dst));

		switch (iph->ip_p) {
			case IPPROTO_TCP:
				printf("    Protocol: TCP\n");
				break;
			case IPPROTO_UDP:
				printf("    Protocol: UDP\n");
				return;
			case IPPROTO_ICMP:
				printf("    Protocol: ICMP\n");
				return;
			case IPPROTO_IP:
				printf("    Protocol: IP\n");
				return;
			default:
				printf("    Protocol: unknown\n");
				return;
		}

		/* 解析 TCP 报文 */
		if (iph->ip_p == IPPROTO_TCP)
		{
			const tcp_head *tcph = (tcp_head *)(packet + sizeof(ethernet_head) + sizeof(ip_head));
			int size_payload, size_ip, size_tcp;
			unsigned char *payload;

			size_ip = IP_HL(iph)*4;
			size_tcp = TH_OFF(tcph)*4;

			printf("    Source port: %d\n", ntohs(tcph->th_sport));
			printf("    Dest port: %d\n", ntohs(tcph->th_dport));
	
			/* 计算 payload 大小 */
			size_payload = ntohs(iph->ip_len) - (size_ip + size_tcp);
			// payload = (u_char *)(packet + sizeof(ethernet_head) + size_ip + size_tcp);
			
			/* 输出 payload 大小 */
			if (size_payload > 0)
				printf("    Payload: %d bytes\n", size_payload);
		}

		/* 解析 UDP 报文 */
		else if (iph->ip_p == IPPROTO_UDP)
		{
			const udp_head *udph = (udp_head *)(packet + sizeof(ethernet_head) + sizeof(ip_head)); 

			printf("    Source port: %d\n", ntohs(udph->uh_sport));
			printf("    Dest port: %d\n", ntohs(udph->uh_dport));
		}
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	char *dev, errbuf[PCAP_ERRBUF_SIZE];
	pcap_t *handle;
	struct bpf_program fp;
	char filter_exp[] = "arp or ip";
	bpf_u_int32 mask, net;
	struct pcap_pkthdr header;
	const u_char *packet;
	int num_packets = 10;

	if (argc < 2)
	{
		printf("Usage:\n    %s <device name>\n", argv[0]);
		return 1;
	}
	dev = argv[1];
	
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

	printf("Sniffer is started & listening packets.\n");
	printf("Press Ctrl+C to quit.\n");
	pcap_loop(handle, -1, packet_callback, NULL);
	// pcap_loop(handle, num_packets, packet_callback, NULL);
	pcap_close(handle);

	return 0;
}
