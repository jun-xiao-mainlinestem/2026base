#pragma once
#include "rgb-template/drive.h"
extern bool teamIsRed;

void pre_auton();
void autonomous();
void exitAuton();

bool continueAutonStep();
void registerAutonTestButtons();