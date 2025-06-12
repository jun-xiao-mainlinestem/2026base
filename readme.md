RGB BOTS TEMPLATE IS INSPIRED BY JARS-TEMPLATE

# Install
go to [github](https://github.com/jun-xiao-mainlinestem/2026base), download the source code as zip file.

![source](./images/github.png)

# Create a new project
- unzip the downloaded file to a folder on local drive
- in vscdoe, select menu 'file' -> 'open folder' 

# Edit chassis.cpp
modify motor and inertia sensor port number

# Edit robot-config.cpp
add more motors and sensors and functions for other systems such as intake

# Edit autons.cpp
write your own auton functions with drivetrain (chassis) APIs:
```cpp
chassis.turn_to_heading(90);
chassis.drive_distance(24);
chassis.drive_with_voltage(6, 6);
```
# Edit main.cpp
write additional functions for game plays and change the gamepad controls (buttons).