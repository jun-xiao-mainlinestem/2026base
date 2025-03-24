#include "vex.h"

int current_auton_selection = 0;

int parameters[] = {12, 20};

char const * auton_test_menu[] = {
  "auton1",
  "auton2"
};

void run_auton_item(int id) {
  switch (id) {
  case 0:
    test1();
    break;
  case 1:
    test2();
    break;
  }
}

void test1() {
  int x = parameters[0];
  int y = parameters[1];
 
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

}

int auton_num = 1;
void show_auton_menu() {
  auton_num = sizeof(auton_test_menu) / sizeof(auton_test_menu[0]);
  print_menu(auton_test_menu, "test");
}

void button_y_test() {
  Brain.Timer.clear();
  chassis.driver_control_disabled = true;
  // write your test code here
  test1();

  chassis.driver_control_disabled = false;
  double t = Brain.Timer.time(sec);
  controller(primary).Screen.print("run time: %.1f       ", t);
}