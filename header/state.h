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
  int bias;
  int width;
  int length;
  State_type type;
}State;

State state(int o,
            int b,
            int w,
            int l,
            State_type t);

State rand_state(int maplength, int mapwidth);
void print_state(State s);
char State2char(State_type st);
