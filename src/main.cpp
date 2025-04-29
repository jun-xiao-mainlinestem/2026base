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

void toggle_intake() {
  intake.spin(forward, 12, volt);
  while(controller(primary).ButtonL1.pressing()) {
    wait (20, msec);
  }
  intake.stop(coast);
}

void toggle_clamp() {
  if (clamp_is_up == true) {
   clamp_mogo();
   clamp_is_up = false;
  }
  else {
   release_mogo();
   clamp_is_up = true;
  }
  wait (200, msec);
}

void hold_drive_train() {
  chassis.stop(hold);
  controller(primary).rumble(".");
  waitUntil(!controller(primary).ButtonR2.pressing());
  chassis.stop(coast);
}

void button_right_action()
{
  // select test to run
  if ((Brain.Timer.time(sec) < 5) || current_auton_selection > -1) {
    show_auton_menu();
    current_auton_selection = (current_auton_selection + 1) % auton_num;
    return;
  }
}

void button_x_action()
{
  if (current_auton_selection > -1)
  {
    run_test_item();
    return;
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  controller(primary).ButtonR2.pressed(hold_drive_train);
  controller(primary).ButtonUp.pressed(turn_north);
  controller(primary).ButtonLeft.pressed(turn_left_90);
  controller(primary).ButtonRight.pressed(button_right_action);
  controller(primary).ButtonX.pressed(button_x_action);

  controller(primary).ButtonL1.pressed(toggle_intake);
  controller(primary).ButtonR1.pressed(toggle_clamp);

  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}