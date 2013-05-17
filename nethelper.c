/*
*This file contains the utility functions to be used for socket communication between server and client
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <netdb.h>
#include "nethelper.h"
#include "inventory.h"

//from beej's guide
//http://beej.us/guide/bgnet/examples/pack2.c
//This function serliaze an integer into the character array
void packi16(unsigned char *buf, unsigned int i)
{
	*buf++ = i>>8; *buf++ = i;
}

//from beej's guide
//http://beej.us/guide/bgnet/examples/pack2.c
//This function de-serializes the integer from the character array
int unpacki16(unsigned char *buf)
{
	unsigned int i2 = ((unsigned int)buf[0]<<8) | buf[1];
	int i;

	// change unsigned numbers to signed
	if (i2 <= 0x7fffu) { i = i2; }
	else { i = -1 - (unsigned int)(0xffffu - i2); }

	return i;
}

//This functiion serializes INVENTORY to character array. It takes each member of the INVENTORY structure, 
//serialize them individually and packs them to character array.
void pack(INVENTORY *t1, unsigned char* buffer){
	packi16(buffer,t1->cameras);
	buffer+=2;
	packi16(buffer,t1->laptops);
	buffer+=2;
	packi16(buffer,t1->printers);
	buffer+=2;
	packi16(buffer,t1->store_no);
	
}

//This function de-serializes the character array into the corresponding INVENTORY stucture. It does that by
// unpacking each memeber of inventory structure.
void unpack(unsigned char* buffer, INVENTORY *t1){
	t1->cameras=unpacki16(buffer);
	buffer+=2;
	t1->laptops=unpacki16(buffer);
	buffer+=2;
	t1->printers=unpacki16(buffer);
	buffer+=2;
	t1->store_no=unpacki16(buffer);
}

//This fucntion gets the host IP address from the hostname
char* gethostaddress(char *hostname){
	struct hostent *he = gethostbyname(hostname);
	char* hostaddress;
	struct in_addr a;
	while (*he->h_addr_list){ // gets the list of address and iterate through them.
        bcopy(*he->h_addr_list++, (char *) &a, sizeof(a));
        hostaddress = (char*)inet_ntoa(a);
    }
	return hostaddress;
}

//This function updates the truck vector and outlet vector for each store in Phase-2
void update_vectors(INVENTORY *t_vector, INVENTORY *o_vector){
	//updates the cameras for both truck and outlet vector.
	if(o_vector->cameras < 0){
		if(((-1)*o_vector->cameras) > t_vector->cameras){
			o_vector->cameras = o_vector->cameras + t_vector->cameras;
			t_vector->cameras = 0;
		} else {
			t_vector->cameras = t_vector->cameras + o_vector->cameras;
			o_vector->cameras = 0;
		}	
	} else if(o_vector->cameras > 0){
		t_vector->cameras = t_vector->cameras + o_vector->cameras;
		o_vector->cameras = 0;
	}
	
	//updates the printers for both truck and outlet vector.
	if(o_vector->printers < 0){
		if(((-1)*o_vector->printers) > t_vector->printers){
			o_vector->printers = o_vector->printers + t_vector->printers;
			t_vector->printers = 0;
		} else {
			t_vector->printers = t_vector->printers + o_vector->printers;
			o_vector->printers = 0;
		}	
	} else if(o_vector->printers > 0){
		t_vector->printers = t_vector->printers + o_vector->printers;
		o_vector->printers = 0;
	}
	
	//updates the laptops for both truck and outlet vector.
	if(o_vector->laptops < 0){
		if(((-1)*o_vector->laptops) > t_vector->laptops){
			o_vector->laptops = o_vector->laptops + t_vector->laptops;
			t_vector->laptops = 0;
		} else {
			t_vector->laptops = t_vector->laptops + o_vector->laptops;
			o_vector->laptops = 0;
		}	
	} else if(o_vector->laptops > 0){
		t_vector->laptops = t_vector->laptops + o_vector->laptops;
		o_vector->laptops = 0;
	}
}
