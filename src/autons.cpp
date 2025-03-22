#include "vex.h"
int current_auton_selection = 0;
int auton_num = 1;
bool wait_for_auton = true;

char const * auton_test_menu[] = {
    "auton1",
    "auton2"
};

void run_auton_item() {
    int id = current_auton_selection % (auton_num / 2);
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

void autonomous(void) {
    wait_for_auton = false;
    // run auton
    run_auton_item();
}

void print_menu_item(char const * txt[], char const * title) {
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print("%s", title);
  Brain.Screen.setCursor(3, 1);
  Brain.Screen.print("%s", txt[current_auton_selection]);
  controller(primary).Screen.print("%s %s     ", txt[current_auton_selection], title);
}

void print_menu(char const * txt[], char const * title) {
  auton_num = sizeof(txt) / sizeof(txt[0]);
  Brain.Screen.setFont(mono30);
  print_menu_item(txt, title);

  while (wait_for_auton) {
    if (Brain.Screen.pressing()) {
      while (Brain.Screen.pressing()) {
        //wait until finger lifted up from the screen
        wait(20, msec);
      }
      current_auton_selection = (current_auton_selection + 1) % auton_num;
      print_menu_item(txt, title);
      controller(primary).rumble(".");
    }
    task::sleep(50);
  }
  Brain.Screen.setFont(mono20);
}

void button_y_test() {
    Brain.Timer.clear();
      // write your test code here

    double t = Brain.Timer.time(sec);
    controller(primary).Screen.print("run time: %.1f       ", t);
}