#include "vex.h"

using namespace vex;
competition Competition;

void hold_drive_train() {
  chassis.stop_drivetrain(true);
  controller(primary).rumble(".");
  waitUntil(!controller(primary).ButtonR2.pressing());
  chassis.stop_drivetrain(false);
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  controller(primary).ButtonY.pressed(button_y_test);
  controller(primary).ButtonA.pressed(set_drive_mode);
  controller(primary).ButtonR2.pressed(hold_drive_train);

  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}