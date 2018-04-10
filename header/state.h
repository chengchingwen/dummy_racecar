#pragma once
#define STATETYPENUM 5

typedef enum {
  PLAIN,
  HILL,
  BASIN,
  POOL,
  RAINING
}State_type ;


typedef struct state{
  int offset;
  int width;
  int length;
  State_type type;
}State;

State rand_state(int maplength, int mapwidth);
void print_state(State s);
