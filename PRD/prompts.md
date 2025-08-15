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

I would like to create a voice control mobile app in 'voice-control-robot' folder. Here is what you need to do. Read index.html in the RGB_web_simple folder and read the test.html in the voice-control-robot folder. Create a similar simple html with javascript style and CSS like the index.html. Ask the user for device ID and connect to the websocket server. Once connected allow the user to use the microphone to dictate a short sentence. Display the recognized text. Make the UI and code as simple as possible. As a first test, parse the user input to see if it includes 'red' and 'go'. If so, send the message "red go" to the websocket server. Put the generated html and markdown document inside the 'voice-control-robot' folder. Do not modify anything outside that folder.


I have created a new swift project under the folder remoteApp. I would like to develop a native iOS application to control the vex robot in a similar way as we did with the web app before. The reason it's native app is becasue I want to access the microphone and gyro data input from the iphone. So here is what I would like you to do.

the layout of the screen should be landscape. 

The user first need to enter the device ID similiar to the web app to connect to the websocket server. 

Once connected, the user can drive the robot either with speech or gyro (with UI buttons).

When the user give a speech input and the recognized text should be shown. The whole text will be sent to the websocket server.

The user can tilt the phone to turn the robot: +- voltage differently for left and right.  there should be some kind of slider or buttons (depends your UI design) to drive the robot forward/backward (+- voltage to the both sides of the drivetrain). there also should be stop button. the drive command (update) will be sent to the websocket server as "vol <left voltage> <right voltage>. limit the voltage to low number to slow down the sensitivity of the control. 




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

There should be one difference between the web app and ios app. the web app get the ip address automatically. in ios, I think the user need to enter the ip address. 

Speed levels please with slow, medium, fast (forward/backward). map to voltage 3v, 4v, 5v. the turn voltage should 1v incremental, not 0.3v.

now, let's test the drive UI in the simulator. for the moment, show the drive UI immediately. and which send drive command simply shown on the screen, ignore the websocket part. I would like to test the sensitivty of tilt -> voltage. 

the mapping is not what i wanted. let's just test the forward and backward command first. what i want is this
1. use Integer number for voltage, not float.  
2. when the phone is flat (right after the user click drive button), it sends the command vol 0, 0. 
3. when the phone is tilt forward to certain threhold (e.g. 20 degree), it sends the command vol +3 +3
4. if the phone changed its angle, but still within 0-20 degreee, do not sends any command.
5. if the phone change its angel to more than 20 degree, sends the command vol +6 +6.
6. do similar things when the phone is tilted backward.

the mapping is wrong. when I swing to a large angle and hold there. the message sent are.
Received message from client: vol 0 0
Received message from client: vol 6 6
Received message from client: vol 0 0

It should not send the last 'vol 0 0' message. 

after i tap drive button, i quickly swing to a large angle and hold there, and then swing back to flat. here is the message sent.

Received message from client: vol 0 0

in general, it is working, but not I would like for the mapping. let me explain. you only sent command when there is a angle change (large enough for 1v differnce). the base vol for left and right drivetrain when it is flat should be 0, 0. If I tilt forward 30 and left 30 degree, the left voltage should be 6+6 = 12v, the right side should be 6-6 = 0v. If I tilt just left 30 degree, the left voltage should be +6, the right side should be 6v.

the forward/backward tilt will add or minus 