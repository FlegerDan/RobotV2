#include "ArduinoMath.h"

int ArduinoMath::constrain(int x, int a, int b){
  if (x < a) return (a);
  if (x > b) return (b);
  return (x);
}

int ArduinoMath::min(int a, int b){
  if (a < b) return (a);
  return (b);
}

int ArduinoMath::max(int a, int b){
  if (a < b) return (b);
  return (a);
}

int ArduinoMath::map(int x, int in_min, int in_max, int out_min, int out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int ArduinoMath::abs(int x){
  if (x < 0) return (-x);
  return (x);
}
