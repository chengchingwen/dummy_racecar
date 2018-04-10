#include<stdio.h>
#include<stdlib.h>
#include "state.h"

State state(int o,
            int w,
            int l,
            State_type t){
  State st = {.offset = o,
              .width = w,
              .length = l,
              .type = t
  };
  return st;
}

State rand_state(int maplength, int mapwidth){
  int r, o, w, l;
  r = rand() % STATETYPENUM;
  o = rand() % maplength;
  w = (rand() % mapwidth + (rand() % 2 + 2)) % mapwidth;
  l = (rand() % 20 + (rand() % 3 + 3)) % maplength;
  return state(o, w, l, r);
}

void print_state(State s){
  printf("Offset: %d\n"
         "Width: %d\n"
         "Length: %d\n", s.offset, s.width, s.length);
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

