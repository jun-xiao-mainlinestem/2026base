using namespace vex;

class Drive;
extern Drive chassis;

extern brain Brain;

extern int drive_mode;
void set_drive_mode();
void usercontrol();

void setup_gyro();
void reset_chassis();

void print_menu(char const * txt[]);
void autonomous();
