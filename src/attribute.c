#include<stdio.h>
#include<stdlib.h>
#include "attribute.h"

Attr attr(Attr_type t){
  Attr a = {.type = t};//, .triggerable = 0};
  return a;
}

Attr rand_attr(){
  int r = rand() % ATTRTYPENUM;
  return attr(r);
}

void print_attr(Attr a){
  printf("Attr type: ");
  switch (a.type){
  case NULL_ATTR:
    printf("NULL\n");break;
  case SPEED_UP:
    printf("SPEED\n");break;
  case WATER_PROOF:
    printf("WATER PROOF\n");break;
  case NAVIGATE:
    printf("NAVIGATE\n");break;
  case FUEL_EFFICIENT:
    printf("FUEL_EFFICIENT\n");break;
  case JET_PACK:
    printf("JET_PACK\n");break;
  case BOOM:
    printf("BOOM\n");break;
  }
}

