#pragma once
#include "rgb-template/drive.h"
extern int current_auton_selection;
extern int auton_num;
extern bool auton_test_mode;
extern bool is_red_team;

void show_auton_menu();
void run_auton_test();
void run_auton_item();