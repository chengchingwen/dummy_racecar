#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "state.h"

int main(){
  srand(time(NULL));
  State s = rand_state(100, 10);//state(0,1,2,3, PLAIN);
  print_state(s);
  return 0;
}
