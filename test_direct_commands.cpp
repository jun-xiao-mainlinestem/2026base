// Simple test to verify robot commands work directly
// Add this to main.cpp temporarily to test robot movement

void test_robot_commands() {
  // Test forward movement
  controller(primary).Screen.print("Testing FORWARD");
  chassis.drive_with_voltage(4, 4);
  wait(2000, msec);
  
  // Test stop
  controller(primary).Screen.print("Testing STOP");
  chassis.stop(brake);
  wait(1000, msec);
  
  // Test left turn
  controller(primary).Screen.print("Testing LEFT");
  chassis.drive_with_voltage(-4, 4);
  wait(2000, msec);
  
  // Test stop
  controller(primary).Screen.print("Testing STOP");
  chassis.stop(brake);
  wait(1000, msec);
  
  // Test right turn
  controller(primary).Screen.print("Testing RIGHT");
  chassis.drive_with_voltage(4, -4);
  wait(2000, msec);
  
  // Test stop
  controller(primary).Screen.print("Testing STOP");
  chassis.stop(brake);
  
  controller(primary).Screen.print("Test complete!");
}

// Add this to buttonA_action() to run the test
// test_robot_commands(); 