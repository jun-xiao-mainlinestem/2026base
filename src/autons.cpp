#include "vex.h"
// set the default auton selection
int current_auton_selection = 0;

// set team color
bool is_red_team = true;

// put all tuning parameters for the autons below
int auton1_parameters[] = {12, 20};

char const * auton_menu_text[] = {
  "auton1",
  "auton2"
};

int auton_num = 1;
void show_auton_menu() {
  auton_num = sizeof(auton_menu_text) / sizeof(auton_menu_text[0]);
  print_menu(auton_menu_text);
}

//stop at check point when this flag is set to true
bool auton_test_mode = false;

void test1() {
  int x = auton1_parameters[0];
  int y = auton1_parameters[1];

  chassis.drive_distance(x, 10, 0, 6, true);
  if (auton_test_mode && !controller(primary).ButtonA.pressing() ) return;
  chassis.turn_to_heading(90, 10, true);
  chassis.drive_distance(y, 10, 90, 6, true);
  chassis.turn_to_heading(180, 10, true);
  chassis.drive_distance(x, 10, 180, 6, true);
  chassis.turn_to_heading(270, 10, true);
  chassis.drive_distance(y, 10, 270, 6, true);
  chassis.turn_to_heading(0);
}

void test2() {
  chassis.drive_with_voltage(3, 3);
  wait(1000, msec);
  chassis.stop(brake);
}

void run_auton_item() {
  switch (current_auton_selection) {
  case 0:
    test1();
    break;
  case 1:
    test2();
    break;
  }
}

void run_auton_test() {
  chassis.driver_control_disabled = true;
  Brain.Timer.clear();

  //write your code here
  run_auton_item(); 

  double t = Brain.Timer.time(sec);
  controller(primary).Screen.print("run time: %.1f   ", t);
  chassis.driver_control_disabled = false;
}

