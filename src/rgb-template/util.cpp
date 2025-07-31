#include "vex.h"
#include <iostream>
#include <string>

float reduce_0_to_360(float angle) {
  while(!(angle >= 0 && angle < 360)) {
    if( angle < 0 ) { angle += 360; }
    if(angle >= 360) { angle -= 360; }
  }
  return(angle);
}

float reduce_negative_180_to_180(float angle) {
  while(!(angle >= -180 && angle < 180)) {
    if( angle < -180 ) { angle += 360; }
    if(angle >= 180) { angle -= 360; }
  }
  return(angle);
}

float to_deg(float angle_rad){
  return(angle_rad*(180.0/M_PI));
}

float threshold(float input, float min, float max){
  if( input > max ){ return(max); }
  if(input < min){ return(min); }
  return(input);
}

float to_volt(float percent){
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

void print_controller_screen(const char* message) {
  std::string padded_message = message;
  
  // Pad the string to 20 characters with spaces on the right
  while (padded_message.length() < 20) {
    padded_message += " ";
  }
  
  // Print the padded message to the controller screen
  controller(primary).Screen.print("%s", padded_message.c_str());
}

