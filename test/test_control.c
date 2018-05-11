#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include "control.h"

#ifdef ARM
int fd;
#endif

int main(){
  srand(time(NULL));
#ifdef ARM
  if((fd = open("/dev/lcd", O_RDWR)) < 0){
    printf("Open /dev/lcd faild.\n");
    exit(-1);
  }
  printf("fd open success\n");
  //  ioctl(fd, KEY_IOCTL_CLEAR, NULL);
#endif

  for (int i=0;i<10;i++){
    print_control(control(0));
  }
  return 0;
}
