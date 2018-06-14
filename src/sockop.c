#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>

//fake
#include<pthread.h>

#include "sockop.h"
#include "game.h"
#include "sh.h"

//fake
pthread_mutex_t mutex;

#ifdef ARM
extern int fd;
#endif

extern int END_GAME;


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
  char sendbuf[3];
  //prepare for arm board lcd
#ifdef ARM
  lcd_write_info_t lcd;
  if((fd = open("/dev/lcd", O_RDWR)) < 0){
    printf("Open /dev/lcd faild.\n");
    exit(-1);
  }
  init_lcd();
  ioctl(fd, KEY_IOCTL_CLEAR, NULL);
#endif

  int readn;
  //start time
  time(&start);
  //recv(sockfd, dpbuffer[0], 2, 0);
  while(1){
    CONTROL ctrl = control(0);
    sendbuf[0] = ctrl2str(ctrl);
    SEND(sockfd, sendbuf, 1);

    READ_START(readn, sockfd, dpbuffer[0], 2) {
      dpbuffer[0][readn] = '\0';
      sevseg(atoi(dpbuffer[0]));
    } READ_END;

    READ_START(readn, sockfd, dpbuffer[0], 1) {
      dpbuffer[0][readn] = '\0';
      if(atoi(dpbuffer[0]) == 1)
        spark_led();
      else
	turn_off_led();
    } READ_END;

    READ_START(readn, sockfd, dpbuffer, DPBUFSIZE){
#ifdef ARM
      print_time(start, &lcd);
      draw_map(dpbuffer, &lcd);
#else
      print_time(start);
      draw_map(dpbuffer);
      printf("\033[1A");
#endif
    } READ_END;

    usleep(1000);

    READ_START(readn, sockfd, dpbuffer, 1){
      E = dpbuffer[0][0] - '0';
    }

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


int Server(const char * addr, int port, Map m){
  int opt = 1;
  int master_socket , addrlen , new_socket , client_socket[30] ,
    max_clients = 30 , activity, valread , sd;
  int max_sd;
  int E = 0;
  CONTROL ctrl_list[30];

  struct sockaddr_in address;
  char buffer[3];  //data buffer of 1K
  char rcbuf[3];

  //set of socket descriptors
  fd_set readfds;
  //a message

  //initialise all client_socket[] to 0 so not checked
  for (int i = 0; i < max_clients; i++){
    client_socket[i] = 0;
    ctrl_list[i] = NULLCONTR;
  }

  //create a master socket
  if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0){
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  //set master socket to allow multiple connections ,
  //this is just a good habit, it will work without this
  if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                 sizeof(opt)) < 0 ){
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  //type of socket created
  bzero(&address,sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(addr);
  address.sin_port = htons(port);

  //bind the socket
  if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0){
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  printf("Listener on port %d \n", port);

  //try to specify maximum of 3 pending connections for the master socket
  if (listen(master_socket, 3) < 0){
    perror("listen");
    exit(EXIT_FAILURE);
  }
  //accept the incoming connection
  addrlen = sizeof(address);
  puts("Waiting for connections ...");

  //boom handler
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = &boomed;
  sigaction(SIGVTALRM, &sa, NULL);
  init_queue(&m);
  //end handler
  signal(SIGUSR1, end_game);

  DPBUFFER(sendbuf);

  while(1){
    //clear the socket set
    FD_ZERO(&readfds);

    //add master socket to set
    FD_SET(master_socket, &readfds);
    max_sd = master_socket;

    //add child sockets to set
    for (int i = 0 ; i < max_clients ; i++){
      //socket descriptor
      sd = client_socket[i];
      //if valid socket descriptor then add to read list
      if(sd > 0)
        FD_SET( sd , &readfds);

      //highest file descriptor number, need it for the select function
      if(sd > max_sd)
        max_sd = sd;
    }

    //wait for an activity on one of the sockets , timeout is NULL ,
    //so wait indefinitely
    activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
    if ((activity < 0) && (errno!=EINTR)){
      printf("select error");
    }

    //If something happened on the master socket ,
    //then its an incoming connection
    if (FD_ISSET(master_socket, &readfds)){
      if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
        perror("accept");
        exit(EXIT_FAILURE);
      }

      //inform user of socket number - used in send and receive commands
      printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr), ntohs(address.sin_port));
      //send new connection greeting message
      /* puts("Welcome message sent successfully"); */

      //add new socket to array of sockets
      for (int i = 0; i < max_clients; i++){
        //if position is empty
        if( client_socket[i] == 0 ){
          client_socket[i] = new_socket;

          //fake
          pthread_mutex_lock(&mutex);

          m.car_num += 1;
          m.cars[i] = rand_car();
          printf("Adding to list of sockets as %d\n" , i);

          //fake
          pthread_mutex_unlock(&mutex);

          break;
        }
      }
    }

    //else its some IO operation on some other socket
    for (int i = 0; i < max_clients; i++){
      sd = client_socket[i];
      if (FD_ISSET( sd , &readfds)){
        //Check if it was for closing , and also read the
        //incoming message
        if ((valread = read( sd , buffer, 1)) == 0){
          //Somebody disconnected , get his details and print
          getpeername(sd , (struct sockaddr*)&address , \
                      (socklen_t*)&addrlen);
          printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
          //Close the socket and mark as 0 in list for reuse
          close( sd );
          client_socket[i] = 0;
        }
        else{

          //fake
          pthread_mutex_lock(&mutex);

          sprintf(rcbuf,"%2d", m.car_num);
          SEND(sd, rcbuf, 2);

          //update game
          int si = check_state(m, i);

          //spark_led();
          if (si != -1)
            rcbuf[0] = '1';
          else
            rcbuf[0] = '0';

          SEND(sd, rcbuf, 1);

          ctrl_list[i] = str2control(buffer[0]);
          move_car(m, i, si, ctrl_list[i]);

          //set the string terminating NULL byte on the end
          //of the data read
          getframe(m, i, sendbuf);
          SEND(sd, sendbuf, DPBUFSIZE);
          sb_win(m);
          E = end(m);

          sprintf(rcbuf,"%d", E);
          SEND(sd, rcbuf, 1);

          //fake
          pthread_mutex_unlock(&mutex);

        }
      }
    }

    if (E || END_GAME){
      break;
    }
  }
  return 0;
}
