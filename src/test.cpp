
#include "vex.h"

void awp()
{
  chassis.setHeading(0); 
  chassis.driveDistance(25);
  rollerBottom.spin(forward, 6, volt);
  rollerTop.spin(forward, -6, volt);
  wait(100, msec);
  stopRollers();
  chassis.turnToHeading(90);
  intake();
  wait(200, msec);
  chassis.driveDistance(13, 3);
  wait(200, msec);
  stopRollers();
  chassis.turnToHeading(180);
  chassis.driveDistance(24);
  chassis.turnToHeading(90);
  float d = frontDistance.objectDistance(inches);
  chassis.driveDistance(d - 14);
  chassis.turnToHeading(180);
  wait(3, seconds);
  chassis.turnToHeading(0);
  chassis.driveDistance(12);
}



void solo_test_8_17(){
  chassis.setHeading(320);
  chassis.driveDistance(26, 10, 320, 6, 2);
//  scoreMiddle();
  chassis.driveDistance(30, 6);
  wait(1, sec);
  chassis.driveDistance(-10, 10, 43, 6, 2);
  stopRollers();
  chassis.turnToHeading(90, 10, 4);
//  scoreMiddle();
  chassis.driveDistance(46);

}