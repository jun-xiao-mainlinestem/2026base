# RGB Remote Control - Simple Version

A simple, single-file HTML web application for controlling VEX robots remotely.

## Features

- **Single HTML file** - no build process, no dependencies
- **Three command types**: Drive, Turn, Set Heading
- **Enable/disable interface** - command section only active when connected
- **Mobile-friendly** - works on smartphones and tablets


## How to Use

### Get Your Computer's IP Address

#### On Mac:
```bash
ipconfig getifaddr en0
```

#### On Windows:
```cmd
ipconfig
```

### Setup VEX Extension

#### 1. Configure WebSocket Server
1. **Open VS Code** with the VEX Extension installed
2. **Access VEX Extension Settings**:
   - Click the gear icon (⚙️) at the bottom left of VS Code
   - Type 'vex' in the search box
   - Look for VEX Extension settings
3. **Enable WebSocket Server**:
   - Check the box to enable the WebSocket server
   - Set the IP address (use your computer's IP address)
   - Leave the port as '7071' (default)
   - Save the settings

#### 2. Get Your Robot's Device ID
1. **Connect Your Robot**:
   - Connect VEX V5 Controller to computer via USB
   - Ensure the controller is paired with the VEX Brain
2. **Load and Run Program**:
   - Download any RGB template-based program to your robot
   - Make sure the program is running on the robot
3. **Find Device ID**:
   - Click the VEX icon in the left sidebar of VS Code
   - Look for your robot's Brain Device ID as shown in the [image](device-id.png)
   - **Important**: This ID changes every time you restart VS Code

#### 3. Verify Setup
- WebSocket server should be running on port 7071
- Robot should be running an RGB template program
- Note down the Device ID for use in the web app

### Start the Web Server

```bash
cd RGB_remote_simple
python3 -m http.server 8000
```

## Mobile Device Usage
Ensure your mobile device is on **the same WiFi network** as your computer.


### Open in Browser
Navigate to: `http://YOUR_IP_ADDRESS:8000`

**Example:** `http://192.168.1.100:8000`

### Connect to Robot

-  Enter your web server's IP address
-  Enter your robot's Device ID
-  Click "Connect"

### Send Commands

Once connected, the command section becomes active:

- **Drive**: Enter distance in inches (negative for backward)
- **Turn**: Enter target heading (0-360°)
- **Set Heading**: Set current heading (0-360°)


## Command Format

The app sends these commands to your robot:

- `drive <distance>\n`
- `turn <heading>\n`
- `set_heading <heading>\n`


## Troubleshooting
**Can't connect from mobile?**
- Ensure the computer and mobile device are on the same WiFi network
- Check the computer's firewall settings
- Verify the computer IP address is correct
- Verify robot brain device ID is correct
- Verify VEX VSCode extension WebSocket server is running on port 7071 with the IP address (not 'localhost')
- Run RGB-template based program on the robot