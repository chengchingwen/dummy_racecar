#pragma once
#define ATTRTYPENUM 6

typedef enum{
  NULL_ATTR,
  SPEED_UP,
  WATER_PROOF,
  NAIVGATE,
  FUEL_EFFICIENT,
  JET_PACK
}Attr_type;


typedef struct attribute{
  Attr_type type;
  /* int triggerable; */
}Attr;

Attr attr(Attr_type t);
Attr rand_attr();
void print_attr(Attr a);
