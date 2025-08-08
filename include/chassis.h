using namespace vex;

// A global instance of the brain used for printing to the V5 Brain screen.
extern brain Brain;

// Forward declaration of the Drive class.
class Drive;
// A global instance of the Drive class.
extern Drive chassis;
extern inertial inertial1;
// A flag to switch between tank and arcade drive modes.
extern int DRIVE_MODE;

void setChassisDefaults();

// The user control task.
void usercontrol();

bool changeDriveMode();