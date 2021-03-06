#pragma once
#ifdef ARM
#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"
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
#else
CONTROL pc_control();
#endif

CONTROL str2control(char s);
char ctrl2str(CONTROL ctrl);
void print_control(CONTROL c);
