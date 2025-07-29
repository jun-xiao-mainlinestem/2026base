# Quick Start Guide - Voice-Controlled Robot

## 🚀 Get Started in 5 Minutes

### Prerequisites
- VEX V5 Controller connected via USB and remotely paired with the VEX Brain
- VEX VS Code Extension installed and running
- Computer with microphone
- Node.js installed

### Step 1: Setup VEX Extension
1. Open VS Code with VEX Extension
2. Connect VEX brain via USB
3. **Configure WebSocket Server**:
   - Go to VEX Extension settings: https://kb.vex.com/hc/en-us/articles/21410932256660-Understanding-the-User-Settings-for-VEX-VS-Code-Extension
   - Enable WebSocket Server in extension settings
4. **Get your Device ID**:
   - View brain info: https://kb.vex.com/hc/en-us/articles/8608865978644-VS-Code-UI-Overview-for-V5
   - Note your Device ID (process ID) - this is your unique identifier
   - The WebSocket URL format is: `ws://localhost:7071/vexrobotics.vexcode/device?id=YOUR_DEVICE_ID`

### Step 2: Setup Web Application
```bash
cd voice-control-robot
npm install
npm run dev
```

### Step 3: Upload VEX Code
1. Review [drivetrain configuration](/readme.md) and download the sample VEX program to VEX brain
2. Run the program and press Button LEFT on controller within 5 seconds to enable listening
3. Verify "Serial listening ON" appears on controller

### Step 4: Start Voice Control
1. Open browser to `http://localhost:3000`
2. Enter your Device ID in the input field
3. Click "Connect to Robot"
4. Click "Start Listening"
5. Say "move" to test

## 🎤 Voice Commands

| Command | Action |
|---------|--------|
| "move" | Drive forward |
| "back" | Drive backward |
| "left" | Turn left |
| "right" | Turn right |
| "stop" | Stop and report status |
| "roll" | Start intake (continues) |
| "shoot" | Start scoring (continues) |

## 🔧 Troubleshooting

### Connection Issues
- Check Device ID is correct (get from brain info)
- Ensure Button LEFT is pressed on controller within 5 seconds
- Verify USB connection
- Check WebSocket server is enabled in VEX Extension settings

### Voice Recognition Issues
- Check microphone permissions
- Speak clearly and slowly
- Try different browsers

### Robot Not Moving
- Check controller screen for "Serial listening ON"
- Verify commands in activity log
- Test with "stop" command first

## 📞 Need Help?

1. Check the full README.md
2. Review [Implmentation Document](doc/voice_control_implementation.md)
3. Check browser console for errors
4. Verify all connections

## ✅ Success Indicators

- ✅ WebSocket shows "🟢 Connected"
- ✅ Controller shows "Serial listening ON"
- ✅ Voice commands appear in activity log
- ✅ Robot responds to "move" command
- ✅ Status updates when "stop" is said

---

**Ready to control your robot with voice commands!** 🎤🤖 