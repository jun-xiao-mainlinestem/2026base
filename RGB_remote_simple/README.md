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
1. **Configure WebSocket Server**:
   - Open VS Code with VEX Extension
   - Go to VEX Extension settings by clicking the 'settings' icon at the bottom left of the UI and typing 'vex' in the search box.
   - Enable the websocket server, enter the ip address and leave the port as '7071'. See the [image](vex-vscode-settings.png).

2. **Get your Device ID**:
   - Connect VEX V5 Controller via USB to the computer and remotely paired with the VEX Brain 
   - Download and run any RGB template based program on the robot
   - Get to the VEX device view by clicking on the 'vex' icon at the left bar. View [VEX device info](https://kb.vex.com/hc/en-us/articles/8608865978644-VS-Code-UI-Overview-for-V5)
   - Note the Brain Device ID for the WebSocket URL in the format of `ws://localhost:7071/vexrobotics.vexcode/device?id=YOUR_DEVICE_ID'. See the [image](device-id.png). Please note that every time you exit and restart VSCode, the id will change.

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

## Requirements

- **Web Server**: Python 3 (or any HTTP server)
- **Robot**: VEX V5 with WebSocket server running on port 7071
- **Browser**: Modern web browser with WebSocket support
- **Network**: Devices must be on the same local network

## File Structure

```
RGB_remote_simple/
└── index.html          # Single file with everything embedded
```

## Troubleshooting

### **Can't connect from mobile?**
- Ensure both devices are on the same WiFi network
- Check your computer's firewall settings
- Verify the IP address is correct

### **Connection fails?**
- Verify VEX VSCode extension WebSocket server is running on port 7071
- Run RGB-template based program on the robot
- Check computer IP address and robot brain device ID
- Ensure network connectivity between devices
