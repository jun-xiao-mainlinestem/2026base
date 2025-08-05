#include "vex.h"
aivision::aiobjdesc aiOjbect = aivision::ALL_AIOBJS;
aivision aiVision = aivision(PORT19, aiOjbect);

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
