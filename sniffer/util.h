#ifndef _UTIL_H
#define _UTIL_H
/* Ethernet addresses are 6 bytes */
#define ETHER_ADDR_LEN	6

#include <arpa/inet.h>
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
	struct in_addr src_ip;						//源IP地址：发送方的IP地址 
	unsigned char dest_addr[6];					//目的MAC地址：ARP请求中该字段没有意义；ARP响应中为接收方的MAC地址 
	struct in_addr dest_ip;						//目的IP地址：ARP请求中为请求解析的IP地址；ARP响应中为接收方的IP地址
	unsigned char padding[18]; 
} arp_head;

/* IP 报头 */
typedef struct  {
        u_char  ip_vhl;                 /* version << 4 | header length >> 2 */
        u_char  ip_tos;                 /* type of service */
        u_short ip_len;                 /* total length */
        u_short ip_id;                  /* identification */
        u_short ip_off;                 /* fragment offset field */
        #define IP_RF 0x8000            /* reserved fragment flag */
        #define IP_DF 0x4000            /* dont fragment flag */
        #define IP_MF 0x2000            /* more fragments flag */
        #define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */
        u_char  ip_ttl;                 /* time to live */
        u_char  ip_p;                   /* protocol */
        u_short ip_sum;                 /* checksum */
        struct  in_addr ip_src,ip_dst;  /* source and dest address */
} ip_head;
#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)                (((ip)->ip_vhl) >> 4)


/* TCP 报头 */
typedef u_int tcp_seq;
typedef struct {
        u_short th_sport;               /* source port */
        u_short th_dport;               /* destination port */
        tcp_seq th_seq;                 /* sequence number */
        tcp_seq th_ack;                 /* acknowledgement number */
        u_char  th_offx2;               /* data offset, rsvd */
#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)
        u_char  th_flags;
        #define TH_FIN  0x01
        #define TH_SYN  0x02
        #define TH_RST  0x04
        #define TH_PUSH 0x08
        #define TH_ACK  0x10
        #define TH_URG  0x20
        #define TH_ECE  0x40
        #define TH_CWR  0x80
        #define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
        u_short th_win;                 /* window */
        u_short th_sum;                 /* checksum */
        u_short th_urp;                 /* urgent pointer */
} tcp_head;

/* UDP 报头 */
typedef struct
{
    u_short uh_sport;               /* source port */
    u_short uh_dport;               /* destination port */
    u_short uh_length;				/* packet length */
    u_short uh_sum;                 /* checksum */
} udp_head;
char * mac_to_str(const unsigned char *mac);
#endif
