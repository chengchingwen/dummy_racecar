#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>

#include "sockop.h"

int main(){
  srand(time(NULL));
  Map m = empty_map(30);
  Server("0.0.0.0", 8888, m);
  destroy_map(m);
  return 0;
}
