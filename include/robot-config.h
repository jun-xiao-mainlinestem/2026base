using namespace vex;

extern motor rollerBottom;
extern motor rollerMiddle;
extern motor rollerTop;

// optical sensor for team color detection
extern optical teamOptical;
extern bool teamIsRed;
// distance sensor at the front
extern distance frontDistance;

bool toggleMatchLoad();
bool toggleHorn();
void intake();
void outTake();
void stopRollers();
void scoreLong();


// ------------------------------------------------------------------------
//               Only change code below this line when necessary
// ------------------------------------------------------------------------

// A global instance of the brain used for printing to the V5 Brain screen.
extern brain Brain;
extern controller controller1;
// Forward declaration of the Drive class.
class Drive;
// A global instance of the Drive class.
extern Drive chassis;
extern int DRIVE_MODE;

extern const int NUMBER_OF_MOTORS;

void changeDriveMode();
void setChassisDefaults();
void usercontrol();
