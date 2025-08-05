#pragma once

#include "vex.h"
#include <string>

namespace rgb {

class SampleRemoteControl {
private:
    static FILE* serialFile;
    static std::string lineBuffer;
    
    void processCommand(const std::string& command);
    
public:
    SampleRemoteControl();
    ~SampleRemoteControl();
    
    // Communication methods
    bool send(const std::string& message);
    void poll();
};

} // namespace rgb 