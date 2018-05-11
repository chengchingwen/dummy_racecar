#include<stdio.h>
#include<stdlib.h>
#include "control.h"

#ifdef ARM
extern int fd;
#endif


CONTROL control(int i){
#ifdef ARM
  if (i==0){
    return arm_control();
  }
#endif
  return rand_control();
}


CONTROL rand_control(){
  int r = rand() % 4 + 1;
  return r;
}

void print_control(CONTROL c){
  printf("ctrl: ");
  switch(c){
  case NULLCONTR:
    printf("NULL-ctrl\n");break;
  case FORWARD:
    printf("forward\n");break;
  case BACKWARD:
    printf("backward\n");break;
  case LEFT:
    printf("left\n");break;
  case RIGHT:
    printf("right\n");break;
  case ATTR1:
    printf("use attr1\n");break;
  case ATTR2:
    printf("use attr2\n");break;
  case START:
    printf("Start!\n");break;
  }
}

#ifdef ARM
CONTROL arm_control(){
  unsigned short key;
  int ret;
  for(int i=0;i<5000;i++){
    ret = ioctl(fd, KEY_IOCTL_CHECK_EMTPY, &key);
    if(ret < 0){
      continue;
    }
    ret = ioctl(fd, KEY_IOCTL_GET_CHAR, &key);
    return str2control(key & 0xff);
  }
  return NULLCONTR;
}
#endif

CONTROL str2control(char s){
  switch(s){
  case '2':
    return FORWARD;
  case '4':
    return LEFT;
  case '6':
    return RIGHT;
  case '8':
    return BACKWARD;
  case '5':
    return START;
  case '1':
    return ATTR1;
  case '3':
    return ATTR2;
  default:
    return NULLCONTR;
  }
}

char ctrl2str(CONTROL ctrl){
  switch(ctrl){
  case FORWARD:
    return '2';
  case LEFT:
    return '4';
  case RIGHT:
    return '6';
  case BACKWARD:
    return '8';
  case START:
    return '5';
  case ATTR1:
    return '1';
  case ATTR2:
    return '3';
  default:
    return '!';
  }
}

