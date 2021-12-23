#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <pcap.h>

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
  pcap_t *handle;
  char errbuf[PCAP_ERRBUF_SIZE];
  struct bpf_program f_prog;
  bpf_u_int32 net;



struct icmp_header {
  unsigned char icmp_type;
  unsigned char icmp_code;
};

struct ethernet_header {
  u_char  ether_dhost[6];
  u_char  ether_shost[6];
  u_short ether_type;  
};

struct ip_header {
  unsigned char      iph_ihl:4;
  struct  in_addr    iph_sourceip;
  struct  in_addr    iph_destip;
};


int main(){ 
  printf("\nWAITING...\n"); 
  handle = pcap_open_live("enp0s3", BUFSIZ, 1, 1000, errbuf); 
  pcap_compile(handle, &f_prog, "icmp", 0, net);      
  pcap_setfilter(handle, &f_prog);                             
  pcap_loop(handle, -1, got_packet, NULL);                
  pcap_close(handle);
  return 0;
}


void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
  struct ethernet_header *ethernet = (struct ethernet_header *) packet;
  if (ntohs(ethernet->ether_type) == 0x0800) {
    struct ip_header * ip = (struct ip_header *) (packet + sizeof(struct ethernet_header)); 
        int ip_header_len=ip->iph_ihl*4;
        struct icmp_header *icmp=(struct icmp_header *)(packet+sizeof(struct ethernet_header)+ip_header_len);
    	printf("       Ip source: %s\n", inet_ntoa(ip -> iph_sourceip));  
    	printf("       Ip dest: %s\n", inet_ntoa(ip -> iph_destip));   
	printf("       Type: %u\n", icmp -> icmp_code);
        printf("       Code: %u\n", icmp -> icmp_type);
	printf("--------------------------------------------------------------------------\n");
  }
  }
 
