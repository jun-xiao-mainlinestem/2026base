#include "vex.h"

using namespace vex;
competition Competition;

//
// Main will set up the competition functions and callbacks.
//
int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  controller(primary).ButtonY.pressed(button_y_test);
  controller(primary).ButtonA.pressed(set_drive_mode);

  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}