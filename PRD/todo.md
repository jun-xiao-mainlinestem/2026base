# steps to create a clean open repository
- new v5 project (named rgb_sample)
- add .gitignore file
- commit and pubblish in github

- copy src, doc, include, and RGB_web_simple folder
- modify the code
    - delete button_control.md
    - clean auton.cpp, remove macro_mode, remove quick test
    - remove unused motors and sensors from robot-config.cpp and h
    - remove setupTeamColor and color sorting
    - fix bug of custom test

- build the project to see if there is any error
- test drive and test button pressing
- add correct video, doc url links in readme.md file

- commit the code


# code to add 
- 
In exitAuton, add immediate actions right at the start of the driver control.

# fixes
- declare brain at the top.

# private feature
- chaining    
```cpp
// Turns the robot to a specific heading without stopping.
    // earlyExitFactor should between 1-5
  void turnToHeading(float heading, float turnMaxVoltage, float earlyExitFactor);
      // Drives the robot a specific distance without stopping.
  void driveDistance(float distance, float driveMaxVoltage, float heading, float headingMaxVoltage,  float earlyExitFactor);
```
# hardare
- matchload refine
- descore horn
- two-side piston
- middle roller motor placement and speed up (done)
- cable management
- opponent ball basket
- own ball baskset (plastic, cart board)
- 6 wheel drivetrain 

# software
- tune PID (fast, normal)
- color sort
- autons
- button mapping
- pre-auton read/write to sd card
- rotation sensor to select auton
- sd save parameter

