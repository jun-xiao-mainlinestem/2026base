using namespace vex;

// A global instance of the brain used for printing to the V5 Brain screen.
extern brain Brain;

// Forward declaration of the Drive class.
class Drive;
// A global instance of the Drive class.
extern Drive chassis;
// A flag to switch between tank and arcade drive modes.
extern bool DRIVE_TANK_MODE;

// The user control task.
void usercontrol();
// The pre-autonomous task.
void pre_auton();
