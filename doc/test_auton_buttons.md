# Test Auton Button System

## 🎮 Overview

The test auton system allows you to test autonomous routines during driver control. It uses two buttons: **Right** (for navigation) and **A** (for execution).

## 🔄 Complete Flow of Actions

### 1. 🚀 Entering Test Mode
**Button: Right** (within first 5 seconds of program startup)

```cpp
if ((Brain.Timer.time(sec) < 5) && !autonTestMode) {
    controller(primary).rumble("-");
    autonTestMode = true;
    return;
}
```

**What happens:**
- ✅ Checks if program has been running for less than 5 seconds
- ✅ Checks if not already in test mode
- ✅ Controller rumbles with "-" pattern
- ✅ Sets `autonTestMode = true`

---

### 2. 📋 Navigating the Auton Menu
**Button: Right** (when already in test mode)

```cpp
if (autonTestMode) {
    currentAutonSelection = (currentAutonSelection + 1) % autonNum;
    showAutonMenu();
    return;
}
```

**What happens:**
- ✅ Increments `currentAutonSelection` (cycles through available autons)
- ✅ Calls `showAutonMenu()` which:
  - Calculates total number of autons (`autonNum`)
  - Displays menu on brain and controller screens
  - Shows current selection (e.g., "auton1", "auton2")

**Navigation Options:**
- **Right Button**: Move forward in menu (auton1 → auton2 → auton1...)
- **Left Button**: Move backward in menu (auton2 → auton1 → auton2...)

---

### 3. 🏃‍♂️ Running the Selected Auton
**Button: A** (when in test mode)

```cpp
if (autonTestMode) {
    chassis.driverControlDisabled = true;
    Brain.Timer.clear();
    
    runAutonItem(); 
    
    double t = Brain.Timer.time(sec);
    char timeMsg[30];
    sprintf(timeMsg, "run time: %.1f", t);
    printControllerScreen(timeMsg);
    chassis.driverControlDisabled = false;
    return;
}
```

**What happens:**
- ✅ Disables driver control (prevents interference)
- ✅ Clears the brain timer
- ✅ Calls `runAutonItem()` which:
  - Uses `switch` statement to run selected auton
  - Executes the actual autonomous routine
- ✅ Calculates and displays run time
- ✅ Re-enables driver control

---

### 4. 🎯 Auton Execution Details

**`runAutonItem()` function:**
```cpp
void runAutonItem() {
    switch (currentAutonSelection) {
    case 0:
        test1();  // Runs auton1
        break;
    case 1:
        test2();  // Runs auton2
        break;
    }
}
```

---

### 5. ⏸️ Step-by-Step Execution (Optional)
**Button: A** (hold during auton execution)

```cpp
bool shouldContinueAutonStep() {
    return !(autonTestMode && !controller(primary).ButtonA.pressing());
}
```

**What happens:**
- ✅ If you **hold** the A button during auton execution
- ✅ The auton will **pause** at each step
- ✅ Release A button to continue to next step
- ✅ This allows fine-grained testing of complex autons

---

## 📋 Button Summary

| Button | Action | When |
|--------|--------|------|
| **Right** | Enter test mode | Within 5 seconds of startup |
| **Right** | Next auton in menu | When in test mode |
| **Left** | Previous auton in menu | When in test mode |
| **A** | Run selected auton | When in test mode |
| **A** (hold) | Step-by-step execution | During auton execution |

---

## 🎯 Example Flow

1. **Start program** → Robot boots up
2. **Press Right** (within 5 seconds) → Enter test mode, see "auton1" on screen
3. **Press Right** → See "auton2" on screen  
4. **Press A** → Run auton2, see "run time: 1.2" on screen
5. **Press Right** → Back to "auton1" on screen
6. **Press A** → Run auton1 (more complex movement)

---

## 🔧 Technical Details

### Global Variables
```cpp
bool autonTestMode = false;           // Indicates if in test mode
int currentAutonSelection = 0;        // Current auton selection
int autonNum;                         // Total number of autons
```

### Key Functions
- `showAutonMenu()`: Displays current selection on brain/controller
- `runAutonItem()`: Executes the selected autonomous routine
- `shouldContinueAutonStep()`: Controls step-by-step execution

### Menu System
The menu system automatically calculates the number of available autons based on the `autonMenuText` array in `autons.cpp`:

```cpp
char const * autonMenuText[] = {
    "auton1",
    "auton2"
};
```

---

## 💡 Tips for Using

1. **Quick Testing**: Use this system to rapidly test auton changes
2. **Step-by-Step**: Hold A during complex autons to debug step by step
3. **Menu Navigation**: Use Right/Left to quickly switch between different autons
4. **Timing**: Remember to press Right within 5 seconds of program startup
5. **Safety**: Driver control is disabled during auton execution for safety

---

## 🚀 Benefits

- **Rapid Iteration**: Quickly test different auton routines
- **Step Debugging**: Use hold-A feature to debug complex movements
- **Menu System**: Easy navigation between multiple autons
- **Timing Feedback**: See execution time for performance optimization

This system allows you to quickly test and iterate on autonomous routines!