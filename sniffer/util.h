#ifndef _UTIL_H
#define _UTIL_H
/* Ethernet addresses are 6 bytes */
#define ETHER_ADDR_LEN	6

#pragma pack (1)

/* 以太网报头 */
typedef struct
{
	u_char dest_mac[ETHER_ADDR_LEN];			// 目标 MAC 地址
	u_char source_mac[ETHER_ADDR_LEN];			// 源 MAC 地址
	u_short ether_type;							// 以太网类型
} ethernet_head;

/* ARP 报头 */
typedef struct
{ 
	unsigned short hardware_type;				//硬件类型：以太网接口类型为1 
	unsigned short protocol_type;				//协议类型：IP协议类型为0X0800 
	unsigned char add_len;						//硬件地址长度：MAC地址长度为6B 
	unsigned char pro_len;						//协议地址长度：IP地址长度为4B 
	unsigned short option;						//操作：ARP请求为1，ARP应答为2 
	unsigned char src_addr[6];					//源MAC地址：发送方的MAC地址 
	unsigned char src_ip[4];					//源IP地址：发送方的IP地址 
	unsigned char dest_addr[6];					//目的MAC地址：ARP请求中该字段没有意义；ARP响应中为接收方的MAC地址 
	unsigned char dest_ip[4];					//目的IP地址：ARP请求中为请求解析的IP地址；ARP响应中为接收方的IP地址
	unsigned char padding[18]; 
} arp_head; 

/* 最终arp包结构 */
typedef struct
{
	ethernet_head eth;					//以太网头部 
	arp_head arp;						//arp数据包头部 
} arp_packet;

char * mac_to_str(const unsigned char *mac);
char * ip_to_str(const unsigned char *ip);
#endif
