#include "vex.h"

using namespace vex;
competition Competition;

void turnfor180()
{
  macro_running = true;
  chassis.reset();
    chassis.turn_to_heading(chassis.get_absolute_heading() - 180, 8);
  chassis.reset();
  macro_running = false;

}

void turnback180()
{
    macro_running = true;

  chassis.reset();
    chassis.turn_to_heading(chassis.get_absolute_heading() + 180, 8);
  chassis.reset();
      macro_running = false;

}

void turn_left()
{
      macro_running = true;

  chassis.reset();
    chassis.turn_to_heading(chassis.get_absolute_heading() - 90, 10);
  chassis.reset();
      macro_running = false;
  
}

void turn_right()
{
        macro_running = true;

  chassis.reset();
    chassis.turn_to_heading(chassis.get_absolute_heading() + 90, 10);
  chassis.reset();
        macro_running = false;

}

void drive24()
{
    macro_running = true;
  chassis.reset();

    chassis.drive_distance(24, 10); 
  macro_running = false;
  chassis.reset();

}

void driveback24()
{
    macro_running = true;
    chassis.reset();

    chassis.drive_distance(-24, 10); 
    chassis.reset();
    macro_running = false;
}



void four_rings_left()
{
  int side = 1;

  macro_running = true;
  chassis.reset();   
  release_mobile_goal();
  chassis.set_heading(180);
  // get the mogo
  chassis.drive_distance(-18, 5, side * 210, 2);

  macro_running = false;
  chassis.reset();
}

void four_rings_right()
{
  int side = -1;
  macro_running = true;
  chassis.reset();
  release_mobile_goal();
  chassis.set_heading(180);

  // get the mogo
//  chassis.drive_distance(-18, 5, side * 210, 2);

  macro_running = false;
  chassis.reset();
}

void alliance_left()
{
  int side = 1;
  macro_running = true;
  chassis.reset();
  release_mobile_goal();

  chassis.set_heading(side * 95);
  chassis.drive_distance(-32, 6, side * 120, 3);
  chassis.turn_to_heading(side *210, 8);
  wait(50, msec);  
  chassis.drive_distance(-16, 6);

  macro_running = false;
}

void alliance_right()
{
  int side = -1;
  macro_running = true;
  chassis.reset();
  release_mobile_goal();

  chassis.set_heading(side * 95);
  chassis.drive_distance(-32, 6, side * 120, 3);
  chassis.turn_to_heading(side *210, 8);
  wait(50, msec);  
  chassis.drive_distance(-15, 6);
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  Competition.autonomous(autonomous);

  /* Competition.drivercontrol(usercontrol);

  controller(primary).ButtonUp.pressed(turnfor180);
    controller(primary).ButtonDown.pressed(turnback180);

  controller(primary).ButtonLeft.pressed(alliance_left);
  controller(primary).ButtonRight.pressed(alliance_right);

  controller(primary).ButtonY.pressed(four_rings_left);
  controller(primary).ButtonA.pressed(four_rings_right);

  controller(primary).ButtonX.pressed(drive24);
  controller(primary).ButtonB.pressed(driveback24); */

  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}