#include "vex.h"
int currentAutonSelection = 1;        // Current auton selection
int autonTestStep = 0;                // Current step in auton
int autonNum;                         // Total number of autons, automatically calculated based on the size of the autonMenuText array
bool autonTestMode = false;           // Indicates if in test mode

// The first autonomous routine.
void auton1() {
  chassis.driveWithVoltage(3, 3);
  wait(1000, msec);
  chassis.stop(brake);
}

// The second autonomous routine.
void auton2() {
//  awp(); return;
  chassis.setHeading(0);
  chassis.driveDistance(12, 6);
  chassis.turnToHeading(90, 6);
  chassis.turnToHeading(0, 6);
  chassis.driveDistance(-12, 6);
}

// A long autonomous routine, e.g. skill.
// This routine is broken into steps to allow for testing of individual steps.
// This allows for easier debugging of individual parts of the long autonomous routine.
void auton_skill() {
  if (autonTestStep == 0) 
  {
    chassis.turnToHeading(180);
    if (!continueAutonStep()) return; // If in test mode, stop here for testing.
  }
  if (autonTestStep == 1)  
  {
    chassis.driveDistance(5);
    chassis.turnToHeading(chassis.getHeading() + 90); // Turn right
    if (!continueAutonStep()) return; // If in test mode, stop here for testing.
  } 
  if(autonTestStep == 2) 
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



// ----------------------------------------------------------------------------
//                 Only change code below this line when necessary
// ----------------------------------------------------------------------------
// This variable is used to exit the autonomous menu.
bool exitAutonMenu = false;
// This is the autonomous function.
// It is called when the autonomous period starts.
void autonomous(void) {
  // Exits the autonomous menu.
  exitAutonMenu = true;
  // Runs the selected autonomous routine.
  runAutonItem();
}

// Displays the autonomous routine selection menu on the brain screen.
void showAutonMenu() {
  // Calculate the number of autons based on the size of the autonMenuText array.
  autonNum = sizeof(autonMenuText) / sizeof(autonMenuText[0]);
  printMenu(autonMenuText);
  autonTestStep = 0;
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
    return true;
  }
  return false;
}

bool nextAutonMenu()
{
  if (!autonTestMode) return false;

  // if in test mode, scroll through the auton menu
  controller(primary).rumble(".");
  // Reset the auton test step to 0 and change the current auton selection.
  currentAutonSelection = (currentAutonSelection + 1) % autonNum;
  showAutonMenu();
  return true;
}

bool prevAutonMenu()
{
  if (!autonTestMode) return false;

  // if in test mode, scroll through the auton menu
  controller(primary).rumble(".");
  // Reset the auton test step to 0 and change the current auton selection.
  currentAutonSelection = (currentAutonSelection - 1 + autonNum) % autonNum;
  autonTestStep = 0;
  showAutonMenu();
  return true;
}

bool prevAutonStep(){
  if (!autonTestMode) return false;

  // If in test mode, go to the previous step.
  controller(primary).rumble(".");
  if (autonTestStep > 0) autonTestStep--;
  controller(primary).Screen.print("Step: %d         ", autonTestStep);
  return true;
}

bool nextAutonStep()
{
  if (!autonTestMode) return false;

  // If in test mode, go to the next step.
  controller(primary).rumble(".");
  autonTestStep++;
  controller(primary).Screen.print("Step: %d           ", autonTestStep);
  return true;
}

bool continueAutonStep()
{
  autonTestStep++;
  if (autonTestMode) return false; // If in test mode, stop here for testing.
  return true; 
}

bool runAutonTest()
{
  if (!autonTestMode) return false;

  // If in test mode, run the selected autonomous routine for testing and displays the run time.
  controller(primary).rumble(".");
  double t1 = Brain.Timer.time(sec);
  runAutonItem(); 
  double t2 = Brain.Timer.time(sec);
  char timeMsg[30];
  sprintf(timeMsg, "run time: %.0f", t2-t1);
  printControllerScreen(timeMsg);
  chassis.stop(coast);
  return true;
}

// end game reminder will start at 85 seconds into the match
const int END_GAME_SECONDS = 85;
// This function is a thread that runs in the background to remind the driver of the end game.
void endgameTimer() {
  // Waits until the end game starts.
  while (Brain.Timer.time(sec) < END_GAME_SECONDS) {
    wait(500, msec);
  }
  // Prints a message to the controller screen.
  printControllerScreen("end game");
  // Rumbles the controller.
  controller(primary).rumble("-");

  // Checks the motors every 60 seconds if it's in practice mode.
  while(true)
  {
    wait(60, seconds);
    checkMotors(NUMBER_OF_MOTORS);
  }
}

void exitAuton()
{
  exitAutonMenu = true;
  chassis.joystickTouched = false;
    // Clears the brain timer.
  Brain.Timer.clear();
    // Starts the end game timer thread.
  thread endgameTimer_thread = thread(endgameTimer);
  if (!chassis.joystickTouched) {
    //TODO: some macto actions
  }
  chassis.stop(coast);
}

bool setupgyro() {
  // Waits until the inertial sensor is calibrated.
  while (chassis.gyro.isCalibrating()) {
    wait(25, msec);
  }
  // Rumbles the controller to indicate that the gyro is calibrated.
  controller(primary).rumble("-");
  // If the inertial sensor is not installed, print an error message to the controller screen.
  if (!chassis.gyro.installed()) {
    printControllerScreen("inertial sensor failure");
    controller(primary).rumble("---");
    wait(2, seconds);
    return false;  
  }
  return true;
}

// optical sensor for team color detection
optical teamOptical = optical(PORT8);
bool teamIsRed = true;
void setupTeamColor(){
  if (teamOptical.installed()) {
    // Sets the team color based on the optical sensor.
    if (teamOptical.color() == color::blue) {
      teamIsRed = false;
      printControllerScreen("team blue");
    } else {
      printControllerScreen("team red");
    }
    wait(1, seconds);
  } 
}

// This function is called before the autonomous period starts.
void pre_auton() {
  // Sets up the gyro.
  bool gyroSetupSuccess = setupgyro();
  // Sets up the team color.
  setupTeamColor();

  bool motorsSetupSuccess = true;
  // Checks the motors.
  motorsSetupSuccess = checkMotors(NUMBER_OF_MOTORS);
  //set the parameters for the chassis
  setChassisDefaults();
  // Shows the autonomous menu.
  if(gyroSetupSuccess && motorsSetupSuccess) showAutonMenu();
}
