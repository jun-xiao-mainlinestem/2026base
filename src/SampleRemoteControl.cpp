#include "SampleRemoteControl.h"
#include "robot-config.h"
#include <cstring>

FILE* SampleRemoteControl::serialFile = nullptr;
std::string SampleRemoteControl::lineBuffer = "";

SampleRemoteControl::SampleRemoteControl() {
    serialFile = fopen("/dev/serial1", "rb");
}

SampleRemoteControl::~SampleRemoteControl() {
    // Clean up resources
    if (serialFile) {
        fclose(serialFile);
        serialFile = nullptr;
    }
    lineBuffer.clear();
}

void SampleRemoteControl::send(const std::string& message) {    
    // Send data back to the computer via printf
    printf("%s", message.c_str());
}

void SampleRemoteControl::poll() {
    // Open serial port if not already open
    if (!serialFile) {
        serialFile = fopen("/dev/serial1", "rb");
        if (!serialFile) {
            controller(primary).Screen.clearScreen();
            printControllerScreen("can't open port");
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
                printControllerScreen(cmdMsg);
                processCommand(lineBuffer);
                lineBuffer.clear();
            }
        } else {
            // Add character to line buffer
            lineBuffer += c;
        }
    } else if (ferror(serialFile)) {
        // Handle read error
        printControllerScreen("serial read error");
        controller(primary).rumble(".");
        fclose(serialFile);
        serialFile = nullptr;
    }
}

void SampleRemoteControl::processCommand(const std::string& command) {
    std::string cmd = command;
    cmd.erase(0, cmd.find_first_not_of(" \t\r\n"));
    cmd.erase(cmd.find_last_not_of(" \t\r\n") + 1);
    
    controller(primary).rumble(".");
    // Map command to action
    if (cmd == "FORWARD" || cmd == "MOVE" || cmd == "GO") {
        chassis.driveWithVoltage(2, 2);
    } else if (cmd == "STOP") {
        chassis.stop(brake);
        stopRollers();
    } else if (cmd == "RIGHT") {
        chassis.driveWithVoltage(2, -2);
    } else if (cmd == "LEFT") {
        chassis.driveWithVoltage(-2, 2);
    } else if (cmd == "BACKWARD" || cmd == "BACK") {
        chassis.driveWithVoltage(-2, -2);
    } else if (cmd == "ROLL" || cmd == "INTAKE") {
        inTake();
    } else if (cmd == "SHOOT" || cmd == "SCORE") {
        scoreLong();
    } else {
        // Unknown command
        controller(primary).Screen.clearScreen();
        char unknownMsg[30];
        sprintf(unknownMsg, "Unknown: %s", cmd.c_str());
        printControllerScreen(unknownMsg);
    }
}
