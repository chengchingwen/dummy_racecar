#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>

#include "sockop.h"
#include "game.h"

int Client(const char* addr, int port);

#ifdef ARM
int fd;
#endif

int main(){
  srand(time(NULL));
  Client("127.0.0.1", 8888);
  return 0;
}


int Client(const char* addr, int port){
  int sockfd;
  struct sockaddr_in address;
  bzero(&address,sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(addr);
  address.sin_port = htons(port);

  if( (sockfd = socket(AF_INET , SOCK_STREAM , 0)) == 0){
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  if(connect(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0){
    printf("\n Error : Connect Failed \n");
    return 1;
  }

  int E;
  time_t start;
  DPBUFFER(dpbuffer);
  //prepare for arm board lcd
#ifdef ARM
  lcd_write_info_t lcd;
  if((fd = open("/dev/lcd", O_RDWR)) < 0){
    printf("Open /dev/lcd faild.\n");
    exit(-1);
  }
  init_lcd();
#endif

  int readn;
  //start time
  time(&start);
  //recv(sockfd, dpbuffer[0], 2, 0);
  while(1){
    CONTROL ctrl = control(0);
    dpbuffer[0][0] = ctrl2str(ctrl);
    if(send(sockfd, dpbuffer, 1, 0) != 1){
      perror("send");
    }

    if((readn = read(sockfd, dpbuffer[0], 2)) !=0){
      dpbuffer[0][readn] = '\0';
      sevseg(atoi(dpbuffer[0]));
    }
    else
      break;

    if((readn = read(sockfd, dpbuffer[0], 1)) !=0){
      dpbuffer[0][readn] = '\0';
      if(atoi(dpbuffer[0]) == 1)
        spark_led();
    }
    else
      break;

    if((readn = read(sockfd, dpbuffer, DPBUFSIZE)) !=0){
#ifdef ARM
      print_time(start, &lcd);
      draw_map(dpbuffer, &lcd);
#else
      print_time(start);
      draw_map(dpbuffer);
      printf("\033[1A");
#endif
    }
    else
      break;

    usleep(200000);
    if((readn = read(sockfd, dpbuffer, 1)) !=0)
      E = dpbuffer[0][0] - '0';
    else
      break;

    if (E){
#ifndef ARM
      printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
#endif
      break;
    }
  }

  close(sockfd);
  return 0;
}
