#include "vex.h"
#include "test.h"
int currentAutonSelection = -1;        // Current auton selection
int autonTestStep = 0;                // Current step in auton

void quick_test() {
  //aiAction();
  //awp();
}

// The first autonomous routine.
void sampleAuton1() {
  chassis.driveWithVoltage(3, 3);
  wait(1000, msec);
  chassis.stop(brake);
}

// The second autonomous routine.
void sampleAuton2() {
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
void sampleSkill() {
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
    sampleAuton1();
    break;
  case 1:
    sampleAuton2();
    break;
  case 2:
    sampleSkill();
    break;
  case -1:
    quick_test();
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
int autonNum;                         // Total number of autons, automatically calculated based on the size of the autonMenuText array
bool autonTestMode = false;           // Indicates if in test mode
bool exitAutonMenu = false;           // Flag to exit the autonomous menu
bool enableEndGameTimer = false;      // Flag to indicate if endgame timer is enabled 
const int END_GAME_SECONDS = 85;      // Endgame reminder starts at 85 seconds

// The function is called when the autonomous period starts.
void autonomous(void) {
  // Exits the autonomous menu.
  exitAutonMenu = true;
  enableEndGameTimer = true;
  // Runs the selected autonomous routine.
  runAutonItem();
}

// This function prints the selected autonomous routine to the brain and controller screens.
void printMenuItem() {
  if (currentAutonSelection < 0) {
    printControllerScreen("quick test");
    return;
  }    
  // Clears the brain screen.
  Brain.Screen.clearScreen();
  // Sets the cursor to the third row, first column.
  Brain.Screen.setCursor(3, 1);
  // Prints the selected autonomous routine name.
  Brain.Screen.print("%s", autonMenuText[currentAutonSelection]);
  printControllerScreen(autonMenuText[currentAutonSelection]);
}

// This function displays the autonomous menu on the brain screen.
void showAutonMenu() {
  autonNum = sizeof(autonMenuText) / sizeof(autonMenuText[0]);
  autonTestStep = 0;

  Brain.Screen.setFont(mono30);
  printMenuItem();

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
      printMenuItem();
      controller(primary).rumble(".");
    }
    // This wait prevents the loop from using too much CPU time.
    wait(50, msec);
  }
  Brain.Screen.setFont(mono20);
}

// This function is a thread that runs in the background to remind the driver of the end game.
void endgameTimer() {
  // Waits until the end game starts.
  while (Brain.Timer.time(sec) < END_GAME_SECONDS) {
    wait(500, msec);
  }
  printControllerScreen("end game");
  controller(primary).rumble("-");

  // Checks the motors health every 60 seconds in drive practice
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
  if (enableEndGameTimer) thread endgameTimer_thread = thread(endgameTimer);
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



// ----------------------------------------------------------------------------
//                 For testing autonomous steps
// ----------------------------------------------------------------------------

bool continueAutonStep()
{
  autonTestStep++;
  if (autonTestMode) return false; // If in test mode, stop here for testing.
  return true; 
}

// This function is called when the Right button is pressed.
void buttonRightAction()
{
  if ((Brain.Timer.time(sec) < 5) && !autonTestMode) {  
    // If the button is pressed within 5 seconds of starting the program, enter test mode.
    controller(primary).rumble("-");
    printControllerScreen("Test Mode: ON");
    wait(1, sec);
    showAutonMenu();
    autonTestMode = true;
    return;
  } 
  if (autonTestMode)
  {
    controller(primary).rumble(".");
    // if in test mode, scroll through the auton menu
    currentAutonSelection = (currentAutonSelection + 1) % autonNum;
    showAutonMenu();
  }
}

void buttonLeftAction()
{
  if ((Brain.Timer.time(sec) < 5)) {
    // If the button is pressed within 5 seconds of starting the program, change the drive mode.
    changeDriveMode();
    return;
  }
  if (autonTestMode) 
  {
    // if in test mode, scroll through the auton menu
    controller(primary).rumble(".");
    currentAutonSelection = (currentAutonSelection - 1 + autonNum) % autonNum;
    showAutonMenu();
    return;
  }
}

bool macroMode = false;

void buttonDownAction()
{
  if (autonTestMode) 
  {
    // If in test mode, go to the next step.
    controller(primary).rumble(".");
    autonTestStep++;
    controller(primary).Screen.print("Step: %d           ", autonTestStep);
  }

  if (macroMode) return; // prevent re-entry
  if (fabs(chassis.getHeading()) - 180 > 3) {
    chassis.driveWithVoltage(-6, -6);
    wait(100, msec);
    chassis.turnToHeading(180);
  }
  if (!controller(primary).ButtonDown.pressing()) return;
  
  macroMode = true;
  // This is a placeholder for future actions triggered by Button Down.
  // safty check to prevent running the code if the distance reading is not valid.
  // Matchload balls when the Button Down is pressed and hold.
  // end matchloading and turn around to score when the Button Down is released.
  macroMode = false;
  chassis.stop(coast);
}

void buttonUpAction()
{
  if (autonTestMode) 
  {
    // If in test mode, go to the previous step.
    controller(primary).rumble(".");
    if (autonTestStep > 0) autonTestStep--;
    controller(primary).Screen.print("Step: %d         ", autonTestStep);
  }
}

// Register the controller button callbacks for autonomous testing.
void registerAutonTestButtons()
{
  controller(primary).ButtonRight.pressed(buttonRightAction);
  controller(primary).ButtonLeft.pressed(buttonLeftAction);
  controller(primary).ButtonDown.pressed(buttonDownAction);
  controller(primary).ButtonUp.pressed(buttonUpAction);
}