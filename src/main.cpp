#include "vex.h"

using namespace vex;
competition Competition;

// intake control
void buttonL1_action() {
  intake.spin(forward, 12, volt);
  while(controller(primary).ButtonL1.pressing()) {
    wait (20, msec);
  }
  intake.stop(coast);
}

// hold drivetrain
void buttonR2_action() {
  chassis.stop(hold);
  controller(primary).rumble(".");
  waitUntil(!controller(primary).ButtonR2.pressing());
  chassis.stop(coast);
}

void buttonRight_action()
{
  // activate test mode if the butotn is pressed immediately afer running the program
  if ((Brain.Timer.time(sec) < 5) && !auton_test_mode) {
    controller(primary).rumble("-");
    auton_test_mode = true;
    return;
  }
  // if in test mode, scroll through the auton menu
  if (auton_test_mode)
  {
    current_auton_selection = (current_auton_selection + 1) % auton_num;
    show_auton_menu();
    return;
  }

  // macro function below
  chassis.driver_control_disabled = true;
  // insert code 
  chassis.driver_control_disabled = false;
}

void buttonB_action()
{
  // toggle team color if the button is pressed when intaking is running
  if (intake.isSpinning()) {
    controller(primary).rumble("-");
    is_red_team = !is_red_team;
    if (is_red_team)  controller(primary).Screen.print("red      ");
    else  controller(primary).Screen.print("blue      ");
    return;
  }

  // macro function below
  chassis.driver_control_disabled = true;
  // insert code 
  chassis.driver_control_disabled = false;
}

void buttonA_action()
{
  // run auton test if in test mode
  if (auton_test_mode)
  {
    run_auton_test();
    return;
  }

  // change drive mode
  if (Brain.Timer.time(sec) < 5)
  {
    controller(primary).rumble("-");
    DRIVE_TANK_MODE = !DRIVE_TANK_MODE;  
    if (DRIVE_TANK_MODE)  controller(primary).Screen.print("tank      ");
    else  controller(primary).Screen.print("double aracde      ");  
    return;
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  controller(primary).ButtonRight.pressed(buttonRight_action);
  controller(primary).ButtonA.pressed(buttonA_action);
  controller(primary).ButtonB.pressed(buttonB_action);

  controller(primary).ButtonR2.pressed(buttonR2_action);
  controller(primary).ButtonL1.pressed(buttonL1_action);
  controller(primary).ButtonR1.pressed(toggle_clamp);

  pre_auton();

  // additional setup for other subsystems of the robot
  clampRotation.setPosition(clampRotation.angle(deg), degrees);
  mogo_distance.changed(detect_mogo);

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}