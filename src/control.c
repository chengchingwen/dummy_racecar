#include<stdio.h>
#include<stdlib.h>
#include "control.h"

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
  ret =  ioctl(fd, KEY_IOCTL_CLEAR, &key);
  if (ret < 0)
    return NULLCONTR;
  ret = ioctl(fd, KEY_IOCTL_GET_CHAR, &key);
  switch(ret){
  case VK_S3:
    return FORWARD;
  case VK_S6:
    return LEFT;
  case VK_S8:
    return RIGHT;
  case VK_S11:
    return BACKWARD;
  case VK_S7:
    return START;
  case VK_S5:
    return ATTR1;
  case VK_S9:
    return ATTR2;
  default:
    return NULLCONTR;
  }
}
#endif
