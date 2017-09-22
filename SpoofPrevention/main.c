#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header

#define TRUE 1
#define FALSE 0

#ifndef BUFSIZ
#define BUFSIZ 1000
#endif

/********************** Macro and Structs to typecast packet **********************/

//Ethernet addresses are 6 bytes
#define ETHER_ADDR_LEN 6

//Ethernet Header
struct sniff_ethernet {
	u_char ether_dhost[ETHER_ADDR_LEN];	// Destination Addr.
	u_char ether_shost[ETHER_ADDR_LEN];	// Source Addr.
	u_short ether_type;	// IP? ARP? RARP? etc 
};

// IP Header
struct sniff_ip {
	u_char ip_vhl;	// version << 4 | header length >>2 
	u_char ip_tos;	// type of service 
	u_short ip_len;	// Total length
	u_short ip_id;	// Identification
	u_short ip_off;	// Fragment offset field
#define IP_RF 0x8000	// Reserved Fragment Flad
#define IP_DF 0x4000	// Dont Fragment Flag
#define IP_MF 0x2000	// More Fragments Flag
#define IP_OFFMASK 0x1fff	// Mask for fragmenting bits
	u_char ip_ttl;	// Time to live
	u_char ip_p;	// Protocol
	u_short ip_sum;	// Checksum
	struct in_addr ip_src, ip_dst;	// Source and Dest Addr.
};
#define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)  (((ip)->ip_vhl) >> 4)

// TCP Header
struct sniff_tcp {
	u_short th_sport;	// Source Port
	u_short th_dport;	// Destination Port
	tcp_seq th_seq;	// Sequence Number
	tcp_seq th_ack;	// Acknowledgement Number
	u_char th_offx2;	// Data Offset, Reserved
#define TH_OFF(th) (((th)->th_offx2 & 0xf0) >> 4)
	u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_PUSH|TH_ACK|TH_URG|TH_ECE|TH_CWR)
	u_short th_win;	// Window
	u_short th_sum;	// Checksum
	u_short th_urp;	// Urgent Pointer
};

#define SIZE_ETHERNET 14
const struct sniff_ethernet *ethernet; /* The ethernet header */
const struct sniff_ip *ip; /* The IP header */
const struct sniff_tcp *tcp; /* The TCP header */
const char *payload; /* Packet payload */
const char *current_ip = "192.168.123.6";
const char *network_ip = "192.168";
const char *google_ip = "52.206.207.65";

u_int size_ip;
u_int size_tcp;
/**********************************************************************************/

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

int main(int argc, char *argv[])
{
	/* Setting up a device */
	char *dev, errbuf[PCAP_ERRBUF_SIZE];	// Name of interface and error buffer
	pcap_t *handle;	// Session Handler
	struct bpf_program fp;	// File where compiled filters are stored
	char *filter_exp = "port 23";	// The filter expression
	bpf_u_int32 mask;	// The netmask of sniffing device
	bpf_u_int32 net;	// The IP of sniffing device
	struct pcap_pkthdr header;	// The header that pcap gives us
	const u_char *packet;	// The actual packet
	int result;	// Result of various commands executed
	dev = pcap_lookupdev(errbuf);	// Get the interface to be sniffed upon
	if (dev == NULL) {
		fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
		return(2);
	}
	printf("Device: %s\n", dev);
	/* Opening the device */
	handle = pcap_open_live(dev,	// Name of device
							BUFSIZ,	// Buffer Size (Maximum to be captured)
							TRUE,	// Promiscuous? Yes - read all packets
									// Irrespective of the expected receiver
							1000,	// Timeout time
							errbuf);// Place to store error string
	/* if(pcap_datalink(handle) != DLT_EN10MB) {	// Check if link is ethernet
		// Connection is not an ethernet connection
		fprintf(stderr, 
			"Device %s doesn't provide Ethernet headers - not supported\n", dev);
		return(2);
	}*/
	printf("Opening done!\n");
	/* Start sniffing for packets */
	packet = pcap_next(handle, &header);	// grab a packet
	printf("Sniffed a packet with a length of [%d]\n", header.len);
	pcap_loop(handle, -1, got_packet, NULL);
	pcap_close(handle);	// Close the session
	return(0);
}

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
	char *ip_src_str, *ip_dst_str;
	ethernet = (struct sniff_ethernet*)(packet);
	ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
	size_ip = IP_HL(ip)*4;
	if (size_ip < 20) {
		printf("   * Invalid IP header length: %u bytes\n", size_ip);
		return;
	}
	tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
	size_tcp = TH_OFF(tcp)*4;
	if (size_tcp < 20) {
		// printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
		return;
	}
	payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);
	ip_src_str = inet_ntoa(ip->ip_src);
	ip_dst_str = inet_ntoa(ip->ip_dst);
	if(strcmp(ip_src_str, google_ip) == 0) {
		/*printf("TCP Packet of length [%d] captured.\n", header->len);
		printf("Source port is %u, Destination Port is %u\n", tcp->th_sport, tcp->th_dport);*/
		printf("Source Address is %s, Destination Address is %s\n", ip_src_str, ip_dst_str);
	}
}
