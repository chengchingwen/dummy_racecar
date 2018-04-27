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
  ret =  ioctl(fd, KEY_IOCTL_CLEAR, &key);
  if (ret < 0)
    return NULLCONTR;
  ret = ioctl(fd, KEY_IOCTL_GET_CHAR, &key);
  return str2control(ret);
}
#endif

CONTROL str2control(char s){
#ifdef ARM
  switch(s){
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
#else
  switch(s){
  case 'w':
    return FORWARD;
  case 'a':
    return LEFT;
  case 'd':
    return RIGHT;
  case 's':
    return BACKWARD;
  case '\n':
    return START;
  case 'q':
    return ATTR1;
  case 'e':
    return ATTR2;
  default:
    return NULLCONTR;
  }
#endif
}

char ctrl2str(CONTROL ctrl){
#ifdef ARM
  switch(ctrl){
  case FORWARD:
    return VK_S3;
  case LEFT:
    return VK_S6;
  case RIGHT:
    return VK_S8;
  case BACKWARD:
    return VK_S11;
  case START:
    return VK_S7;
  case ATTR1:
    return VK_S5;
  case ATTR2:
    return VK_S9;
  default:
    return '!';
  }
#else
  switch(ctrl){
  case FORWARD:
    return 'w';
  case LEFT:
    return 'a';
  case RIGHT:
    return 'd';
  case BACKWARD:
    return 's';
  case START:
    return '\n';
  case ATTR1:
    return 'q';
  case ATTR2:
    return 'e';
  default:
    return '!';
  }
#endif
}

