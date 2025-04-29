#include "vex.h"

int current_auton_selection = 0;

int auton1_parameters[] = {12, 20};

char const * auton_test_menu[] = {
  "auton1",
  "auton2"
};

void run_auton_item(int id) {
  switch (id) {
  case 1:
    test1();
    break;
  case 2:
    test2();
    break;
  }
}

void test1() {
  int x = auton1_parameters[0];
  int y = auton1_parameters[1];

  chassis.drive_distance(x, 10, 0, 6, true);
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

int auton_num = 1;
void show_auton_menu() {
  auton_num = sizeof(auton_test_menu) / sizeof(auton_test_menu[0]);
  print_menu(auton_test_menu);
}

void run_test_item() {
  chassis.driver_control_disabled = true;

  if (current_auton_selection == 0) {
    Brain.Timer.clear();

    //write your code here
    //test_custom_auton();

    double t = Brain.Timer.time(sec);
    controller(primary).Screen.print("run time: %.1f   ", t);
  } else {
    run_auton_item(current_auton_selection);
  }

  chassis.driver_control_disabled = false;
}