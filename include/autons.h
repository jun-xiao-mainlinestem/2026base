#pragma once
#include "rgb-template/drive.h"
#include "test.h"

// The currently selected autonomous routine.
extern int currentAutonSelection;
extern int autonTestStep;
extern bool autonTestMode;

// Displays the autonomous routine selection menu on the brain screen.
void showAutonMenu();
// Runs the selected autonomous routine.
void runAutonItem();
// Prints the autonomous selection menu to the brain screen.
void printMenu(char const * txt[]);

extern bool exitAutonMenu;

// The autonomous task.
void autonomous();

bool enterTestMode();
bool prevAutonMenu();
bool nextAutonMenu();
bool runAutonTest();
bool prevAutonStep();
bool nextAutonStep();
bool continueAutonStep();