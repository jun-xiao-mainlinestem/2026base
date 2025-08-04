#pragma once
#include "rgb-template/drive.h"

// The currently selected autonomous routine.
extern int currentAutonSelection;
// The total number of autonomous routines available.
extern int autonNum;
// A flag to enable autonomous routine testing.
// A flag to indicate if the robot is on the red team.
extern bool isRedTeam;

// Displays the autonomous routine selection menu on the brain screen.
void showAutonMenu();

// Runs the selected autonomous routine.
void runAutonItem(int step);

// Prints the autonomous selection menu to the brain screen.
void printMenu(char const * txt[]);

extern bool exitAutonMenu;

// The autonomous task.
void autonomous();
