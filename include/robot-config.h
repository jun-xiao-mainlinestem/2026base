using namespace vex;

extern motor rollerBottom;
extern motor rollerMiddle;
extern motor rollerTop;

// optical sensor for team color detection
extern optical teamOptical;
extern bool teamIsRed;

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
// Forward declaration of the Drive class.
class Drive;
// A global instance of the Drive class.
extern Drive chassis;

extern const int NUMBER_OF_MOTORS;

void changeDriveMode();
void setChassisDefaults();
void usercontrol();
