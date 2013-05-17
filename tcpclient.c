/*
*This file defines the functions for TCP client
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "inventory.h"
#include "nethelper.h"

#define SERVER_PORT "21952" // server port number
#define SERVER_NAME "nunki.usc.edu" //server host name

int sockfd; //socket file descriptor

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

//This function establishes the TCP connection to the server. The core funtionality is build re-using code from Beej's guide
//http://beej.us/guide/bgnet/examples/client.c
void establishConnection(int store_no)
{
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	
	if ((rv = getaddrinfo(SERVER_NAME, SERVER_PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return;
	}
	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}
		break;
	}
	
	//get host IP by using its name and its port
	struct sockaddr_in ss;
	int len=sizeof(ss);
	getsockname(sockfd,(struct sockaddr*)&ss,&len);
	//get host IP by using its name
    char* address = gethostaddress(SERVER_NAME);
	printf("Phase 1: store_%d has TCP port number %d and IP address %s\n",store_no,ss.sin_port,address);

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return;
	}
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s, sizeof s);
	//printf("client: connecting to %s\n", s);
	freeaddrinfo(servinfo); // all done with this structure
}

//this method  releases the TCP connection
void releaseConnection(){
	//printf("Closing the client TCP socket\n");
	close(sockfd);
}

//This method sends the data which is an Inventory type structure to the server.
void sendData(INVENTORY* data){		
	unsigned char buffer[10];
	int bytes_Sent, len;
	len = sizeof(*data);
	//serialize the Inventory structure to character array.
	pack(data,buffer);
	bytes_Sent = send(sockfd, buffer, 10, 0); //send data to server
	
	//print statement
	printf("Phase 1: The outlet vector<%d,%d,%d> for store_%d has been sent to the central warehouse\n",data->cameras, data->laptops, data->printers, data->store_no);

}
