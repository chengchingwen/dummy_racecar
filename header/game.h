#pragma once
#include "param.h"
#include "map.h"
#include "car.h"
#include "state.h"
#include "display.h"
#ifdef ARM
#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"
#endif



void run(Map m);
int end(Map m);
