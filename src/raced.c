#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>

#include "sockop.h"
#include "game.h"
int Server(const char * addr, int port, Map m);


int main(){
  srand(time(NULL));
  Map m = empty_map(30);
  Server("127.0.0.1", 8888, m);
  destroy_map(m);
  return 0;
}

int Server(const char * addr, int port, Map m){
  int opt = 1;
  int master_socket , addrlen , new_socket , client_socket[30] ,
    max_clients = 30 , activity, i , valread , sd;
  int max_sd;
  int E;
  CONTROL ctrl_list[30];

  struct sockaddr_in address;
  char buffer[3];  //data buffer of 1K
  char rcbuf[3];

  //set of socket descriptors
  fd_set readfds;
  //a message
  char *message = "ECHO Daemon v1.0 \r\n";

  //initialise all client_socket[] to 0 so not checked
  for (i = 0; i < max_clients; i++){
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

  DPBUFFER(sendbuf);

  while(1){
    //clear the socket set
    FD_ZERO(&readfds);

    //add master socket to set
    FD_SET(master_socket, &readfds);
    max_sd = master_socket;

    //add child sockets to set
    for ( i = 0 ; i < max_clients ; i++){
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
      for (i = 0; i < max_clients; i++){
        //if position is empty
        if( client_socket[i] == 0 ){
          client_socket[i] = new_socket;
          m.car_num += 1;
          m.cars[i] = rand_car();
          printf("Adding to list of sockets as %d\n" , i);
          break;
        }
      }
    }

    //else its some IO operation on some other socket
    for (i = 0; i < max_clients; i++){
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
          sprintf(rcbuf,"%2d", m.car_num);
          if(send(sd, rcbuf, 2, 0) != 2){
            perror("send");
          }
          //update game
          int si = check_state(m, i);
          //spark_led();
          if (si != -1)
            rcbuf[0] = '1';
          else
            rcbuf[0] = '0';
          if(send(sd, rcbuf, 1, 0) != 1){
            perror("send");
          }

          ctrl_list[i] = str2control(buffer[0]);
          move_car(m, i, si, ctrl_list[i]);

          //set the string terminating NULL byte on the end
          //of the data read
          getframe(m, i, sendbuf);
          send(sd , sendbuf , DPBUFSIZE , 0 );
          E = end(m);
          sprintf(rcbuf,"%d", E);
          if(send(sd, rcbuf, 1, 0) != 1)
            perror("send");
        }
      }
    }

    /* E = end(m); */
    /* for (i = 0; i < max_clients; i++){ */
    /*   sd = client_socket[i]; */
    /*   if (sd > 0){ */
    /*     sprintf(rcbuf,"%d", E); */
    /*     if(send(sd, rcbuf, 1, 0) != 1){ */
    /*       perror("send"); */
    /*     } */
    /*   } */
    /* } */

    if (E){
      break;
    }
  }
  return 0;
}
