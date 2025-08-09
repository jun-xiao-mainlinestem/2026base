# steps to create a clean open repository
- new v5 project (named rgb_sample)
- add .gitignore file
- commit and pubblish in github
- copy src, doc, and include folder
    - remove test.h test.cpp and reference of it
    - clean auton.cpp
    - remove macroMode in main.cpp
    - in main.cpp remove button macros for left/right and others
    - check if joystickTouched is still needed
    - remove unused motors and sensors from robot-config.cpp and h
    - remove setupTeamColor and color sorting
    - remove R2 button
- build the project to see if there is any error.
- add url link to the repository for readme.md


# code to add
- Remote control web app, sampleremotecontorl.cpp, readme doc
- In exitAuton, add immediate actions right at the start of the driver control.

