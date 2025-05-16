#include "vex.h"

using namespace vex;
competition Competition;

void turn_north() {
  chassis.driver_control_disabled = true;
  chassis.turn_to_heading(0);
  chassis.driver_control_disabled = false;
}

void button_L1_action() {
  intake.spin(forward, 12, volt);
  while(controller(primary).ButtonL1.pressing()) {
    wait (20, msec);
  }
  intake.stop(coast);
}


void buttonR2_action() {
  chassis.stop(hold);
  controller(primary).rumble(".");
  waitUntil(!controller(primary).ButtonR2.pressing());
  chassis.stop(coast);
}

void button_right_action()
{
  // activate test mode
  if ((Brain.Timer.time(sec) < 5)) {
    auton_test_mode = true;
  }
}

void buttonA_action()
{
  // run auton test
  if (auton_test_mode)
  {
    run_auton_test();
    return;
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  controller(primary).ButtonRight.pressed(button_right_action);
  controller(primary).ButtonA.pressed(buttonA_action);

  controller(primary).ButtonR2.pressed(buttonR2_action);
  controller(primary).ButtonUp.pressed(turn_north);
  controller(primary).ButtonL1.pressed(button_L1_action);
  controller(primary).ButtonR1.pressed(toggle_clamp);

  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}