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

void print_controller_screen(const char* message) {
  std::string paddedMessage = message;
  
  // Pad the string to 20 characters with spaces on the right
  while (paddedMessage.length() < 20) {
    paddedMessage += " ";
  }
  
  // Print the padded message to the controller screen
  controller(primary).Screen.print("%s", paddedMessage.c_str());
}

// Converts and cleans a command string for processing
std::string cleanCommand(const std::string& command) {
    // Remove any whitespace and newlines
    std::string cmd = command;
    cmd.erase(0, cmd.find_first_not_of(" \t\r\n"));
    cmd.erase(cmd.find_last_not_of(" \t\r\n") + 1);
    
    // Convert to uppercase for case-insensitive comparison
    for (char& c : cmd) {
        c = toupper(c);
    }

        
    // Debug: print the cmd value
    controller(primary).Screen.clearScreen();
    char cmdDebug[30];
    sprintf(cmdDebug, "cmd: '%s'", cmd.c_str());
    printControllerScreen(cmdDebug);
    
    return cmd;
}

