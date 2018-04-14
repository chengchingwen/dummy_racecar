#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "display.h"
//#include "map.h"

int main(){
  Map m = map(100, 1, 1, rand_car());
  m.states = malloc(sizeof(State));
  m.states[0] = state(10, 0, 1, 30, POOL);
  print_map(m);
  printf("\n");
  draw_map(m);
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  destroy_map(m);
  return 0;
}
