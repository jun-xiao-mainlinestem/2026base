#include "vex.h"
#include "serial_communication.h"
#include <iostream>
using namespace serial;

void test_serial_communication() {
  SerialCommunication serial;
  
  // Set up message callback
  serial.onMessage([](const std::string& message) {
    controller(primary).Screen.print("Received: %s", message.c_str());
  });
  
  // Set up error callback
  serial.onError([](const std::string& error) {
    controller(primary).Screen.print("Error: %s", error.c_str());
  });
  
  // Connect to serial communication
  if (serial.connect()) {
    controller(primary).Screen.print("Serial connected!");
    
    // Send a test message
    serial.send("Hello from VEX V5!\n");
    
    // Poll for messages for 10 seconds
    Brain.Timer.clear();
    while (serial.isConnected() && Brain.Timer.time(sec) < 10) {
      serial.poll();
      wait(50, msec);
    }
    
    // Disconnect
    serial.disconnect();
  } else {
    controller(primary).Screen.print("Failed to connect serial");
  }
}


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

bool check_motors(int motor_count, int temperature_limit) {
  int count = 0;
  int t = 0;
  for (int i = 0; i < 20; i++) {
    motor m = motor(i);
    if (m.installed()) {
      count++;
      t = m.temperature(celsius);
      if (t > temperature_limit) {
        controller(primary).Screen.print("motor %d is %dC           ", i + 1, t);
        controller(primary).rumble("---");
        return false;
      }
    }
  }
  if (count < motor_count) {
    controller(primary).Screen.print("%d motor is disconnected      ", motor_count - count);
    controller(primary).rumble("---");
    return false;
  }
  return true;
}

