#pragma once

#ifdef ARM
#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include "asm-arm/arch-pxa/lib/creator pxa270 lcd.h"
#endif

#include "param.h"
#include "map.h"


void spark_led();
void sevseg(int n);

#ifdef ARM
void init_lcd();
#endif

void print_time(time_t start
#ifdef ARM
                ,lcd_write_info_t * lcd
#endif
                );
void draw_map(Map m
#ifdef ARM
              ,lcd_write_info_t * lcd
#endif
              );

