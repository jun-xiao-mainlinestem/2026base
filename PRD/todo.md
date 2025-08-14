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

- build the project to see if there is any error
- test drive and test button pressing
- add correct video, doc url links in readme.md file

- commit the code


# code to add 
- In exitAuton, add immediate actions right at the start of the driver control.

