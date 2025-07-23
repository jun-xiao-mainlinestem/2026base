#include "vex.h"

// This array helps tuning the auton1 routine without changing the code.
// The first element is the distance to drive in inches, and the second element is the distance
// to drive in inches after the turn.
int auton1_parameters[] = {12, 20};

// The first autonomous routine.
void test1() {
  int x = auton1_parameters[0];
  int y = auton1_parameters[1];

  chassis.drive_distance(x, 10, 0, 6, true);

  if (!should_continue_auton_step()) return;

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

// The names of the autonomous routines to be displayed in the menu.
char const * auton_menu_text[] = {
  "auton1",
  "auton2"
};

// The default autonomous routine selection.
int current_auton_selection = 0;

// The team color.
bool is_red_team = true;



// No need to change code below this line

// When true, the autonomous routine will stop at each step if the 'A' button is hold.
bool auton_test_mode = false;
bool should_continue_auton_step() {
  return !(auton_test_mode && !controller(primary).ButtonA.pressing());
}

// The total number of autonomous routines. This is calculated in show_auton_menu().
int auton_num;

// Displays the autonomous routine selection menu on the brain screen.
void show_auton_menu() {
  // Calculate the number of autons based on the size of the auton_menu_text array.
  auton_num = sizeof(auton_menu_text) / sizeof(auton_menu_text[0]);
  print_menu(auton_menu_text);
}

// This function prints the selected autonomous routine to the brain and controller screens.
void print_menu_item(char const * txt[]) {
  // If the custom test is selected, print "custom test" to the controller screen.
  if (current_auton_selection < 0) {
    controller(primary).Screen.print("custom test");
    return;
  }    
  // Clears the brain screen.
  Brain.Screen.clearScreen();
  // Sets the cursor to the third row, first column.
  Brain.Screen.setCursor(3, 1);
  // Prints the selected autonomous routine to the brain screen.
  Brain.Screen.print("%s", txt[current_auton_selection]);
  // Prints the selected autonomous routine to the controller screen.
  controller(primary).Screen.print("%s", txt[current_auton_selection]);
}

// This variable is used to exit the autonomous menu.
bool exit_auton_menu = false;

// This function displays the autonomous menu on the brain screen.
void print_menu(char const * txt[]) {
  // Sets the font to mono30.
  Brain.Screen.setFont(mono30);
  // Prints the selected autonomous routine.
  print_menu_item(txt);

  // This loop runs until the autonomous menu is exited.
  while (!exit_auton_menu) {
    // If the brain screen is pressed, cycle through the autonomous routines.
    if (Brain.Screen.pressing()) {
      // Waits until the finger is lifted up from the screen.
      while (Brain.Screen.pressing()) {
        wait(20, msec);
      }
      // Cycles through the autonomous routines.
      current_auton_selection = (current_auton_selection + 1) % auton_num;
      // Prints the selected autonomous routine.
      print_menu_item(txt);
      // Rumbles the controller.
      controller(primary).rumble(".");
    }
    // This wait prevents the loop from using too much CPU time.
    wait(50, msec);
  }
  // Sets the font to mono20.
  Brain.Screen.setFont(mono20);
}

// This is the autonomous function.
// It is called when the autonomous period starts.
void autonomous(void) {
  // Exits the autonomous menu.
  exit_auton_menu = true;
  // Runs the selected autonomous routine.
  run_auton_item();
}

