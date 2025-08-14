# Voice Control Robot - Simple Voice Recognition App

A simple, single-file HTML web application that allows users to control VEX robots using voice commands. The app combines WebSocket communication with browser-based speech recognition for hands-free robot control.

## Features

- **Voice Recognition**: Uses browser's built-in speech recognition API
- **WebSocket Communication**: Connects to VEX robot via WebSocket
- **Simple Interface**: Clean, mobile-friendly design
- **Voice Command Parsing**: Automatically detects and sends robot commands
- **Real-time Feedback**: Shows recognized speech and command status

## How It Works

### 1. Connection
- User enters the robot's Device ID
- App automatically detects the server IP address
- Establishes WebSocket connection to the robot

### 2. Voice Control
- User clicks the microphone button
- App listens for voice input
- Displays recognized text with confidence level
- Automatically parses voice commands

### 3. Command Detection
- **Current Test**: Detects if speech contains both "red" and "go"
- **Command Sent**: Sends "red go" to the robot when detected
- **Future**: Can be extended to detect more complex commands

## Setup Instructions

### Prerequisites
- **VEX V5 Robot**: Running RGB template program
- **VS Code with VEX Extension**: WebSocket server enabled
- **Web Browser**: With speech recognition support (Chrome, Edge, Safari)

### 1. Configure VEX Extension
1. **Open VS Code** with the VEX Extension installed
2. **Access VEX Extension Settings**:
   - Click the gear icon (⚙️) at the bottom left of VS Code
   - Type 'vex' in the search box
   - Look for VEX Extension settings
3. **Enable WebSocket Server**:
   - Check the box to enable the WebSocket server
   - Set the IP address (use your computer's IP address)
   - Leave the port as '7071' (default)

### 2. Get Your Robot's Device ID
1. **Connect Your Robot**:
   - Connect VEX V5 Controller to computer via USB
   - Ensure the controller is paired with the VEX Brain
2. **Load and Run Program**:
   - Download any RGB template-based program to your robot
   - Make sure the program is running on the robot
3. **Find Device ID**:
   - Click the VEX icon in the left sidebar of VS Code
   - Look for your robot's Brain Device ID
   - **Important**: This ID changes every time you restart VS Code

### 3. Start the Web Server

**Open Terminal/Command Prompt and run:**

```bash
cd voice-control-robot
python3 -m http.server 8000
```

**You should see: "Serving HTTP on 0.0.0.0 port 8000..."**

> **Note**: If you get an error like "command not found" or "python3 is not recognized", you need to install Python first:
> - **Mac**: Download from [python.org](https://python.org) or use `brew install python3`
> - **Windows**: Download from [python.org](https://python.org) or use Microsoft Store

### 4. Open in Browser
Navigate to: `http://YOUR_IP_ADDRESS:8000`

**Example:** `http://192.168.1.100:8000`

## Usage

### 1. Connect to Robot
1. Enter your robot's Device ID
2. Click "Connect"
3. Wait for connection status to show "Connected"

### 2. Use Voice Control
1. **Click the microphone button** (🎤 Start Listening)
2. **Speak clearly** when you see "Listening... Speak now!"
3. **Wait for recognition** - the app will show what it heard
4. **Check for commands** - if you say "red go", it will send the command

### 3. Voice Command Examples
- **"The robot should go red"** → Sends "red go"
- **"Red light go now"** → Sends "red go"
- **"Go to the red area"** → Sends "red go"

## Technical Details

### Speech Recognition API
- **Browser Support**: Chrome, Edge, Safari (desktop and mobile)
- **Language**: English (US)
- **Mode**: Single utterance recognition
- **Confidence**: Shows recognition confidence percentage

### WebSocket Communication
- **URL Format**: `ws://{IP}:7071/vexrobotics.vexcode/device?id={DEVICE_ID}`
- **Protocol**: WebSocket over HTTP
- **Port**: 7071 (VEX extension default)
- **Message Format**: Commands sent with newline termination

### Voice Command Parsing
```javascript
function checkVoiceCommands(transcript) {
    const lowerText = transcript.toLowerCase();
    
    // Check if text contains 'red' and 'go'
    if (lowerText.includes('red') && lowerText.includes('go')) {
        sendVoiceCommand('red go');
    }
}
```

**Current Logic**: 
- Converts speech to lowercase
- Checks if both "red" and "go" are present
- Sends "red go" command if both words detected

## File Structure

```
voice-control-robot/
├── index.html          # Main voice control application
├── README.md           # This documentation file
└── test.html           # Original voice recognition test file
```

## Browser Compatibility

### Speech Recognition Support
- ✅ **Chrome/Chromium**: Full support
- ✅ **Edge**: Full support  
- ✅ **Safari**: Full support
- ❌ **Firefox**: No built-in support
- ❌ **Internet Explorer**: No support

### WebSocket Support
- ✅ **All Modern Browsers**: Full WebSocket support
- ✅ **Mobile Browsers**: Full support on iOS Safari, Chrome Mobile

## Troubleshooting

### Voice Recognition Issues
- **"Speech recognition not supported"**: Use Chrome, Edge, or Safari
- **"Already started"**: Wait for current recognition to finish
- **Poor recognition**: Speak clearly in a quiet environment
- **No microphone access**: Allow microphone permissions in browser

### Connection Issues
- **Can't connect to robot**: Verify VEX extension WebSocket server is running
- **WebSocket connection fails**: Check robot device ID and network connectivity
- **Robot not responding**: Ensure RGB template program is running on robot

### Mobile Device Issues
- **Voice not working on mobile**: Ensure microphone permissions are granted
- **Connection fails on mobile**: Verify both devices are on same WiFi network
- **Poor mobile performance**: Use Chrome or Safari on mobile devices

## Extending the App

### Adding New Voice Commands
To add more voice commands, modify the `checkVoiceCommands()` function:

```javascript
function checkVoiceCommands(transcript) {
    const lowerText = transcript.toLowerCase();
    
    // Existing command
    if (lowerText.includes('red') && lowerText.includes('go')) {
        sendVoiceCommand('red go');
    }
    
    // New command examples
    if (lowerText.includes('forward') && lowerText.includes('drive')) {
        sendVoiceCommand('drive 12');
    }
    
    if (lowerText.includes('turn') && lowerText.includes('right')) {
        sendVoiceCommand('turn 90');
    }
}
```

### Customizing Robot Commands
- **Drive commands**: `drive <distance>`
- **Turn commands**: `turn <heading>`
- **Set heading**: `set_heading <heading>`
- **Custom commands**: Any text the robot can parse

## Security Considerations

- **Local network only**: WebSocket connections are local
- **No persistent storage**: Voice data is not stored
- **Browser permissions**: Microphone access required
- **HTTPS required**: Speech recognition may require secure context on some browsers

## Performance Tips

- **Clear speech**: Speak clearly and at normal volume
- **Quiet environment**: Reduce background noise for better recognition
- **Close proximity**: Position microphone close to your mouth
- **Browser optimization**: Use Chrome or Edge for best performance

## Future Enhancements

- **Multiple command detection**: Recognize more complex voice patterns
- **Voice feedback**: Audio confirmation of recognized commands
- **Command history**: Log of all voice commands sent
- **Custom wake words**: Trigger recognition with specific phrases
- **Multi-language support**: Support for different languages

## Conclusion

The Voice Control Robot app provides a simple, effective way to control VEX robots using voice commands. By combining browser speech recognition with WebSocket communication, it creates an intuitive interface for hands-free robot control.

The app is designed to be:
- **Simple to use**: Minimal interface, clear feedback
- **Easy to extend**: Modular code structure for adding new commands
- **Mobile-friendly**: Responsive design for all devices
- **Reliable**: Robust error handling and connection management

Perfect for educational settings, demonstrations, or anyone who wants to control robots with their voice!
