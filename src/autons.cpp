#include "vex.h"
int current_auton_selection = 0;

char const * auton_test_menu[] = {
    "auton1",
    "auton2"
};

void run_auton_item(int id) {
    switch (id) {
    case 0:
        test1();
        break;
    case 1:
        test2();
        break;
    }
}

void test1()
{

}

void test2()
{

}

void show_auton_menu()
{
    print_menu(auton_test_menu, "test");
}

void button_y_test() {
    Brain.Timer.clear();
    chassis.driver_control_disabled = true;
      // write your test code here

    chassis.driver_control_disabled = false;
    double t = Brain.Timer.time(sec);
    controller(primary).Screen.print("run time: %.1f       ", t);
}