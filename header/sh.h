#pragma once
#include<signal.h>
#include<sys/time.h>

#include "attribute.h"
#include "map.h"

typedef struct queue{
  int start;
  int end;
  int Q[30];
}Queue;

void boomed(int signum);
void init_queue(Map *m);
int qhead();
void push(int si);
int pop();
void boomed(int signum);
void set_boom(int si);
void end_game(int signum);
