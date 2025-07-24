// vex.h includes all of the headers for the VEX V5 library
#include "vex.h"

// All vex classes and functions are in the vex namespace
using namespace vex;

// A global instance of competition
// This object is used to register callbacks for the autonomous and driver control periods.
competition Competition;

// ----------------------------------------------------------------------------
//                                Controller Callbacks
// ----------------------------------------------------------------------------

// This function is called when the L1 button is pressed.
// It spins the intake motor forward while the button is held down.
void buttonL1_action() {
  in_take();
  // Wait until the button is released to stop the intake.
  while(controller(primary).ButtonL1.pressing()) {
    wait (20, msec);
  }
  stop_rollers();
}

void buttonL2_action() {
  out_take();
  // Wait until the button is released to stop the intake.
  while(controller(primary).ButtonL2.pressing()) {
    wait (20, msec);
  }
  stop_rollers();
}

void buttonR1_action() {
  chassis.stop(hold);
  score_long();
  // Wait until the button is released to stop the intake.
  while(controller(primary).ButtonR1.pressing()) {
    wait (20, msec);
  }
  chassis.stop(coast);
  stop_rollers();
}

// This function is called when the R2 button is pressed.
void buttonR2_action() {
  chassis.stop(hold);
  score_middle();
  while(controller(primary).ButtonR2.pressing()) {
    wait (20, msec);
  }  
  chassis.stop(coast);
  stop_rollers();

}

// This function is called when the B button is pressed.
// It holds the drivetrain in place until the button is released.
void buttonB_action()
{
  double distance_traveled = (chassis.get_left_position_in() + chassis.get_right_position_in()) / 2.0;
  chassis.stop(hold);
  controller(primary).rumble(".");
  wait(0.5, sec);
  // Display the distance traveled on the controller screen.
  controller(primary).Screen.print("distance: %.2f in.    ", distance_traveled);
  waitUntil(!controller(primary).ButtonB.pressing());
  chassis.stop(coast);
}

// This function is called when the X button is pressed.
void buttonX_action()
{
  // run macro function
  chassis.driver_control_disabled = true;
  // TODO: Insert macro code here. This is a placeholder for future macro actions triggered by Button X.

  chassis.driver_control_disabled = false;
}


// Global flag auton_test_mode indicating whether the robot is in autonomous test mode.
// When true, special button actions allow selection and testing of autonomous routines.
// This function is called when the Right button is pressed.
void buttonRight_action()
{
  // activate test mode if the button is pressed immediately after running the program
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
  // otherwise, run ohter macro code
  chassis.driver_control_disabled = true;
  // TODO: Insert test code here. This is a placeholder for future actions triggered by Button Right.

  chassis.driver_control_disabled = false;

}

// This function is called when the Left button is pressed.
void buttonLeft_action()
{
  // if in test mode, scroll through the auton menu
  if (auton_test_mode)
  {
    current_auton_selection = (current_auton_selection - 1) % auton_num;
    show_auton_menu();
    return;
  }

  // otherwise, run ohter macro code
  chassis.driver_control_disabled = true;
  // TODO: Insert test code here. This is a placeholder for future actions triggered by Button Left.

  chassis.driver_control_disabled = false;
}

// This function is called when the A button is pressed.
void buttonA_action()
{
  // If in test mode, run the selected autonomous routine for testing and displays the run time.
  if (auton_test_mode)
  {
    chassis.driver_control_disabled = true;
    Brain.Timer.clear();

    run_auton_item(); 

    double t = Brain.Timer.time(sec);
    controller(primary).Screen.print("run time: %.1f   ", t);
    chassis.driver_control_disabled = false;

    return;
  }

    // otherwise run test code 
  chassis.driver_control_disabled = true;
  // TODO: Insert test code here. This is a placeholder for future test actions triggered by Button A.

  chassis.driver_control_disabled = false;
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

  // Register the controller button callbacks.
  controller(primary).ButtonRight.pressed(buttonRight_action);
  controller(primary).ButtonRight.pressed(buttonLeft_action);
  controller(primary).ButtonA.pressed(buttonA_action);
  controller(primary).ButtonB.pressed(buttonB_action);
  controller(primary).ButtonX.pressed(buttonX_action);

  controller(primary).ButtonL1.pressed(buttonL1_action);
  controller(primary).ButtonL2.pressed(buttonL2_action);
  controller(primary).ButtonR1.pressed(buttonR1_action);
  controller(primary).ButtonR2.pressed(buttonR2_action);


  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
