#pragma once

#ifdef ARM
#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"
#endif

#include "param.h"
#include "map.h"

#define LINE( dpbuffer, n, line) \
  for (int i=0; i<16; i++) \
    dpbuffer[n][i] = line[i];

#define DPBUFFER(dpbuffer) \
  char dpbuffer[DISPLAYLENGTH][MAXMAPWIDTH];

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

void getframe(Map m, char dpbuffer[DISPLAYLENGTH][MAXMAPWIDTH]);

void draw_map(Map m, char dpbuffer[DISPLAYLENGTH][MAXMAPWIDTH]
#ifdef ARM
              ,lcd_write_info_t * lcd
#endif
              );

