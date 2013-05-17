#ifndef NETHELPER_H
#define NETHELPER_H
#include "inventory.h"

void pack(INVENTORY *inv, unsigned char *buffer);
void unpack(unsigned char* buffer, INVENTORY *inv);

char* gethostaddress(char *hostname);
void update_vectors(INVENTORY *t_vector, INVENTORY *outlet_vector);

#endif
