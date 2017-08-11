#include <stdio.h>
#include <pcap.h>

#define TRUE 1
#define FALSE 0

#ifndef BUFSIZ
#define BUFSIZ 1000
#endif

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
	/* Filtering the packets */
	// Command used to get netmask
	if(pcap_lookupnet(dev, &net, &mask, errbuf) == 1) {	
		fprintf(stderr, "Can't get netmask for device %s\n", dev);
		net = 0;
		mask = 0;
		printf("%u %u\n", net, mask);
	}
	// Skipping the filter bit
	// Compile given filter expressions 
	// and store in a struct
	result = pcap_compile(handle,		// Connection
						  &fp,			// Pointer to compiled data
						  filter_exp,	// Expression for filtering
						  FALSE,		// Whether to optimise
						  net);			//Net mask of network
	if(result == -1) {
		// Unable to create filters
		fprintf(stderr, "Couldn't parse filter %s: %s\n", dev, errbuf);
		return(2);
	}
	// Apply the compiled filter
	result = pcap_setfilter(handle,	// Connection
							&fp);	// Pointer to compiled data
	if(result == -1) {
		// Unable to set filter
		fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, 
															pcap_geterr(handle));
		return(2);
	}
	// Ending the filter bit */
	/* Start sniffing for packets */
	packet = pcap_next(handle, &header);	// grab a packet
	printf("Sniffed a packet with a length of [%d]\n", header.len);
	pcap_close(handle);	// Close the session
	return(0);
}
