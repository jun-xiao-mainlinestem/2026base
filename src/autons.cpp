#include "vex.h"

// The first autonomous routine.
void auton1() {
  chassis.driveWithVoltage(3, 3);
  wait(1000, msec);
  chassis.stop(brake);
}

// The second autonomous routine.
void auton2() {
  awp(); return;
  chassis.setHeading(0);
  chassis.driveDistance(12, 6);
}

// The long autonomous skill routine.
// This routine is broken into steps to allow for testing.
// Each step can be run independently based on gloabl variable autonTestStep.
// This allows for easier debugging and testing of individual parts of the autonomous routine.
void auton_skill() {
  if (autonTestStep == 1) 
  {
    chassis.turnToHeading(180);
    if (!continueAutonStep()) return; // If in test mode, stop here for testing.
  }
  if (autonTestStep == 2)  
  {
    chassis.driveDistance(5);
    chassis.turnToHeading(chassis.getHeading() + 90); // Turn right
    if (!continueAutonStep()) return; // If in test mode, stop here for testing.
  } 
  if(autonTestStep == 3) 
  {
    chassis.turnToHeading(chassis.getHeading() - 90); // Turn left
    chassis.driveDistance(-5);
  }
}

// Runs the selected autonomous routine.
void runAutonItem() {
  switch (currentAutonSelection) {
  case 0:
    auton1();
    break;
  case 1:
    auton2();
    break;
  case 2:
    auton_skill();
    break;
  }
}

// The names of the autonomous routines to be displayed in the menu.
char const * autonMenuText[] = {
  "auton1",
  "auton2",
  "auton_skill"
};

// The default autonomous routine selection.
int currentAutonSelection = 1;
// The current step in the autonomous test mode.
int autonTestStep = 1;
// When true, the autonomous routine will stop at each step for testing.
bool autonTestMode = false;


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

bool enterTestMode()
{
  // Activate test mode if the button is pressed immediately after running the program
  if ((Brain.Timer.time(sec) < 5) && !autonTestMode) {
    controller(primary).rumble("-");
    printControllerScreen("Test Mode: ON");
    wait(1, sec);
    showAutonMenu();
    autonTestMode = true;
    autonTestStep = 1;
    return true;
  }
  return false;
}

bool nextAutonMenu()
{
  // if in test mode, scroll through the auton menu
  if (autonTestMode)
  {
    controller(primary).rumble(".");
    // Reset the auton test step to 1 and change the current auton selection.
    currentAutonSelection = (currentAutonSelection + 1) % autonNum;
    autonTestStep = 1;
    showAutonMenu();
    return true;
  }
  return false;
}

bool prevAutonMenu()
{
  // if in test mode, scroll through the auton menu
  if (autonTestMode)
  {
    controller(primary).rumble(".");
    // Reset the auton test step to 1 and change the current auton selection.
    currentAutonSelection = (currentAutonSelection - 1 + autonNum) % autonNum;
    autonTestStep = 1;
    showAutonMenu();
    return true;
  }
  return false;
}

bool prevAutonStep(){
  // If in test mode, go to the previous step.
  if (autonTestMode) {
    controller(primary).rumble(".");
    if (autonTestStep > 1) autonTestStep--;
    controller(primary).Screen.print("Step: %d         ", autonTestStep);
    return true;
  }
  return false;
}

bool nextAutonStep()
{
  // If in test mode, go to the next step.
  if (autonTestMode) {
    controller(primary).rumble(".");
    autonTestStep++;
    controller(primary).Screen.print("Step: %d           ", autonTestStep);
    return true;
  }
  return false; 
}

bool continueAutonStep()
{
  autonTestStep++;
  if (autonTestMode) {
    return false;
  }
  return true; 
}

bool runAutonTest()
{
  // If in test mode, run the selected autonomous routine for testing and displays the run time.
  if (autonTestMode)
  {
    controller(primary).rumble(".");

    double t1 = Brain.Timer.time(sec);
    chassis.driverControlDisabled = true;

    runAutonItem(); 

    double t2 = Brain.Timer.time(sec);
    char timeMsg[30];
    sprintf(timeMsg, "run time: %.0f", t2-t1);
    printControllerScreen(timeMsg);
    chassis.driverControlDisabled = false;
    chassis.stop(coast);

    return true;
  }
  return false;
}


// This is the autonomous function.
// It is called when the autonomous period starts.
void autonomous(void) {
  // Exits the autonomous menu.
  exitAutonMenu = true;
  // Runs the selected autonomous routine.
  runAutonItem();
}

