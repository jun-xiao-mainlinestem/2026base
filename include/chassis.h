using namespace vex;
extern brain Brain;

class Drive;
extern Drive chassis;
extern bool DRIVE_TANK_MODE;

void usercontrol();
void pre_auton();
void print_menu(char const * txt[]);
void autonomous();
