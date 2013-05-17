#ifndef TCPCLIENT_H
#define TCPCLIENT_H

void establishConnection(int store_no);
void sendData(INVENTORY *data);
void releaseConnection();

#endif
