#include "rgb-template/RemoteControl.h"
#include "chassis.h"
#include <cstring>

namespace rgb {

// Static member initialization
FILE* RemoteControl::serialFile = nullptr;
std::string RemoteControl::lineBuffer = "";

RemoteControl::RemoteControl() : connected(false) {
    // Initialize remote control
}

RemoteControl::~RemoteControl() {
    if (connected) {
        disconnect();
    }
}

bool RemoteControl::connect() {
    connected = true;
    updateStatus(true, "           connection on");
    return connected;
}

void RemoteControl::disconnect() {
    if (connected) {
        connected = false;
        updateStatus(true, "           connection off");
    }
}

bool RemoteControl::send(const std::string& message) {
    if (!connected) return false;
    
    // Send data back to the computer via printf
    printf("%s", message.c_str());
    return true;
}

void RemoteControl::updateStatus(bool success, const char* status_message) {
    controller(primary).Screen.clearScreen();
    print_controller_screen(status_message);
    controller(primary).rumble(success ? "." : "--");
}

bool RemoteControl::attemptConnection() {
    if (connect()) {
        return true;
    } else {
        updateStatus(false, "           connection failed");
        return false;
    }
}

void RemoteControl::poll() {
    if (!connected) return;

    // Open serial port if not already open
    if (!serialFile) {
        serialFile = fopen("/dev/serial1", "rb");
        if (!serialFile) {
            controller(primary).Screen.clearScreen();
            print_controller_screen("can't open port");
            controller(primary).rumble(".");
            return;
        }
    }

    // Read one character at a time and build lines
    char c;
    if (fread(&c, 1, 1, serialFile) == 1) {
        if (c == '\n' || c == '\r') {
            // End of line reached, process the command
            if (!lineBuffer.empty()) {
                controller(primary).Screen.clearScreen();
                char cmdMsg[50];
    sprintf(cmdMsg, "Command: %s", lineBuffer.c_str());
    print_controller_screen(cmdMsg);
                processCommand(lineBuffer);
                lineBuffer.clear();
            }
        } else {
            // Add character to line buffer
            lineBuffer += c;
        }
    }
}

void RemoteControl::processCommand(const std::string& command) {
    // Remove any whitespace and newlines
    std::string cmd = command;
    cmd.erase(0, cmd.find_first_not_of(" \t\r\n"));
    cmd.erase(cmd.find_last_not_of(" \t\r\n") + 1);
    
    // Convert to uppercase for case-insensitive comparison
    for (char& c : cmd) {
        c = toupper(c);
    }
    
    // Debug: print the cmd value
    controller(primary).Screen.clearScreen();
    char cmdDebug[30];
    sprintf(cmdDebug, "cmd: '%s'", cmd.c_str());
    print_controller_screen(cmdDebug);
    controller(primary).Screen.newLine();
    
    // Map command to action
    if (cmd == "FORWARD" || cmd == "MOVE" || cmd == "GO") {
        controller(primary).rumble(".");
        chassis.drive_with_voltage(2, 2);
    } else if (cmd == "STOP") {
        controller(primary).rumble(".");
        float current_heading = chassis.get_heading();
        float distance_traveled = (chassis.get_left_position_in() + chassis.get_right_position_in()) / 2.0;
        chassis.stop(brake);
        stopRollers();
        
        // Send status back to webpage            
        char status_message[100];
        sprintf(status_message, "STATUS:%.1f:%.1f\n", current_heading, distance_traveled);
        send(status_message);

    } else if (cmd == "RIGHT") {
        controller(primary).rumble(".");
        chassis.drive_with_voltage(2, -2);
    } else if (cmd == "LEFT") {
        controller(primary).rumble(".");
        chassis.drive_with_voltage(-2, 2);
    } else if (cmd == "BACKWARD" || cmd == "BACK") {
        controller(primary).rumble(".");
        chassis.drive_with_voltage(-2, -2);
    } else if (cmd == "ROLL" || cmd == "INTAKE") {
        controller(primary).rumble(".");
        inTake();
    } else if (cmd == "SHOOT" || cmd == "SCORE") {
        controller(primary).rumble(".");
        scoreLong();
    } else {
        // Unknown command
        wait(2, seconds);
        controller(primary).Screen.clearScreen();
        char unknownMsg[30];
    sprintf(unknownMsg, "Unknown: %s", cmd.c_str());
    print_controller_screen(unknownMsg);
        controller(primary).rumble(".");
    }
}

} // namespace rgb 