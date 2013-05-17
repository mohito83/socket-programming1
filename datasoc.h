#ifndef DATAASOC_H
#define DATASOC_H
#include "inventory.h"
#include "nethelper.h"

void receive_data(char* port,INVENTORY *inv);
void send_data(char* s_port, char* d_port, INVENTORY *inv);


#endif
