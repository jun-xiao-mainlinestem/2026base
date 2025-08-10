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
  while(controller(primary).ButtonL1.pressing()) {
    colorSort();
    wait (20, msec);
  }
  stopRollers();
}

bool matchLoaderDown = false;
void buttonL2Action() {
  if(controller(primary).ButtonL1.pressing()) 
  {
    matchLoaderDown = !matchLoaderDown;
    // toggle the matchloader position
    return;
  }
  outTake();
  // Wait until the button is released to stop the rollers.
  while(controller(primary).ButtonL2.pressing()) {
    wait (20, msec);
  }
  stopRollers();
}

void buttonR1Action() {
  chassis.stop(hold);
  if(controller(primary).ButtonR2.pressing()) scoreMiddle();
  else scoreLong();

  // Wait until the button is released to stop the rollers.
  while(controller(primary).ButtonR1.pressing()) {
    wait (20, msec);
  }
  chassis.stop(coast);
  stopRollers();
}

void buttonR2Action()
{
  // brake the drivetrain until the button is released.
  chassis.stop(hold);
  controller(primary).rumble(".");
  waitUntil(!controller(primary).ButtonR2.pressing());
  chassis.checkStatus();
  chassis.stop(coast);
}

void setupButtonMapping() {
  controller(primary).ButtonL1.pressed(sampleButtonL1Action);
  controller(primary).ButtonL2.pressed(buttonL2Action);
  controller(primary).ButtonR1.pressed(buttonR1Action);
  controller(primary).ButtonR2.pressed(buttonR2Action);
}


// ----------------------------------------------------------------------------
//                                Main
// ----------------------------------------------------------------------------
//
// Main will set up the competition functions and callbacks.
//
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
    chassis.pollRemoteCommand();
    wait(200, msec);
  }
}
