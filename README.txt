									README.TXT
									----------

FULL NAME: MOHIT AGGARWAL
STUDENT ID: 9243363852



DESCRIPTION
------------
The application simulates the activity of a chain marts where each store sends the information of its inventory to the central warehouse and based on the data received the central warehouse will load the deficit inventory in the truck to the stores. This simulation is divided into 2 pahses. 
In Phase-I the central warehouse will create a TCP socket, bind to a well known port and start listening for incoming connections. Each store (from 1-4) will read its inventory from a text file Store-#.txt and store in an INVENTORY structure. Each store will then creates TCP socket and establish connection with the server running at the central warehouse on a well known port and send the inventory over the connection. The central warehouse receives each store's connection and creates a child socket to receives incoming data. Once data is received the ware house stores that information and close the socket. The store also closes the socket once it sends data to the warehouse.
Once warehouse receives data from all 4 stores then it will tear down the parent TCP socket and aggregates the inventory information. Based on this information the warehouse will prepare a truck vector for Phase-II. Both the central warehouse and stores will display messages as given in the project description for Phase-I.

In Phase-II each store will create a UDP socket to receive data from the store that preceeds it in number (e.g store 2 will be waiting to received data from store 1 etc.). The warehouse establishes a UDP connection with the store 1 and send the truck vector to it. The store 1 will update his outlet inventory and the truck vector. The store 1 will then pass the updated truck vector to store 2. Store 2 updates its outlet inventory and truck vector and send the updated truck vector to store 3 and so on. Once store 4 receives the truck vector it updates its outlet inventory and truck vector, and send the updated truck vector back to store 1 for round 2 of updates to outlet inventories and truck vector across all stores.
At the end of round 2 the store 4 sends the updated truck vector to cenrtal warehouse. By this point the outlet inventory of of each store will be zero and each store executable will come out of main(). The warehouse after receiving the truck vector from store 4 will print that information and come out of main(). Once each store or warehouse sends or receives the data it closes the corresponding UDP socket.
During Phase-II each store and central warehouse will display messages on the console as described in the project description.
Also the list of Port numbers used by the warehouse and stores for Phase-I and II as follows.

----------------------------------------------------------------------------------------------------------
Process				|	Dynamic Ports	|	Static Ports												
----------------------------------------------------------------------------------------------------------
Central Warehouse		|			|	1 TCP, 21952 (last three digits of your ID) (phase 1)
				|			|	1 UDP, 31952 (last three digits of your ID) (phase 2 Part 1)
				|			|	1 UDP, 32952 (last three digits of your ID) (phase 2 Part 3)
----------------------------------------------------------------------------------------------------------
Store1				|	1 TCP(phase 1)	|	1 UDP, 5952 (last digits of your ID) (phase 2 part1)
				|			|	1 UDP, 6952 (last digits of your ID) (phase 2 part2)
				|			|	1 UDP, 7952 (last digits of your ID) (phase 2 part2)
				|			|	1 UDP, 8952 (last digits of your ID) (phase 2 part2)
----------------------------------------------------------------------------------------------------------								
Store2				|	1 TCP(phase 1)	|	1 UDP, 9952 (last digits of your ID) (phase 2 part2)
				|			|	1 UDP, 10952 (last digits of your ID) (phase 2 part2)
				|			|	1 UDP, 11952 (last digits of your ID) (phase 2 part2)
				|			|	1 UDP, 12952 (last digits of your ID) (phase 2 part2)
-----------------------------------------------------------------------------------------------------------
Store3				|	1 TCP(phase 1)	|	1 UDP, 13952 (last digits of your ID) (phase 2 part2)
				|			|	1 UDP, 14952 (last digits of your ID) (phase 2 part2)
				|			|	1 UDP, 15952 (last digits of your ID) (phase 2 part2)
				|			|	1 UDP, 16952 (last digits of your ID) (phase 2 part2)
------------------------------------------------------------------------------------------------------------
Store4				|	1 TCP(phase 1)	|	1 UDP, 17952 (last digits of your ID) (phase 2 part2)
				|			|	1 UDP, 18952 (last digits of your ID) (phase 2 part2)
				|			|	1 UDP, 19952 (last digits of your ID) (phase 2 part2)
				|			|	1 UDP, 20952 (last digits of your ID) (phase 2 part2)
					
					



PACKAGE CONTENT
---------------
The package includes following files:
1) warehouse.c : This C file defines functions pertainig to Centeral Warehouse operation.
2) store1.c: This C file defines functions pertainig to Store 1 operation.
3) store2.c: This C file defines functions pertainig to Store 2 operation.
4) store3.c: This C file defines functions pertainig to Store 3 operation.
5) store4.c: This C file defines functions pertainig to Store 4 operation. 
6) inventory.c: This C file defines functions pertaining to parsing the Store-#.txt file and populating the corresponding INVENTORY structure.
7) inventory.h: This is the header file corresponding to inventory.c file. This file declares the parsing function and INVENTORY structure.
8) tcpclient.c: This C file defines functions pertaining to TCP client transactions.
9) tcpclient.h: This is the header file corresponding to the tcpclient.c file. This file declares the TCP client functions.
10) datasoc.c: This C file defines function related to  UDP transactions between the receiver and senders.
11) datasoc.h: This is the header file corresponding to datasoc.c file. This file declares the UDP transaction functions.
12) nethelper.c: This C file defines utility functions used for the transaction between the server and the client.
13) nethelper.h: This is the header file corresponding to nethelper.c file. This file declares the network related utility function.
14) Makefile: This is the make file which when run creates 5 executables, 4 corresponding to each store and 1 for the warehouse.
15) README.txt: This is the read me file provinding general description about the project to the end user.



BUILD INSTRUCTIONS
------------------
The user should extract the contents of .gz file in a folder. And execute the make command. The output will be 5 executables, which are as follows.
1) warehouse: Executable for warehouse
2) store1: Executable for store1
3) store2: Executable for store2
4) store3: Executable for store3
5) store4: Executable for store4



EXECUTION INSTRUCTIONS
----------------------
The user should copy the Store-#.txt files corresponding to each store in the same folder where he/she extracted the contents .gz file. It is important that executables and Store-#.txt files should be included in the same folder on nunki.usc.edu.
The user should start execution of project with executing warehouse and then followed by stores. At the end of Phase II all five executables will come out of main().



MESSAGE FORMAT
--------------
All messages are transacted via character arrays. Before sending the data the outlet inventory or truck vector is serialized to character array and after receiving the data it is de-serialized into truck vector for further calulcation at the store or warehouse.



ACKNOWLEDGMENT
--------------
The core APIs related to TCP and UDP transactions are re-used from the Beej's guide and such usage is acknowledged appropriately in the source code of the project.
