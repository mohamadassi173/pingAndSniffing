#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>


#define PACKETSIZE	64



void cal_ping_t(struct sockaddr_in *addr, int pid);
unsigned short check_sum(void *b, int len);
struct protoent *proto=NULL;



struct packet { 
struct icmphdr hdr;
	char msg[PACKETSIZE-sizeof(struct icmphdr)];
};

int main()
{	
	int pid = getpid();
	struct hostent *hname;
	struct sockaddr_in addr;
	proto = getprotobyname("ICMP");
	hname = gethostbyname("www.google.com");
	bzero(&addr, sizeof(addr));
	addr.sin_family = hname->h_addrtype;
	addr.sin_port = 0;
	addr.sin_addr.s_addr = *(long*)hname->h_addr;
	cal_ping_t(&addr, pid);
	return 0;
}

void cal_ping_t(struct sockaddr_in *addr, int pid)
{	const int val=255;
	int cnt=1;
	struct packet pckt;
	struct sockaddr_in r_addr;
   	float elapsed_mill;
	float elapsed_micro;
  	struct timeval t0;
   	struct timeval t1;
	int sock = socket(PF_INET, SOCK_RAW, proto->p_proto);
	setsockopt(sock, SOL_IP, IP_TTL, &val, sizeof(val));
	fcntl(sock, F_SETFL, O_NONBLOCK);
		bzero(&pckt, sizeof(pckt));
		int len=sizeof(r_addr);
		pckt.hdr.type = ICMP_ECHO;
		pckt.hdr.un.echo.id = pid;
		int i;
		for (i = 0; i < sizeof(pckt.msg)-1; i++ ){
			pckt.msg[i] = i+'0';
        }
		pckt.msg[i] = 0;
		pckt.hdr.un.echo.sequence = cnt++;
		pckt.hdr.checksum = check_sum(&pckt, sizeof(pckt));



		gettimeofday(&t0, NULL);
		sendto(sock, &pckt, sizeof(pckt), 0, (struct sockaddr*)addr, sizeof(*addr));
		while (1) {
		    if ( recvfrom(sock, &pckt, sizeof(pckt), 0, (struct sockaddr*)&r_addr, &len) > 0 )
			break;		
		}

		gettimeofday(&t1, NULL);



		elapsed_mill = (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
		elapsed_micro =elapsed_mill *1000;
		printf("\n");
		printf("////////////////////////////////////////////////////////////////\n");
		printf("\n");
   		printf("	in milliseconds: %f\n", elapsed_mill);
   		printf("	in microseconds:  %f\n", elapsed_micro);
		printf("\n");
		printf("////////////////////////////////////////////////////////////////\n");

}

unsigned short check_sum(void *b, int len){
	unsigned short *buf = b;
	unsigned int sum=0;
	unsigned short result;

	for ( sum = 0; len > 1; len -= 2 ){
		sum += *buf++;
}
	if ( len == 1 )
		sum += *(unsigned char*)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}
