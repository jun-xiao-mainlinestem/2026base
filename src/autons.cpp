#include "vex.h"

// The default autonomous routine selection.
int current_auton_selection = 0;

// The default team color.
bool is_red_team = true;

// An array to hold tuning parameters for the first autonomous routine.
int auton1_parameters[] = {12, 20};

// The names of the autonomous routines to be displayed in the menu.
char const * auton_menu_text[] = {
  "auton1",
  "auton2"
};

// The total number of autonomous routines. This is calculated in show_auton_menu().
int auton_num;

// Displays the autonomous routine selection menu on the brain screen.
void show_auton_menu() {
  // Calculate the number of autons based on the size of the auton_menu_text array.
  auton_num = sizeof(auton_menu_text) / sizeof(auton_menu_text[0]);
  print_menu(auton_menu_text);
}

// When true, the autonomous routine will stop at each step if the 'A' button is hold.
bool auton_test_mode = false;

// The first autonomous routine.
void test1() {
  int x = auton1_parameters[0];
  int y = auton1_parameters[1];

  chassis.drive_distance(x, 10, 0, 6, true);
  // If in test mode and the 'A' button not hold, stop the routine here.
  if (auton_test_mode && !controller(primary).ButtonA.pressing() ) return;

  chassis.turn_to_heading(90, 10, true);
  chassis.drive_distance(y, 10, 90, 6, true);
  chassis.turn_to_heading(180, 10, true);
  chassis.drive_distance(x, 10, 180, 6, true);
  chassis.turn_to_heading(270, 10, true);
  chassis.drive_distance(y, 10, 270, 6, true);
  chassis.turn_to_heading(0);
}

// The second autonomous routine.
void test2() {
  chassis.drive_with_voltage(3, 3);
  wait(1000, msec);
  chassis.stop(brake);
}

// Runs the selected autonomous routine.
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

