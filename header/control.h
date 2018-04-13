#pragma once
#ifdef ARM
#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include "asm-arm/arch-pxa/lib/creator pxa270 lcd.h"
#endif

typedef enum{
  NULLCONTR,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  ATTR1,
  ATTR2,
  START
}CONTROL;

CONTROL control(int i);
CONTROL rand_control();

#ifdef ARM
CONTROL arm_control();
#endif

void print_control(CONTROL c);
