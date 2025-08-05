#pragma once

#include "vex.h"
#include <string>

class SampleRemoteControl {
private:
    static FILE* serialFile;
    static std::string lineBuffer;
    
    void processCommand(const std::string& command);
    
public:
    SampleRemoteControl();
    ~SampleRemoteControl();
    
    // Communication methods
    void send(const std::string& message);
    void poll();
};
