#include<stdio.h>
#include<stdlib.h>
#include "inventory.h"

typedef enum {false, true} bool;

//This function releases the memory allocated to Inventory
void releaseInventory(INVENTORY *inv){
	free(inv);
}

//This function reads the data from the Store# file and populate the corresponding INVENTORY structure.
INVENTORY *readInventory(char * inputFile){

INVENTORY *inv;
FILE *fp;
bool startParsing = false, isNegative = false;
int a[3], i=0, j=0, k=0, c=0;

if((fp = fopen(inputFile, "r")) != NULL){	 //opens the file
	while((c = fgetc(fp)) != EOF) {	 //read the file character by character
		if(c == ' '){	 // a white space is the trigger to start the parsing the line
			startParsing = true;
			continue;
		}
		
		if(c == '\n'){	// newline characters is the trigger for stopping the parsing
			if(isNegative == true){
				a[i] = (-1) * j;
			} else {
				a[i] = j;
			}	
			i++;
			j=0;
			k=0;
			c=0;
			startParsing = false;
			isNegative = false;
		}
		
		if(c == '-'){
			isNegative = true;
			continue;
		}
		
		if(startParsing == true){	//read each character representing the digits and compute the whole number
			c = c - '0';	//converting each character to corresponding digit
			j = (j * 10) + c;
			k++;
		}	
	}
fclose(fp);

} else {
	printf("Unable to read the file: %s\n",inputFile); //show error message on the screen in case the the application unable to open file
}

//populating the Inventory structure
inv = (INVENTORY *) malloc(sizeof(INVENTORY));
inv->cameras = a[0];
inv->laptops = a[1];
inv->printers = a[2];

return inv;
}
