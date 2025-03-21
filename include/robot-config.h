using namespace vex;

class Drive;

extern Drive chassis;

extern brain Brain;
extern controller Controller1;

extern motor leftMotor1, leftMotor2, leftMotor3, rightMotor1,  rightMotor2, rightMotor3;

extern motor thresholdMotor; 
extern distance mogo_distance;
extern motor intake;

extern motor slapperMotor;
extern motor flipperMotor;
extern bool flipper_enabled;

extern rotation thresholdSensor;  

extern motor catcherMotor;
extern rotation catcherSensor;  


extern int robot_id;

extern int current_auton_selection;
extern int team_color;
extern bool auton_started;

extern bool threshold_is_down;
extern bool threshold_is_running;

extern int drive_mode;
extern bool macro_running;

extern bool auto_threshold;

extern int RELEASE_ANGLE;
extern int threshold_ANGLE;
extern int STOP_ANGLE;

extern const double MAX_INTAKE_CURRENT;

void usercontrol();
void pre_auton();
void reset_robot();

void threshold_mobile_goal();
void release_mobile_goal();
void show_threshold_degree();

void slapper_down();
void slapper_up();

void spin_flipper(int angle);
void pushback_flipper();

void catcher_swing_back();
void catcher_up();
void catcher_down();