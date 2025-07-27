# Quick Start Guide - Voice-Controlled Robot

## 🚀 Get Started in 5 Minutes

### Prerequisites
- VEX V5 Brain + Controller connected via USB
- VEX VS Code Extension installed and running
- Computer with microphone
- Node.js installed

### Step 1: Setup VEX Extension
1. Open VS Code with VEX Extension
2. Connect VEX brain via USB
3. Enable WebSocket Server in extension settings
4. Note your Device ID (shown in extension)

### Step 2: Setup Web Interface
```bash
cd voice-control-robot
npm install
npm run dev
```

### Step 3: Configure Device ID
1. Open `src/app.ts`
2. Replace `"78490"` with your actual Device ID
3. Save the file

### Step 4: Upload VEX Code
1. Upload the updated `main.cpp` to VEX brain
2. Press Button X on controller to enable listening
3. Verify "Serial listening ON" appears on controller

### Step 5: Start Voice Control
1. Open browser to `http://localhost:3000`
2. Click "Connect to Robot"
3. Click "Start Listening"
4. Say "forward" to test

## 🎤 Voice Commands

| Command | Action |
|---------|--------|
| "forward" | Drive forward |
| "backward" | Drive backward |
| "left" | Turn left |
| "right" | Turn right |
| "stop" | Stop and report status |
| "intake" | Start intake (continues) |
| "score" | Start scoring (continues) |

## 🔧 Troubleshooting

### Connection Issues
- Check Device ID in `app.ts`
- Ensure Button X is pressed on controller
- Verify USB connection

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
2. Review doc/voice_control_implementation.md
3. Check browser console for errors
4. Verify all connections

## ✅ Success Indicators

- ✅ WebSocket shows "🟢 Connected"
- ✅ Controller shows "Serial listening ON"
- ✅ Voice commands appear in activity log
- ✅ Robot responds to "forward" command
- ✅ Status updates when "stop" is said

---

**Ready to control your robot with voice commands!** 🎤🤖 