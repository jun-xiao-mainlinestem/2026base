I have created a folder called 'remote_control'. I would like to create a mobile web application in this folder. Here is the descrption of what the application does:

1. Ask the user to input IP address and device ID.
2. Connect to the websocket server, the same way as the 'command_line_control' app does.
3. Once connected, show a select box that allows the user to select commands as a list.
4. The commands are: drive, turn, set heading.
5. The parameters for drive are: distance, voltage, heading. Distance are integers numbers in inches, including negative numbers. Maximum voltage is 12, with default voltage set to 10. Heading is 0 to 360, with default to current heading.
6. The parameter for turn is: heading, voltage.
7. The user clicks on send command to send the command.
8. The actual text sent to the websocket server should be in the format like this: drive 12 10 90 \n.

I want the UI modern but simple. Use typescript to develop the application. The end result should be simple html and javascript in the 'dist' folder that I can deploy directly to netlify.

Do not write any code yet, simply create an implementation plan in markdown that is saved in the 'PRD' folder for me to review. 



I have updated the plan, implement the plan in the folder 'RGB_remote'. Do not change any files outside that folder.