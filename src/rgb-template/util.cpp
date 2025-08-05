#include "vex.h"

float reduce_0_to_360(float angle) {
  return fmod(angle + 360, 360);
}

float reduce_negative_180_to_180(float angle) {
  return fmod(angle + 540, 360) - 180;
}

float threshold(float input, float min, float max){
  if( input > max ){ return(max); }
  if(input < min){ return(min); }
  return(input);
}

float toVolt(float percent){
  return(percent*12.0/100.0);
}

float deadband(float input, float width){
  if (fabs(input)<width){
    return(0);
  }
  return(input);
}

bool checkMotors(int motorCount, int temperatureLimit) {
  int count = 0;
  int t = 0;
  for (int i = 0; i < 20; i++) {
    motor m = motor(i);
    if (m.installed()) {
      count++;
      t = m.temperature(celsius);
      if (t > temperatureLimit) {
        controller(primary).Screen.print("motor %d is %dC           ", i + 1, t);
        controller(primary).rumble("---");
        return false;
      }
    }
  }
  if (count < motorCount) {
    controller(primary).Screen.print("%d motor is disconnected      ", motorCount - count);
    controller(primary).rumble("---");
    return false;
  }
  return true;
}

void printControllerScreen(const char* message) {
  char padded[21];
  snprintf(padded, sizeof(padded), "%-20s", message);
  controller(primary).Screen.print("%s", padded);
}


