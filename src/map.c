#include<stdio.h>
#include<stdlib.h>
#include "map.h"

Map map(int l, int sn, int cn){
  Map m = {.length = l,
           .state_num = sn,
           .car_num = cn,
           .states = NULL,
           .cars = NULL
  };
  return m;
}
void destroy_map(Map m){
  if (m.car_num != 0)
    free(m.cars);
  if (m.state_num != 0)
    free(m.states);
}

void print_map(Map m){
  printf("Map length: %d\n"
         "car #: %d\n", m.length, m.car_num);
  for (int i=0;i<m.car_num;i++){
    print_car(m.cars[i]);
    printf("\n");
  }
  printf("state #: %d\n", m.state_num);
  for(int i=0;i<m.state_num;i++){
    print_state(m.states[i]);
    printf("\n");
  }
}

Map empty_map(int max_car){
  int l, sn;
  l = rand() % MAXMAPLENGTH;
  sn = rand() % 5 + (l / 80);
  Map m = map(l, sn, 0);
  m.cars = malloc(max_car * sizeof(Car));
  m.states = malloc(m.state_num * sizeof(State));
  for (int i=0;i<m.state_num;i++)
    m.states[i] = rand_state(l, MAXMAPWIDTH);
  return m;
}

Map rand_map(){
  int l, sn, cn;
  l = rand() % MAXMAPLENGTH;
  sn = rand() % 5 + (l / 80);
  cn = rand() % 3 + 1;
  Map m = map(l, sn, cn);
  m.cars = malloc(m.car_num * sizeof(Car));
  for (int i=0;i<m.car_num;i++)
    m.cars[i] = rand_car();
  m.states = malloc(m.state_num * sizeof(State));
  for (int i=0;i<m.state_num;i++)
    m.states[i] = rand_state(l, MAXMAPWIDTH);
  return m;
}

int cover(State s, Car c){
  if (abs(c.location - s.offset) <= s.length){
    if (abs(c.bias - s.bias) <= s.width){
      return 1;
    }
  }
  return 0;
}

int check_state(Map m, int c){
  for (int i=0;i<m.state_num;i++){
    if (cover(m.states[i], m.cars[c]))
      return i;
  }
  return -1;
}

int has_car(Map m, int cn, int l, int b){
  int mcl = m.cars[cn].location;
  int lb = mcl-10;
  int ub = mcl+ 10*(DISPLAYLENGTH - 1);
  if (lb < 0) lb = 0;
  int nl = (l - lb) / 10;
  int nb = b + (MAXMAPWIDTH/2);

  for (int i=0;i<m.car_num;i++){
    if (m.cars[i].location > lb && m.cars[i].location < ub && i != cn){
      int lidx = (m.cars[i].location - lb) / 10;
      int widx = m.cars[i].bias + (MAXMAPWIDTH/2);

      if (nl == lidx && nb == widx){
        return lidx * 10 - 1 + lb;
      }
    }
  }
  return 0;
}

int move_car(Map m, int c, int si, CONTROL ctrl){
  if (m.cars[c].location > m.length){
    m.cars[c].phase = END;
  }
  Car_phase p = m.cars[c].phase;
  if (p == END)
    return 0;

  if (p == LOST){
    if (ctrl == START){
      return -1;
    }
    else{
      return 0;
    }
  }

  int speed = m.cars[c].speed;

  if (si != -1){
    State_type st = m.states[si].type;
    switch (st){
    case HILL:
      speed /= 3;
      break;
    case BASIN:
      if (!has_attr(m.cars[c], JET_PACK)){
        int mb = rand() % 15 + 10;
        if (mb < 25){
          if (!has_attr(m.cars[c], NAVIGATE)){
            m.cars[c].phase = LOST;
            return 0;
          };
        }
        else{
          m.cars[c].location -= mb;
        }
      }
      break;
    case POOL:
      speed = (2 * speed) / 3;
      break;
    case RAINING:
      if (!has_attr(m.cars[c], WATER_PROOF)){
        speed = ((rand() % 6 + 1) * speed) / 3;
        m.cars[c].bias += rand() % 3 -1;
      }
      else{
        speed = ((rand() % 2 + 1) * speed);
      }
      break;
    default:
      break;
    }
  }
  if (has_attr(m.cars[c], SPEED_UP)){
    int so10 = speed / 10;
    if (so10 <1){
      so10 = 1;
    }
    m.cars[c].location += (speed + so10);
  }
  else{
    m.cars[c].location += speed;
  }
  int old_l = m.cars[c].location;
  int old_b = m.cars[c].bias;
  update_car(&m.cars[c], ctrl);
  int new_l = m.cars[c].location;
  int new_b = m.cars[c].bias;
  int U_bound;
  if (U_bound = has_car(m, c, new_l, new_b)){
    if (old_b == new_b){
      m.cars[c].location = U_bound;
    }
    else{
      m.cars[c].bias = old_b;
    }
  }
  return 0;
}
