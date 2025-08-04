#include "vex.h"

// The first autonomous routine.
void auton1() {
  chassis.driveWithVoltage(3, 3);
  wait(1000, msec);
  chassis.stop(brake);
}

// The second autonomous routine.
// This routine is broken into steps to allow for testing.
// Each step can be run independently by passing the step number to the function.
// This allows for easier debugging and testing of individual parts of the autonomous routine.
void auton2(int step) {
  if (step == 1) 
  {
    chassis.turnToHeading(180);
    step++;
    if (autonTestMode) return; // If in test mode, stop here for testing.
  }
  if (step == 2)  
  {
    chassis.driveDistance(12);
    chassis.turnToHeading(chassis.getHeading() + 90); // Turn right
    step++;
    if (autonTestMode) return; // If in test mode, stop here for testing.
  } 
  if(step == 3) 
  {
    chassis.turnToHeading(chassis.getHeading() - 90); // Turn left
    chassis.driveDistance(-12);
  }
}

// Runs the selected autonomous routine.
void runAutonItem(int step=1) {
  switch (currentAutonSelection) {
  case 0:
    auton1();
    break;
  case 1:
    auton2(step);
    break;
  }
}

// The names of the autonomous routines to be displayed in the menu.
char const * autonMenuText[] = {
  "auton1",
  "auton2"
};

// The default autonomous routine selection.
int currentAutonSelection = 1;

// ----------------------------------------------------------------------------
//                     No need to change code below this line
// ----------------------------------------------------------------------------



// The total number of autonomous routines. This is calculated in showAutonMenu().
int autonNum;

// Displays the autonomous routine selection menu on the brain screen.
void showAutonMenu() {
  // Calculate the number of autons based on the size of the autonMenuText array.
  autonNum = sizeof(autonMenuText) / sizeof(autonMenuText[0]);
  printMenu(autonMenuText);
}

// This function prints the selected autonomous routine to the brain and controller screens.
void printMenuItem(char const * txt[]) {
  // If the custom test is selected, print "custom test" to the controller screen.
  if (currentAutonSelection < 0) {
    printControllerScreen("custom test");
    return;
  }    
  // Clears the brain screen.
  Brain.Screen.clearScreen();
  // Sets the cursor to the third row, first column.
  Brain.Screen.setCursor(3, 1);
  // Prints the selected autonomous routine to the brain screen.
  Brain.Screen.print("%s", txt[currentAutonSelection]);
  // Prints the selected autonomous routine to the controller screen.
  printControllerScreen(txt[currentAutonSelection]);
}

// This variable is used to exit the autonomous menu.
bool exitAutonMenu = false;

// When true, the autonomous routine will stop at each step for testing.
bool autonTestMode = false;

// This function displays the autonomous menu on the brain screen.
void printMenu(char const * txt[]) {
  // Sets the font to mono30.
  Brain.Screen.setFont(mono30);
  // Prints the selected autonomous routine.
  printMenuItem(txt);

  // This loop runs until the autonomous menu is exited.
  while (!exitAutonMenu) {
    // If the brain screen is pressed, cycle through the autonomous routines.
    if (Brain.Screen.pressing()) {
      // Waits until the finger is lifted up from the screen.
      while (Brain.Screen.pressing()) {
        wait(20, msec);
      }
      // Cycles through the autonomous routines.
      currentAutonSelection = (currentAutonSelection + 1) % autonNum;
      // Prints the selected autonomous routine.
      printMenuItem(txt);
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
  exitAutonMenu = true;
  // Runs the selected autonomous routine.
  runAutonItem();
}

