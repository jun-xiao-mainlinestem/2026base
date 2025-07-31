# Test Auton Button System

## 🎮 Overview

The test auton system allows you to test autonomous routines during driver control. It uses two buttons: **Right** (for navigation) and **A** (for execution).

## 🔄 Complete Flow of Actions

### 1. 🚀 Entering Test Mode
**Button: Right** (within first 5 seconds of program startup)

```cpp
if ((Brain.Timer.time(sec) < 5) && !auton_test_mode) {
    controller(primary).rumble("-");
    auton_test_mode = true;
    return;
}
```

**What happens:**
- ✅ Checks if program has been running for less than 5 seconds
- ✅ Checks if not already in test mode
- ✅ Controller rumbles with "-" pattern
- ✅ Sets `auton_test_mode = true`

---

### 2. 📋 Navigating the Auton Menu
**Button: Right** (when already in test mode)

```cpp
if (auton_test_mode) {
    current_auton_selection = (current_auton_selection + 1) % auton_num;
    show_auton_menu();
    return;
}
```

**What happens:**
- ✅ Increments `current_auton_selection` (cycles through available autons)
- ✅ Calls `show_auton_menu()` which:
  - Calculates total number of autons (`auton_num`)
  - Displays menu on brain and controller screens
  - Shows current selection (e.g., "auton1", "auton2")

**Navigation Options:**
- **Right Button**: Move forward in menu (auton1 → auton2 → auton1...)
- **Left Button**: Move backward in menu (auton2 → auton1 → auton2...)

---

### 3. 🏃‍♂️ Running the Selected Auton
**Button: A** (when in test mode)

```cpp
if (auton_test_mode) {
    chassis.driver_control_disabled = true;
    Brain.Timer.clear();
    
    run_auton_item(); 
    
    double t = Brain.Timer.time(sec);
    controller(primary).Screen.print("run time: %.1f   ", t);
    chassis.driver_control_disabled = false;
    return;
}
```

**What happens:**
- ✅ Disables driver control (prevents interference)
- ✅ Clears the brain timer
- ✅ Calls `run_auton_item()` which:
  - Uses `switch` statement to run selected auton
  - Executes the actual autonomous routine
- ✅ Calculates and displays run time
- ✅ Re-enables driver control

---

### 4. 🎯 Auton Execution Details

**`run_auton_item()` function:**
```cpp
void run_auton_item() {
    switch (current_auton_selection) {
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
bool should_continue_auton_step() {
    return !(auton_test_mode && !controller(primary).ButtonA.pressing());
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
bool auton_test_mode = false;           // Indicates if in test mode
int current_auton_selection = 0;        // Current auton selection
int auton_num;                          // Total number of autons
```

### Key Functions
- `show_auton_menu()`: Displays current selection on brain/controller
- `run_auton_item()`: Executes the selected autonomous routine
- `should_continue_auton_step()`: Controls step-by-step execution

### Menu System
The menu system automatically calculates the number of available autons based on the `auton_menu_text` array in `autons.cpp`:

```cpp
char const * auton_menu_text[] = {
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