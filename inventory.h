#ifndef INVENTORY_H
#define INVENTORY_H

typedef struct Inventory{
int store_no;
int cameras;
int laptops;
int printers;
}INVENTORY;


INVENTORY *readInventory(char * inputFile);
void releaseInventory(INVENTORY *inv);

#endif
