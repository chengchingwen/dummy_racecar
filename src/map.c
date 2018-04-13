#include<stdio.h>
#include<stdlib.h>
#include "map.h"

Map map(int l, int sn, int cn, Car c){
  Map m = {.length = l,
           .state_num = sn,
           .car_num = cn,
           .states = NULL,
           .cars = NULL
  };
  m.cars = malloc(m.car_num * sizeof(Car));
  m.cars[0] = c;
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

Map rand_map(){
  int l, sn, cn;
  l = rand() % MAXMAPLENGTH;
  sn = rand() % 5;
  cn = rand() % 3 + 1;
  Map m = map(l, sn, cn, rand_car());
  for (int i=1;i<m.car_num;i++)
    m.cars[i] = rand_car();
  m.states = malloc(m.state_num * sizeof(State));
  for (int i=0;i<m.state_num;i++)
    m.states[i] = rand_state(100,10);
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

int move_car(Map m, int c, int si){
  if (m.cars[c].location > m.length){
    m.cars[c].phase = END;
  }
  Car_phase p = m.cars[c].phase;
  if (p == END)
    return 0;

  CONTROL ctrl = control(c);

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
        if (mb < 20){
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
  update_car(&m.cars[c], ctrl);
  return 0;
}
