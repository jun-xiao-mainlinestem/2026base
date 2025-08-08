#pragma once
#include "rgb-template/drive.h"
#include "test.h"
extern bool teamIsRed;

// Displays the autonomous routine selection menu on the brain screen.
void showAutonMenu();
// Runs the selected autonomous routine.
void runAutonItem();

void pre_auton();
void autonomous();
void exitAuton();

bool enterTestMode();
bool prevAutonMenu();
bool nextAutonMenu();
bool runAutonTest();
bool prevAutonStep();
bool nextAutonStep();
bool continueAutonStep();