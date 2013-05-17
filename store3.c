/*
*This file define functions pertaining to store 3
*/
#include<stdio.h>
#include<stdlib.h>
#include "inventory.h"
#include "tcpclient.h"
#include "nethelper.h"
#include "datasoc.h" //server name

#define SERVER_NAME "nunki.usc.edu"

int main(){

	//Step1: read the store inventory file.
	char *fileName = "Store-3.txt";

	INVENTORY *t_vector = (INVENTORY*)malloc(sizeof(INVENTORY));;
	INVENTORY * outlet_vector = readInventory(fileName);
	outlet_vector->store_no = 3;

	//Step2: estabilish TCP connection with the server
	establishConnection(3);
	
	//Step3: send data over TCP connnection
	sendData(outlet_vector);
	
	//Step4: close the TCP connection
	releaseConnection();
	
	printf("End of Phase 1 for store_3\n");
	
	
	//starting PHASE-2
	char *s_port="13952";
	char *d_port="17952";
	char *address=gethostaddress(SERVER_NAME);
	printf("Phase 2: Store_3 has UDP port %s and IP address %s\n",s_port, address);
	
	//receive truck vector from store 2
	receive_data(s_port,t_vector);
	printf("Phase 2: Store_3 received the truck-vector<%d,%d,%d> from store_2.\n", t_vector->cameras, t_vector->laptops, t_vector->printers);
	
	//Upon start of Phase2 part2 round1
	s_port = "14952";
	printf("Phase 2: Store_3 has UDP port %s and IP address %s\n",s_port, address);
	
	//updating the truck and outlet vectors
	update_vectors(t_vector, outlet_vector);
	
	//send the truck vector to store 4
	send_data(s_port,d_port,t_vector);
	printf("Phase 2: The updated truck-vector<%d,%d,%d> has been sent to store_4\n",t_vector->cameras, t_vector->laptops, t_vector->printers);
	printf("Phase 2: Store_3 updated outlet-vector is <%d,%d,%d>\n",outlet_vector->cameras, outlet_vector->laptops, outlet_vector->printers);
	
	//Upon start of Phase 2 part 2 round2
	s_port="15952";
	printf("Phase 2: Store_3 has UDP port %s and IP address %s\n", s_port, address);

	//waiting to receive data gram from store 2
	receive_data(s_port,t_vector);
	printf("Phase 2: truck-vector<%d,%d,%d> has been received from Store_2\n",t_vector->cameras, t_vector->laptops, t_vector->printers);
	
	//Phase 2 part2 round 2
	//updating the vectors
	update_vectors(t_vector, outlet_vector);
	
	//Sending truck vector to store 4
	s_port="16952";
	d_port="19952";
	printf("Phase 2: Store_3 has UDP port %s and IP address %s\n",s_port, address);
	send_data(s_port,d_port,t_vector);
	printf("Phase 2: The updated truck-vector<%d,%d,%d> has been sent to store_4\n",t_vector->cameras, t_vector->laptops, t_vector->printers);
	printf("Phase 2: Store_3 updated outlet vector is <%d,%d,%d>\n",outlet_vector->cameras, outlet_vector->laptops, outlet_vector->printers);
	
	//Ends of PHASE -2
	printf("End of Phase 2 for store_3\n");
	
	releaseInventory(outlet_vector);
	releaseInventory(t_vector);
	
	return 1;
}

