// vex.h includes all of the headers for the VEX V5 library
#include "vex.h"

// All vex classes and functions are in the vex namespace
using namespace vex;

// A global instance of competition
// This object is used to register callbacks for the autonomous and driver control periods.
competition Competition;

// This function is called when the L1 button is pressed.
void sampleButtonL1Action() {
  inTake();
  
  // Wait until the button is released to stop the rollers.
  while(controller1.ButtonL1.pressing()) {
    if(controller1.ButtonL2.pressing()) colorSort();
    wait (20, msec);
  }
  stopRollers();
}

bool matchLoaderDown = false;
void buttonL2Action() {
  if(controller1.ButtonL1.pressing()) 
  {
    matchLoaderDown = !matchLoaderDown;
    // toggle the matchloader position
    return;
  }
  outTake();
  // Wait until the button is released to stop the rollers.
  while(controller1.ButtonL2.pressing()) {
    wait (20, msec);
  }
  stopRollers();
}

void buttonR1Action() {
  chassis.stop(hold);
  if(controller1.ButtonR2.pressing()) scoreMiddle();
  else scoreLong();

  // Wait until the button is released to stop the rollers.
  while(controller1.ButtonR1.pressing()) {
    wait (20, msec);
  }
  chassis.stop(coast);
  stopRollers();
}

void buttonR2Action()
{
  // brake the drivetrain until the button is released.
  chassis.stop(hold);
  controller1.rumble(".");
  waitUntil(!controller1.ButtonR2.pressing());
  chassis.checkStatus();
  chassis.stop(coast);
}

void setupButtonMapping() {
  controller1.ButtonL1.pressed(sampleButtonL1Action);
  controller1.ButtonL2.pressed(buttonL2Action);
  controller1.ButtonR1.pressed(buttonR1Action);
  controller1.ButtonR2.pressed(buttonR2Action);
}



// ------------------------------------------------------------------------
//               Only change code below this line when necessary
// ------------------------------------------------------------------------
  
FILE* serialPort = nullptr;
void pollCommandMessages()
{
  if (serialPort == nullptr) {
    serialPort = fopen("/dev/serial1", "r+");
    if (serialPort == nullptr) return;
  }

  char buffer[256];
  if (fgets(buffer, sizeof(buffer), serialPort) != nullptr) {
    std::string line(buffer);
    // Skip empty lines or lines with only whitespace
    if (line.find_first_not_of(" \t\r\n") != std::string::npos) {
      // Remove leading/trailing whitespace and newlines
      std::string trimmedCommand = line;
      trimmedCommand.erase(0, trimmedCommand.find_first_not_of(" \t\r\n"));
      trimmedCommand.erase(trimmedCommand.find_last_not_of(" \t\r\n") + 1);
      
      if (trimmedCommand.empty()) {
          return;
      }
      
      controller(primary).rumble(".");
      printControllerScreen(trimmedCommand.c_str());
        
      // Find the first space to separate command from parameters
      size_t spacePos = trimmedCommand.find(' ');
      // Extract command (first word)
      std::string cmd = trimmedCommand.substr(0, spacePos);
      // Extract parameters (everything after the first space)
      std::string params = trimmedCommand.substr(spacePos + 1);
      
      if (cmd == "drive") {
          // Format: drive <distance>
          float distance = atof(params.c_str());
          chassis.driveDistance(distance);
      }
      else if (cmd == "turn") {
          // Format: turn <heading>
          float heading = atof(params.c_str());
          chassis.turnToHeading(heading);
      }
      else if (cmd == "set_heading") {
          // Format: set_heading <heading>
          float heading = atof(params.c_str());
          chassis.setHeading(heading);
      }
      chassis.stop(coast);
    }
  }
}

int main() {
  // Register the autonomous and driver control functions.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  //comment out the following line to disable auton testing
  registerAutonTestButtons();

  // Set up other button mapping for the controller
  setupButtonMapping();

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    // comment out the following line to disable remote command processing
    pollCommandMessages();
    wait(200, msec);
  }
}
