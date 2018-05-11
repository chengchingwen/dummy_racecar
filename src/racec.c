#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>

#include "sockop.h"

int main(){
  srand(time(NULL));
  Client("192.168.0.170", 8888);
  return 0;
}
