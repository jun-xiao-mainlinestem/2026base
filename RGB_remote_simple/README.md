# Simple RGB Remote Control

A simple, single-file HTML web application for controlling VEX robots remotely.

## Features

- **Single HTML file** - no build process, no dependencies
- **Three command types**: Drive, Turn, Set Heading
- **Mobile-friendly** - works on smartphones and tablets


## How to Use

### Get Your Computer's IP Address

**Open Terminal/Command Prompt and run:**

#### On Mac:
```bash
ipconfig getifaddr en0
```

#### On Windows:
```cmd
ipconfig
```

**Look for your IP address (usually starts with 192.168.x.x or 10.0.x.x)**

### Setup VEX Extension

#### 1. Configure WebSocket Server
1. **Open VS Code** with the VEX Extension installed
2. **Access VEX Extension Settings**:
   - Click the gear icon (⚙️) at the bottom left of VS Code
   - Type 'vex' in the search box
   - Look for VEX Extension settings
3. **Enable WebSocket Server**:
   - Check the box to enable the WebSocket server (see [screen](vex-vscode-settings.png))
   - Set the IP address (use your computer's IP address)
   - Leave the port as '7071' (default)

#### 2. Get Your Robot's Device ID
1. **Connect Your Robot**:
   - Connect VEX V5 Controller to computer via USB
   - Ensure the controller is paired with the VEX Brain
2. **Load and Run Program**:
   - Download any RGB template-based program to your robot
   - Make sure the program is running on the robot
3. **Find Device ID**:
   - Click the VEX icon in the left sidebar of VS Code
   - Look for your robot's Brain Device ID as shown in the [screen](device-id.png)
   - **Important**: This ID changes every time you restart VS Code

#### 3. Verify Setup
- WebSocket server should be running on port 7071
- Robot should be running an RGB template program
- Note down the Device ID for use in the web app

### Start the Web Server

**Open Terminal/Command Prompt and run:**

```bash
cd RGB_remote_simple
python3 -m http.server 8000
```

**You should see: "Serving HTTP on 0.0.0.0 port 8000..."**

## Mobile Device Usage
Ensure your mobile device is on **the same WiFi network** as your computer.


### Open the Browser
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


## Troubleshooting

### Connection Issues

#### **Can't connect from mobile?**
- Ensure the computer and mobile device are on the same WiFi network
- Check the computer's firewall settings
- Verify the computer IP address is correct

#### **WebSocket connection fails?**
- Verify VEX VSCode extension WebSocket server is running on port 7071
- **Important**: Use your computer's IP address (not 'localhost') in VEX extension settings
- Ensure robot brain device ID is correct and current

#### **Robot not responding?**
- Check that RGB-template based program is running on the robot
- Verify the VEX ontroller is connected is connected to the computer via USB cable
- Ensure the robot brain and controller are properly paired