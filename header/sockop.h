#pragma once
#ifndef _SOCKOPH_
#define _SOCKOPH_

#include<arpa/inet.h>
#include<errno.h>
#include<netdb.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/time.h>
#include "map.h"

#define errexit(format, arg...) exit(printf(format, ##arg))

#define SEND(FD, BUF, LEN) if(send(FD, BUF, LEN, 0) != LEN) perror("send");

#define READ_START(GLEN, FD, BUF, LEN) if((GLEN = read(FD, BUF, LEN)) !=0)

#define READ_END else \
    break;

int Client(const char* addr, int port);
int Server(const char * addr, int port, Map m);

#endif
/*_SOCKOP_H_*/








