using namespace vex;

extern motor rollerBottom;
extern motor rollerMiddle;
extern motor rollerTop;

void inTake();
void outTake();
void stopRollers();
void scoreMiddle();
void scoreLong();

void colorSort();

// ------------------------------------------------------------------------
//               Only change code below this line when necessary
// ------------------------------------------------------------------------

// A global instance of the brain used for printing to the V5 Brain screen.
extern brain Brain;
extern const int NUMBER_OF_MOTORS;

// Forward declaration of the Drive class.
class Drive;
// A global instance of the Drive class.
extern Drive chassis;
extern inertial inertial1;
// A flag to switch between tank and arcade drive modes.
extern int DRIVE_MODE;

void setChassisDefaults();
void usercontrol();
bool changeDriveMode();