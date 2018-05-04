#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>

#include "sockop.h"


#ifdef ARM
int fd;
#endif

int main(){
  srand(time(NULL));
  Client("127.0.0.1", 8888);
  return 0;
}
