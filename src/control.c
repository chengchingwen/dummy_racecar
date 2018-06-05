#include<stdio.h>
#include<stdlib.h>

#ifndef ARM
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#endif

#include "control.h"

#ifdef ARM
extern int fd;
#endif

#ifndef ARM
void set_mode(int want_key)
{
	static struct termios old, new;
	if (!want_key) {
		tcsetattr(STDIN_FILENO, TCSANOW, &old);
		return;
	}

	tcgetattr(STDIN_FILENO, &old);
	new = old;
	new.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &new);
}

int get_key()
{
	int c = 0;
	struct timeval tv;
	fd_set fs;
	tv.tv_usec = tv.tv_sec = 0;

	FD_ZERO(&fs);
	FD_SET(STDIN_FILENO, &fs);
	select(STDIN_FILENO + 1, &fs, 0, 0, &tv);

	if (FD_ISSET(STDIN_FILENO, &fs)) {
		c = getchar();
  
	}
	return c;
}

char pc_map(char c){
  switch(c){
  case 'w':
    return '2';
  case 'a':
    return '4';
  case 'd':
    return '6';
  case 's':
    return '8';
  case 'x':
    return '5';
  case 'q':
    return '1';
  case 'e':
    return '3';
  default:
    return '!';
  }

}


CONTROL pc_control(){
  set_mode(1);
  char c;
  for (int i=0;i<5000;i++){
    c = get_key();
    if (c!=0){
      c = pc_map(c);
      set_mode(0);
      return str2control(c);
    }
  }
  set_mode(0);
  return NULLCONTR;
}
#endif
CONTROL control(int i){
  if (i==0){
#ifdef ARM
    return arm_control();
#else
    return pc_control();
#endif
  }
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

