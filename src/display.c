#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include "display.h"

void spark_led(){
#ifdef ARM
  static unsigned short data;
  if (data == LED_ALL_ON) data = LED_ALL_OFF;
  else if (data == LED_ALL_OFF) data = LED_ALL_ON;
  ioctl(fd, LED_IOCTL_SET, &data);
#else
  printf("led!");
#endif
}

void sevseg(int n){
#ifdef ARM
  _7seg_info_t data;
  ioctl(fd, _7SEG_IOCTL_ON, NULL);
  data.Mode = _7SEG_MODE_HEX_VALUE;
  data.Which = _7SEG_D8_INDEX;
  data.Value = (unsigned long) n;
  ioctl (fd, _7SEG_IOCTL_SET, &data);
  ioctl(fd, _7SEG_IOCTL_OFF, NULL);
#else
  printf("%d cars\n", n);
#endif
}

#ifdef ARM
void init_lcd(){
  ioctl(fd, LCD_IOCTL_CLEAR, NULL);
}
#endif


void print_time(time_t start
#ifdef ARM
                ,lcd_write_info_t * lcd
#endif
                ){
  time_t now;
  time(&now);
  double diff = difftime(now, start);
#ifdef ARM
  lcd->CursorX = lcd->CursorY = 0;
  ioctl(fd, LCD_IOCTL_CUR_SET, &lcd);
  lcd->Count = sprintf((char *) lcd->Msg,
                       "Time: %.2f\n", diff);
  ioctl(fg, LCD_IOCTL_WRITE, &lcd);
#else
  printf("Time: %.2f\n", diff);
#endif
}

void draw_map(Map m
#ifdef ARM
              ,lcd_write_info_t * lcd
#endif
              ){
  char start[16] = "+=====START=====";
  char final[16] = "+=====FINAL=====";
  char lost1[16] = "================";
  char lost2[16] = "=     LOST     =";
  char lost3[16] = "================";

  int ml = m.length;
  int mcl = m.cars[0].location;
  int lb = mcl-10;
  int ub = mcl+ 10*(DISPLAYLENGTH - 1);
  char dpbuffer[DISPLAYLENGTH][MAXMAPWIDTH];
  if (lb < 0) lb = 0;

  //init display buffer
  for (int i=0;i<DISPLAYLENGTH;i++){
    for (int j=0;j<MAXMAPWIDTH;j++){
      dpbuffer[i][j] = ' ';
    }
  }

  //print lost if main car lost
  if(m.cars[0].phase == LOST){
    for(int i=0; i<16;i++){
      dpbuffer[DISPLAYLENGTH/2 -1][i] = lost1[i];
    }
    for(int i=0; i<16;i++){
      dpbuffer[DISPLAYLENGTH / 2][i] = lost2[i];
    }
    for(int i=0; i<16;i++){
      dpbuffer[DISPLAYLENGTH/2 +1][i] = lost3[i];
    }
  }

  //plot state
  for (int i=m.state_num-1;i--;i>-1){
    State s = m.states[i];
    char dps = State2char(s.type);

    if (s.type == PLAIN) continue;

    int sub = s.offset+s.length;
    int slb = s.offset - s.length;
    if (sub > lb || slb < ub){
      for(int bl = slb; bl < sub; bl+=10){
        if (bl > ml) break;
        int lidx = (bl-lb)/10;
        if (lidx >= DISPLAYLENGTH) break;
        else if (lidx >= 0){
          for(int widx=s.bias-s.width+(MAXMAPWIDTH/2);widx<s.bias+s.width+(MAXMAPWIDTH/2);widx++){
            dpbuffer[lidx][widx] = dps;
          }
        }
      }
    }
  }

  //plot start line
  if (lb <= 0 && 0 < ub){
    int lidx = (0 - lb) / 10;
    for (int i=0; i<16; i++){
      dpbuffer[lidx][i] = start[i];
    }
  }

  //plot final line
  if (lb <= ml && ml < ub){
    int lidx = (ml - lb) / 10;
    for (int i=0; i<16; i++){
      dpbuffer[lidx][i] = final[i];
    }
  }

  //print cars
  for (int i=0;i<m.car_num;i++){
    if (m.cars[i].location > lb && m.cars[i].location < ub){
      int lidx = (m.cars[i].location - lb) / 10;
      int widx = m.cars[i].bias + (MAXMAPWIDTH/2);
      if (i == 0)
        dpbuffer[lidx][widx] = '&';
      else
        dpbuffer[lidx][widx] = '@';
    }
  }
  for(int i=DISPLAYLENGTH-1;i>-1;i--){
#ifdef ARM
    lcd->Count = sprintf((char *) lcd->Msg,
                         "%.*s", MAXMAPWIDTH, &dpbuffer[i]);
    ioctl(fg, LCD_IOCTL_WRITE, &lcd);
#else
    printf("%.*s\n", MAXMAPWIDTH, &dpbuffer[i]);
#endif
  }
#ifndef ARM
  printf("\033[15A");
#endif
}