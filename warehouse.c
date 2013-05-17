/*
* This file defined functions pertaining to ware house
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "inventory.h"
#include "nethelper.h"
#include "datasoc.h"

#define SERVER_NAME "nunki.usc.edu"
#define PORT "21952" // the port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold



void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

//This function creates the TCP server. The core functionality is build re-using the code from Beej's guide
//http://beej.us/guide/bgnet/examples/server.c
int createTCPServer(INVENTORY *t_vector){
	int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information	
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;
	int dataLen;
	INVENTORY *inv;
	unsigned char buffer[10];
	
	int count = 0;
		
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	
	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}
	
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}

	//print statement
	char * address = gethostaddress(SERVER_NAME);
	printf("Phase 1: The central warehouse has TCP port number 2962 and IP address %s \n", address);
	
	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}

	freeaddrinfo(servinfo); // all done with this structure
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	//printf("server: waiting for connections...\n");
	while(1 && count < 4) { // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);
		//printf("server: got connection from %s\n", s);

		//if (!fork()) { // this is the child process
			//close(sockfd); // child doesn't need the listener
			dataLen = recv(new_fd, buffer, 10, 0);
			//printf("dataLen: %d\n",dataLen);
			if (dataLen == -1){
					perror("recv");
			}else{
				inv = (INVENTORY *) malloc(sizeof(INVENTORY));
				//printf("BEFORE camera:%d, laptops:%d, printers:%d\n",t_vector->cameras,t_vector->laptops,t_vector->printers);
				unpack(buffer,inv);
				printf("Phase 1: The central warehouse received information from store# %d\n", inv->store_no);
								
				//update truck vector
				t_vector->cameras+=inv->cameras;
				t_vector->laptops+=inv->laptops;
				t_vector->printers+=inv->printers;
				//printf("AFTER camera:%d, laptops:%d, printers:%d\n",t_vector->cameras,t_vector->laptops,t_vector->printers);
				memset(buffer,0,10*sizeof(unsigned char));
				free(inv);
				count++;
			}

			//close(new_fd);
			//exit(0);
		//}
		close(new_fd); // parent doesn't need this
	}
	//close the TCP connection.
	close(sockfd);
	printf("End of Phase 1 for the central warehouse\n");
}

//This function prepare the truck vector to be sent to the store in PhaseII.
void prepare_t_vector(INVENTORY *inv){
	if(inv->cameras <= 0){
		inv->cameras = (-1) * inv->cameras;
	} else {
		inv->cameras = 0;
	}
	
	if(inv->printers <= 0){
		inv->printers = (-1) * inv->printers;
	} else {
		inv->printers = 0;
	}
	
	if(inv->laptops <= 0){
		inv->laptops = (-1) * inv->laptops;
	} else {
		inv->laptops = 0;
	}
}

int main(){
	//initialize truck vector
	INVENTORY *t_vector;
	t_vector = (INVENTORY *) malloc(sizeof(INVENTORY));
	memset(t_vector,0,sizeof(INVENTORY));
	
	createTCPServer(t_vector);
	
	//prepare truck vector for PhaseII
	prepare_t_vector(t_vector);
	
	//PHASE-2 Part 1
	//prepare UDP connection to store_1 and send truck vector to it.
	char *w_port = "31952";
	char *s1_port = "5952";
	
	char *address = gethostaddress(SERVER_NAME);
	
	printf("Phase 2: The central warehouse has UDP port number %s and IP address %s\n",w_port, address);
	send_data(w_port, s1_port, t_vector); //send data to store 1
	printf("Phase 2: Sending the truck-vector to outlet store store_1. The truck vector value is <%d,%d,%d>.\n",t_vector->cameras, t_vector->laptops, t_vector->printers);
	
	//wait for truck vector to return from store 4 PHASE-2 Part 3
	w_port = "32952";
	printf("Phase 2: The central warehouse has UDP port number %s and IP address %s\n",w_port, address);
	
	//setup UDP socket to receive datagram from store 4
	receive_data(w_port,t_vector);
	
	printf("Phase 2: The final truck-vector is received from the outlet store store_4, the truck-vector value is: <%d,%d,%d>\n", t_vector->cameras, t_vector->laptops, t_vector->printers);

	//End of PHASE-2
	printf("End of Phase 2 for the central warehouse\n");
	
	return 0;
} // end of main()
