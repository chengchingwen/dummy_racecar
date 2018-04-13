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

void print_time(time_t start, lcd_write_info_t * lcd){
  time_t now;
  time( &now);
  lcd->CursorX = lcd->CursorY = 0;
  ioctl(fd, LCD_IOCTL_CUR_SET, &lcd);
  lcd->Count = sprintf((char *) lcd->Msg,
                      "%.2llf\n", difftime(now, start));
  ioctl(fg, LCD_IOCTL_WRITE, &lcd);
}

void draw_map(Map m, lcd_write_info_t * lcd){
  int mcl = m.cars[0].location;
  int mcb = m.cars[0].bias;
  int lb = mcl-10;
  int ub = mcl+ 10*(DISPLAYLENGTH - 1);
  char dpbuffer[DISPLAYLENGTH][MAXMAPWIDTH];
  for (int i=0;i<DISPLAYLENGTH;i++){
    for (int j=0;j<MAXMAPWIDTH;j++){
      dpbuffer[i][j] = " ";
    }
  }
  for (int i=m.state_num-1;i--;i>-1){
    State s = m.states[i];
    char dps = State2char(s.type);
    if (s.offset+s.length > lb || s.offset - s.length < ub){
      for(int bl=s.offset-s.length;bl<s.offset+s.length;bl+=10){
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
  for(int i=0;i<DISPLAYLENGTH;i++){
    lcd->Count = sprintf((char *) lcd->Msg,
                         "%16s", dpbuffer[i]);
    ioctl(fg, LCD_IOCTL_WRITE, &lcd);
  }
}
#endif
