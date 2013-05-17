/*
*This file define functions pertaining to store 2
*/
#include<stdio.h>
#include<stdlib.h>
#include "inventory.h"
#include "tcpclient.h"
#include "nethelper.h"
#include "datasoc.h"

#define SERVER_NAME "nunki.usc.edu" //server name

int main(){

	//Step1: read the store inventory file.
	char *fileName = "Store-2.txt";

	INVENTORY *t_vector= (INVENTORY*)malloc(sizeof(INVENTORY));;
	INVENTORY * outlet_vector = readInventory(fileName);
	outlet_vector->store_no = 2;

	//Step2: estabilish TCP connection with the server
	establishConnection(2);
	
	//Step3: send data over TCP connnection
	sendData(outlet_vector);
	
	//Step4: close the TCP connection
	releaseConnection();
	
	printf("End of Phase 1 for store_2\n");
	
	
	//starting PHASE-2
	char *s_port="9952";
	char *d_port="13952";
	char *address=gethostaddress(SERVER_NAME);
	printf("Phase 2: Store_2 has UDP port %s and IP address %s\n",s_port, address);
	
	//receive truck vector from store 1
	receive_data(s_port,t_vector);
	printf("Phase 2: Store_2 received the truck-vector<%d,%d,%d> from store_1.\n", t_vector->cameras, t_vector->laptops, t_vector->printers);
	
	//Upon start of Phase2 part2 round1
	s_port = "10952";
	printf("Phase 2: Store_2 has UDP port %s and IP address %s\n",s_port, address);
	
	//updating the truck and outlet vectors
	update_vectors(t_vector, outlet_vector);
	
	//send the truck vector to store 3
	send_data(s_port,d_port,t_vector);
	printf("Phase 2: The updated truck-vector<%d,%d,%d> has been sent to store_3\n",t_vector->cameras, t_vector->laptops, t_vector->printers);
	printf("Phase 2: Store_2 updated outlet-vector is <%d,%d,%d>\n",outlet_vector->cameras, outlet_vector->laptops, outlet_vector->printers);
	
	//Upon start of Phase 2 part 2 round2
	s_port="11952";
	printf("Phase 2: Store_2 has UDP port %s and IP address %s\n", s_port, address);

	//waiting to receive data gram from store 1
	receive_data(s_port,t_vector);
	printf("Phase 2: truck-vector<%d,%d,%d> has been received from Store_1\n",t_vector->cameras, t_vector->laptops, t_vector->printers);
	
	//Phase 2 part2 round 2
	//updating the vectors
	update_vectors(t_vector, outlet_vector);
	
	//Sending truck vector to store 3
	s_port="12952";
	d_port="15952";
	printf("Phase 2: Store_2 has UDP port %s and IP address %s\n",s_port, address);
	send_data(s_port,d_port,t_vector);
	printf("Phase 2: The updated truck-vector<%d,%d,%d> has been sent to store_3\n",t_vector->cameras, t_vector->laptops, t_vector->printers);
	printf("Phase 2: Store_2 updated outlet vector is <%d,%d,%d>\n",outlet_vector->cameras, outlet_vector->laptops, outlet_vector->printers);
	
	//Ends of PHASE -2
	printf("End of Phase 2 for store_2\n");
	
	releaseInventory(outlet_vector);
	releaseInventory(t_vector);

	return 1;
}

