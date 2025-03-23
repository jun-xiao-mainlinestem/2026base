#include "vex.h"

using namespace vex;
competition Competition;

void turn_north() {
  chassis.driver_control_disabled = true;
  chassis.turn_to_heading(0);
  chassis.driver_control_disabled = false;
}

void turn_left_90() {
 // chassis.driver_control_disabled = true;
  float h = chassis.get_heading();
  chassis.turn_to_heading(h - 90);
//  chassis.driver_control_disabled = false;  
}

void turn_right_90() {
  chassis.driver_control_disabled = true;
  float h = chassis.get_heading();
  chassis.turn_to_heading(h + 90);
  chassis.driver_control_disabled = false;  
}

void hold_drive_train() {
  chassis.stop(hold);
  controller(primary).rumble(".");
  waitUntil(!controller(primary).ButtonR2.pressing());
  chassis.stop(coast);
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  controller(primary).ButtonY.pressed(button_y_test);
 // controller(primary).ButtonA.pressed(set_drive_mode);
  controller(primary).ButtonR2.pressed(hold_drive_train);
  controller(primary).ButtonUp.pressed(turn_north);
  controller(primary).ButtonLeft.pressed(turn_left_90);
  controller(primary).ButtonRight.pressed(turn_right_90);



  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}