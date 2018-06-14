#include<stdio.h>
#include<stdlib.h>

#include "sh.h"


Queue bq;
Map * _m;

struct itimerval timer;
int END_GAME = 0;

void init_queue(Map *m){
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = 1000;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 0;

  bq.start = 0;
  bq.end = 0;
  for(int i=0;i<30;i++)
    bq.Q[i] = 0;
  _m = m;
}

int qhead(){
  return bq.Q[bq.start];
}

void push(int si){
  bq.Q[bq.end] = si;
  bq.end += 1;
  bq.end %= 30;
}

int pop(){
  int rt = qhead();
  bq.start += 1;
  bq.start %= 30;
  return rt;
}

void boomed(int signum){
  int si = pop();
  printf("boomed at %d\n", si);
  _m->states[si] = nullstate();
}



void set_boom(int si){
  push(si);
  setitimer(ITIMER_VIRTUAL, &timer, NULL);

}

void end_game(int signum){
  END_GAME = 1;
}

