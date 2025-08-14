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

Make a couple of changes.
1. all the parameter input should bring up the number input keyboard.
2. once in the command view, remove header "RGB remote control" and connected status text. it should assume it is connected. 
3. remove the "send comment" text and the "disconnect" button.
4. for the drive command, instead of using -1 as "current heading", by default, leave it input empty so that it means keep the current heading. 
5. make all the text bigger.

make a couple of changes.
1. the input for ip address should be number keyboard that has the '.'
2. the input  keyboard for id should be number
3. the input keyboard for parameters shoould has the '-' since negative numbers are allowed.
4. remove the 'connection succesful', 'command send' flyover message. 
5. once the user enter the ip address and id, show the the contructed ulr as text above the connect button (remind the user to verify the url text).

Change the voltage to "Max voltage". it will be from  1 to 12. 

please design the UI as I want to simplify the interaction.

The Command Interface is simplifed as 
- **Drive Command**: distance (inches)(negative or positive)
- **Turn Command**: heading (0-360°)
- **Set Heading Command**: heading (0-360°)

in drive.cpp, update the processCommand function to interpret the text sent from the remote control web app and preform the coresponding drive actions.

Though the web app looks ok. It's just too complicated. I would like to have a simple web page that serve the same function. show me how in markdown. Here is the requirement.

1. the whole app should be a single html with some javascript.
2. the web server starts simply with shell comand "python3 -m http.server"
3. no node.js and all the dependancy. 

Do not generate the code. just explain to me how.