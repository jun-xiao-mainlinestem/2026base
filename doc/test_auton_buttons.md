# Test Auton Button System

## 🎮 Overview

The test auton system allows you to test autonomous routines during driver control with step-by-step execution capability. It uses multiple buttons for navigation and execution control.

## 🔄 Complete Flow of Actions

### 1. 🚀 Entering Test Mode
**Button: Right** (within first 5 seconds of program startup)

```cpp
if ((Brain.Timer.time(sec) < 5) && !autonTestMode) {
    controller(primary).rumble("-");
    printControllerScreen("Test Mode: ON");
    wait(1, sec);
    showAutonMenu();
    autonTestMode = true;
    autonTestStep = 1;
    return true;
}
```

**What happens:**
- ✅ Checks if program has been running for less than 5 seconds
- ✅ Checks if not already in test mode
- ✅ Controller rumbles with "-" pattern
- ✅ Displays "Test Mode: ON" on controller screen
- ✅ Shows auton menu
- ✅ Sets `autonTestMode = true` and `autonTestStep = 1`

---

### 2. 📋 Navigating the Auton Menu
**Button: Right** (when already in test mode)

```cpp
if (autonTestMode) {
    controller(primary).rumble(".");
    currentAutonSelection = (currentAutonSelection + 1) % autonNum;
    autonTestStep = 1;
    showAutonMenu();
    return true;
}
```

**What happens:**
- ✅ Increments `currentAutonSelection` (cycles through available autons)
- ✅ Resets `autonTestStep` to 1 for new auton
- ✅ Calls `showAutonMenu()` which displays menu on brain and controller screens
- ✅ Shows current selection (e.g., "auton1", "auton2", "auton_skill")

**Navigation Options:**
- **Right Button**: Move forward in menu (auton1 → auton2 → auton_skill → auton1...)
- **Left Button**: Move backward in menu (auton_skill → auton2 → auton1 → auton_skill...)

---

### 3. 🎯 Step-by-Step Navigation
**Button: Up/Down** (when in test mode)

```cpp
// Up Button - Previous Step
if (autonTestMode) {
    controller(primary).rumble(".");
    if (autonTestStep > 1) autonTestStep--;
    controller(primary).Screen.print("Step: %d         ", autonTestStep);
    return true;
}

// Down Button - Next Step  
if (autonTestMode) {
    controller(primary).rumble(".");
    autonTestStep++;
    controller(primary).Screen.print("Step: %d           ", autonTestStep);
    return true;
}
```

**What happens:**
- ✅ **Up Button**: Decreases step number (if > 1)
- ✅ **Down Button**: Increases step number
- ✅ Controller rumbles to confirm action
- ✅ Displays current step on controller screen

---

### 4. 🏃‍♂️ Running the Selected Auton/Step
**Button: A** (when in test mode)

```cpp
if (autonTestMode) {
    controller(primary).rumble(".");
    
    double t1 = Brain.Timer.time(sec);
    
    runAutonItem(); 
    
    double t2 = Brain.Timer.time(sec);
    char timeMsg[30];
    sprintf(timeMsg, "run time: %.0f", t2-t1);
    printControllerScreen(timeMsg);
    chassis.stop(brake);
    
    return true;
}
```

**What happens:**
- ✅ Disables driver control (prevents interference)
- ✅ Records start time
- ✅ Calls `runAutonItem()` which executes the selected auton
- ✅ Calculates and displays run time
- ✅ Re-enables driver control
- ✅ Stops chassis with brake

---

### 5. 🎯 Step-by-Step Execution System

**Auton Functions with Step Control:**
```cpp
void auton_skill() {
    if (autonTestStep == 1) {
        chassis.turnToHeading(180);
        if (!continueAutonStep()) return; // Stop here if in test mode
    }
    if (autonTestStep == 2) {
        chassis.driveDistance(5);
        chassis.turnToHeading(chassis.getHeading() + 90);
        if (!continueAutonStep()) return; // Stop here if in test mode
    } 
    if(autonTestStep == 3) {
        chassis.turnToHeading(chassis.getHeading() - 90);
        chassis.driveDistance(-5);
    }
}
```

**Step Control Function:**
```cpp
bool continueAutonStep() {
    autonTestStep++;
    if (autonTestMode) {
        return false; // Stop execution in test mode
    }
    return true; // Continue execution in normal mode
}
```

**What happens:**
- ✅ Each step checks `autonTestStep` to determine what to execute
- ✅ `continueAutonStep()` increments step and returns false in test mode
- ✅ This causes the function to return early, stopping execution
- ✅ In normal mode, execution continues through all steps

---
### 6. Show status
**Button: B**
- ✅ If hold while driving, upon releasing display current heading and distance drived on the controller screen

### 7. Abort auto driving
- ✅ Move joystick to abort the auto driving.


## 📋 Button Summary

| Button | Action | When |
|--------|--------|------|
| **Right** | Enter test mode | Within 5 seconds of startup |
| **Right** | Next auton in menu | When in test mode |
| **Left** | Previous auton in menu | When in test mode |
| **Up** | Previous step | When in test mode |
| **Down** | Next step | When in test mode |
| **A** | Run selected auton/step | When in test mode |
| **B** | Show status | Always |
| **Movement of Joystick** | Abort auto driving | Always |

---

## 🎯 Example Flow

1. **Start program** → Robot boots up
2. **Press Right** (within 5 seconds) → Enter test mode, see "auton1" on screen
3. **Press Right** → See "auton2" on screen  
4. **Press Down** → Step increases to 2
5. **Press A** → Run step 2 of auton2, see "run time: 1" on screen
6. **Press Up** → Step decreases to 1
7. **Press A** → Run step 1 of auton2
8. **Press Right** → Back to "auton1" on screen

---

## 🔧 Technical Details

### Global Variables
```cpp
bool autonTestMode = false;           // Indicates if in test mode
int currentAutonSelection = 1;        // Current auton selection
int autonTestStep = 1;                // Current step in auton
int autonNum;                         // Total number of autons
```

### Key Functions
- `enterTestMode()`: Enters test mode and shows menu
- `nextAutonMenu()`: Moves to next auton in menu
- `prevAutonMenu()`: Moves to previous auton in menu
- `nextAutonStep()`: Increases step number
- `prevAutonStep()`: Decreases step number
- `runAutonTest()`: Executes current auton/step
- `continueAutonStep()`: Controls step progression

### Menu System
The menu system automatically calculates the number of available autons based on the `autonMenuText` array in `autons.cpp`:

```cpp
char const * autonMenuText[] = {
    "auton1",
    "auton2", 
    "auton_skill"
};
```

### Available Autons
- **auton1**: Simple forward movement
- **auton2**: Basic movement with heading
- **auton_skill**: Complex multi-step routine with step-by-step testing

---

## 💡 Tips for Using

1. **Quick Testing**: Use this system to rapidly test auton changes
2. **Step-by-Step**: Use Up/Down buttons to navigate through steps
3. **Menu Navigation**: Use Right/Left to quickly switch between different autons
4. **Timing**: Remember to press Right within 5 seconds of program startup
5. **Safety**: Driver control is disabled during auton execution for safety
6. **Step Control**: Each auton can have different numbers of steps
7. **Visual Feedback**: Controller screen shows current step and auton selection

---

## 🚀 Benefits

- **Rapid Iteration**: Quickly test different auton routines
- **Step Debugging**: Navigate through individual steps for precise testing
- **Menu System**: Easy navigation between multiple autons
- **Timing Feedback**: See execution time for performance optimization
- **Flexible Testing**: Test individual steps or full autons
- **Visual Feedback**: Clear indication of current step and auton

This system allows you to quickly test and iterate on autonomous routines with precise step-by-step control!