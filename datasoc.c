/*
*This file defines the fucntions related to UDP socket transation
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "inventory.h"
#include "nethelper.h"
#include "datasoc.h"


#define MAXBUFLEN 10
#define SERVER_NAME "nunki.usc.edu"

int sockfd;
struct addrinfo hints, *servinfo, *destinfo, *p;
int rv;
int numbytes;
struct sockaddr_storage their_addr;
unsigned char buf[MAXBUFLEN];
socklen_t addr_len;
char s[INET6_ADDRSTRLEN];

//This function receives the datagram. This core fucntonality is build re-using code from Beej's guide
//http://beej.us/guide/bgnet/examples/listener.c
void receive_data(char* port,INVENTORY *inv){
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) { //bind the socket to a port
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return;
	}

	freeaddrinfo(servinfo); //free servInfo address

	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) { //blocking to receive the data
		perror("recvfrom");
		exit(1);
	}
	
	//de-serialize the data 
	unpack(buf,inv);

	//clsoe the socket once the data is received
	close(sockfd);

	return;
}

//This function sends the datagram. This core fucntonality is build re-using code from Beej's guide
//http://beej.us/guide/bgnet/examples/talker.c
void send_data(char* s_port, char* d_port, INVENTORY *inv){
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(SERVER_NAME, s_port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}
		//bind the socket with the static port
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker: failed to bind socket\n");
		return;
	}
	
	//populate destination address
	if ((rv = getaddrinfo(SERVER_NAME, d_port, &hints, &destinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return;
	}
	
	//serialize the data
	pack(inv,buf);
	
	if ((numbytes = sendto(sockfd, buf, MAXBUFLEN, 0, destinfo->ai_addr, destinfo->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}

	freeaddrinfo(servinfo); //free serveinfo
	freeaddrinfo(destinfo); //free destination info

	close(sockfd); //close the socket after sending the data.

	return;
}
