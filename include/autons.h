#pragma once
#include "rgb-template/drive.h"

// The currently selected autonomous routine.
extern int current_auton_selection;
// The total number of autonomous routines available.
extern int auton_num;
// A flag to enable autonomous routine testing.
extern bool auton_test_mode;
// A flag to indicate if the robot is on the red team.
extern bool is_red_team;

// Displays the autonomous routine selection menu on the brain screen.
void show_auton_menu();
// Runs the selected autonomous routine for testing purposes.
void run_auton_test();
// Runs the selected autonomous routine.
void run_auton_item();