#pragma once
#define ATTRTYPENUM 7

typedef enum{
  NULL_ATTR,
  SPEED_UP,
  WATER_PROOF,
  NAVIGATE,
  FUEL_EFFICIENT,
  JET_PACK,
  BOOM
}Attr_type;


typedef struct attribute{
  Attr_type type;
  /* int triggerable; */
}Attr;

Attr attr(Attr_type t);
Attr rand_attr();
void print_attr(Attr a);
