#include<stdio.h>
#include<stdlib.h>
#include "state.h"

State state(int o,
            int b,
            int w,
            int l,
            State_type t){
  State st = {.offset = o,
              .bias = b,
              .width = w,
              .length = l,
              .type = t
  };
  return st;
}

State rand_state(int maplength, int mapwidth){
  int r, o, b, w, l;
  r = rand() % STATETYPENUM;
  o = rand() % maplength + 30;
  b = rand() % (mapwidth - 3) - (mapwidth / 2);
  w = (rand() % mapwidth + (rand() % 2 + 2)) % mapwidth;
  l = (rand() % 50 + (rand() % 3 + 3)) % maplength;
  return state(o, b, w, l, r);
}

void print_state(State s){
  printf("Offset: %d\n"
         "Bias: %d\n"
         "Width: %d\n"
         "Length: %d\n", s.offset, s.bias, s.width, s.length);
  printf("State type: ");
  switch (s.type){
  case PLAIN:
    printf("PLAIN\n");break;
  case HILL:
    printf("HILL\n");break;
  case BASIN:
    printf("BASIN\n");break;
  case POOL:
    printf("POOL\n");break;
  case RAINING:
    printf("RAINING\n");break;
  }
}

char State2char(State_type st){
  switch(st){
  case HILL:
    return 'H';
  case BASIN:
    return 'L';
  case POOL:
    return 'W';
  case RAINING:
    return 'R';
  default:
    return ' ';
  }
}
