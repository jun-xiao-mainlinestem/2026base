# Xcode Project Setup

After creating the Swift files, you need to manually add the required frameworks and permissions to your Xcode project:

## Required Frameworks

1. **Speech.framework** - For speech recognition
2. **CoreMotion.framework** - For gyroscope and motion data
3. **Network.framework** - For WebSocket communication

## Steps to Add Frameworks

1. Open your project in Xcode
2. Select your project in the navigator
3. Select your target (remoteApp)
4. Go to the "General" tab
5. Scroll down to "Frameworks, Libraries, and Embedded Content"
6. Click the "+" button
7. Add the following frameworks:
   - Speech.framework
   - CoreMotion.framework
   - Network.framework

## Alternative Method

1. In Xcode, go to your target's "Build Phases"
2. Expand "Link Binary With Libraries"
3. Click "+" and add the required frameworks

## Required Permissions

You need to add the following permissions to your Info.plist through Xcode:

### Method 1: Through Xcode Interface
1. In Xcode, find your project's Info.plist file
2. Right-click and select "Open As" → "Source Code"
3. Add these keys inside the `<dict>` section:

```xml
<key>NSMicrophoneUsageDescription</key>
<string>This app needs access to the microphone to recognize speech commands for robot control.</string>
<key>NSSpeechRecognitionUsageDescription</key>
<string>This app uses speech recognition to convert your voice commands into robot control instructions.</string>
```

### Method 2: Through Target Settings
1. Select your target in Xcode
2. Go to "Info" tab
3. Add these keys:
   - **Privacy - Microphone Usage Description**: "This app needs access to the microphone to recognize speech commands for robot control."
   - **Privacy - Speech Recognition Usage Description**: "This app uses speech recognition to convert your voice commands into robot control instructions."

## Device Capabilities

Add these device capabilities to ensure the app works on devices with motion sensors:

1. In your target's "Info" tab, add:
   - **Required device capabilities**: Add `magnetometer`, `gyroscope`, `accelerometer`

## Interface Orientation

**CRITICAL**: You must configure the Info.plist to allow both portrait and landscape orientations for the dynamic orientation switching to work:

### Method 1: Through Xcode Interface (Recommended)
1. In your target's "Info" tab, set:
   - **Supported interface orientations**: Check ALL of these:
     - ✅ Portrait
     - ✅ Landscape (left home button)
     - ✅ Landscape (right home button)
   - **Supported interface orientations (iPad)**: Same options

### Method 2: Through Info.plist Source Code
1. Find your Info.plist file in Xcode
2. Right-click → "Open As" → "Source Code"
3. Add these keys inside the `<dict>` section:

```xml
<key>UISupportedInterfaceOrientations</key>
<array>
    <string>UIInterfaceOrientationPortrait</string>
    <string>UIInterfaceOrientationLandscapeLeft</string>
    <string>UIInterfaceOrientationLandscapeRight</string>
</array>

<key>UISupportedInterfaceOrientations~ipad</key>
<array>
    <string>UIInterfaceOrientationPortrait</string>
    <string>UIInterfaceOrientationLandscapeLeft</string>
    <string>UIInterfaceOrientationLandscapeRight</string>
</array>
```

## Why This is Critical

Without these settings:
- ❌ Connection screen will stay in landscape
- ❌ Orientation switching won't work
- ❌ App may crash or behave unexpectedly
- ❌ Dynamic orientation control won't function

**The app MUST support both portrait and landscape orientations for the dynamic switching to work properly!**

## Verify Setup

After adding the frameworks and permissions, your project should build without errors. The app will request the necessary permissions when it runs:

- Microphone access for speech recognition
- Speech recognition permission
- Motion data access (automatically granted)

## Build and Run

1. Select your target device (iPhone/iPad)
2. Press Cmd+R to build and run
3. Grant permissions when prompted
4. Test the app functionality

## Troubleshooting

If you encounter build errors:
- Ensure all frameworks are properly linked
- Check that Info.plist contains the required permission keys
- Verify that the device supports the required sensors
- Clean build folder (Shift+Cmd+K) and rebuild if needed
