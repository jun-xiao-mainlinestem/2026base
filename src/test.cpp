#include "vex.h"
aivision::aiobjdesc aiOjbect = aivision::ALL_AIOBJS;
aivision aiVision = aivision(PORT19, aiOjbect);

distance frontDistance = distance(PORT18);
void aiAction()
{
  chassis.driveWithVoltage(2, -2);
  bool foundBlueBall = false;
  while(controller(primary).ButtonX.pressing() && !foundBlueBall) {
    aiVision.takeSnapshot(aivision::ALL_AIOBJS);
    // Check if the blue ball is detected in the snapshot.
    if (aiVision.objectCount > 0) {
      // Iterate through all detected objects.
      for (int i = 0; i < aiVision.objectCount; i++) {
        aivision::object obj = aiVision.objects[i] ;
        // Check if the object is a blue ball.
        if (obj.id == 0) {
          // Print the coordinates of the blue ball.
            controller(primary).rumble(".");
                char msg[50];
          sprintf(msg, "Ball: x=%d, y=%d   ", obj.centerX, obj.centerY);
          printControllerScreen(msg);
          // Stop the robot and exit the loop.
          chassis.stop(coast);
          foundBlueBall = true;
          break;
        }
      }
      wait(20, msec);
    }
  }
  chassis.stop(coast);
}


int thread_heading = 90;
double thread_distance = 18;

void thread_drive_sensor()
{
  while(fabs(chassis.getHeading() - thread_heading) > 3)
  {
    wait(20, msec);
  }
  chassis.desiredDistance = (chassis.getLeftPositionIn() + chassis.getRightPositionIn()) / 2.0  + frontDistance.objectDistance(inches) - thread_distance;
  while(frontDistance.objectDistance(inches) > thread_distance)
  {
    wait(10, msec);
  }
  chassis.drivetrainNeedsStopped = true;
}

void awp()
{
  chassis.setHeading(0); 
 /* chassis.driveDistance(25);
  rollerBottom.spin(forward, 6, volt);
  rollerTop.spin(forward, -6, volt);
  wait(100, msec);
  stopRollers();
  chassis.turnToHeading(90);
  inTake();
  wait(200, msec);
  chassis.driveDistance(12, 3);
  wait(200, msec);
  stopRollers();
  */
 chassis.turnToHeading(160);
 thread distanceThread = thread(thread_drive_sensor);
  chassis.driveDistance(30, 8, 90, 4);
  chassis.turnToHeading(180);
}
