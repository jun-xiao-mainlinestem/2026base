# Speech Recognition Server for Command Line Control - Implementation Plan

## Overview

This plan outlines the integration of offline speech recognition with natural language processing to enable voice commands for the VEX robot command line interface. The system combines whisper.cpp for speech-to-text with Rasa NLU for intent recognition and entity extraction in a single server.


## Architecture Components

### 1. Frontend (Browser-based Voice Interface)
- **Location**: `command_line_control/src/`
- **Technology**: TypeScript + MediaRecorder
- **Purpose**: Audio capture and user interface
- **Visibility**: Voice button only shown on localhost

### 2. Combined Speech Recognition & NLP Server
- **Location**: `offline_voice_whisper/` (enhanced)
- **Technology**: Node.js + Express + whisper.cpp + Rasa NLU
- **Purpose**: Convert speech to text and extract structured commands
- **Port**: 3000

### 3. Command Line Control Integration
- **Location**: `command_line_control/src/`
- **Technology**: TypeScript
- **Purpose**: Parse server results into robot commands

## Data Flow Architecture

```
┌─────────────────┐    ┌─────────────────────────────┐    ┌─────────────────┐
│   User Voice    │───▶│  Combined Whisper + NLP     │───▶│ Command Line    │
│   (Browser)     │    │  Server (Speech→Command)    │    │   Interface     │
└─────────────────┘    └─────────────────────────────┘    └─────────────────┘
         │                       │                               │
         ▼                       ▼                               ▼
   Audio Recording         Structured Command Data         Robot Commands
   (MediaRecorder)        (Intent + Entities)            (chassis.driveDistance)
```

## Detailed Component Breakdown

### 1. Frontend Voice Interface

#### Features:
- **Voice Button**: Only visible in local development (`localhost`)
- **Recording UI**: Visual feedback during recording
- **Command Display**: Show transcribed text and parsed command
- **Simple Integration**: Direct upload to combined server

#### Implementation:
```typescript
class VoiceRecognition {
  private isLocalDevelopment(): boolean {
    return window.location.hostname === 'localhost' || 
           window.location.hostname === '127.0.0.1';
  }
  
  async startRecording(): Promise<void> {
    const stream = await navigator.mediaDevices.getUserMedia({ audio: true });
    this.mediaRecorder = new MediaRecorder(stream);
    // ... recording logic
  }
  
  async processVoiceCommand(audioBlob: Blob): Promise<ParsedCommand> {
    // Send to combined whisper + NLP server
    const response = await fetch('http://localhost:3000/process-voice', {
      method: 'POST',
      body: formData
    });
    const result = await response.json();
    return this.parseServerResult(result);
  }
}
```

### 2. Combined Whisper + NLP Server

#### Enhanced Server Features:
- **Single Endpoint**: `/process-voice` handles both speech and NLP
- **Integrated Pipeline**: whisper.cpp → Rasa NLU → Structured output
- **Model Management**: Support multiple whisper models and NLP models
- **Error Handling**: Comprehensive error reporting

#### API Endpoints:
```
POST /process-voice
- Input: FormData with audio blob
- Output: { 
    transcript: string,
    intent: string,
    entities: object,
    command: ParsedCommand
  }

GET /health
- Output: { status: "healthy", whisper_model: "base", nlp_model: "rasa" }
```

#### Server Implementation:
```javascript
// Enhanced backend/index.js
app.post('/process-voice', upload.single('audio'), async (req, res) => {
  try {
    // Step 1: Whisper transcription
    const transcript = await processWithWhisper(req.file.path);
    
    // Step 2: NLP processing
    const nlpResult = await processWithRasa(transcript);
    
    // Step 3: Command parsing
    const command = parseToRobotCommand(nlpResult);
    
    res.json({
      transcript: transcript,
      intent: nlpResult.intent.name,
      entities: nlpResult.entities,
      command: command
    });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});
```

### 3. Command Line Control Integration

#### Voice Command Parsing:
```typescript
interface ServerResponse {
  transcript: string;
  intent: string;
  entities: object;
  command: ParsedCommand;
}

class VoiceCommandParser {
  parseServerResult(result: ServerResponse): ParsedCommand | null {
    if (result.command) {
      return result.command;
    }
    
    // Fallback parsing if server doesn't return structured command
    return this.parseFromIntent(result.intent, result.entities);
  }
  
  private parseFromIntent(intent: string, entities: any): ParsedCommand | null {
    switch (intent) {
      case 'drive_forward':
        return {
          type: 'drive',
          distance: entities.distance || 10,
          voltage: entities.voltage || 10
        };
      case 'turn_to_heading':
        return {
          type: 'turn',
          heading: entities.heading || 0,
          voltage: entities.voltage || 10
        };
      // ... more intents
    }
  }
}
```

## Implementation Phases

### Phase 1: Basic Voice Integration
1. **Add voice button** to command line interface (localhost only)
2. **Enhance whisper server** to include NLP processing
3. **Create audio recording interface** with visual feedback
4. **Test with simple commands**: "drive 10", "turn 90"

### Phase 2: NLP Integration
1. **Install and configure Rasa NLU** in the whisper server
2. **Create training data** based on command mapping
3. **Train NLP model** with robot-specific commands
4. **Implement intent recognition** and entity extraction

### Phase 3: Full Integration
1. **Connect all components** in proper sequence
2. **Add error handling** and validation
3. **Implement command validation** and feedback
4. **Add voice command history** and download functionality

## Technical Requirements

### Dependencies:
```json
// Combined server
{
  "express": "^4.18.2",
  "multer": "^1.4.5-lts.1",
  "child_process": "built-in",
  "fs": "built-in"
}

// Python dependencies for Rasa
{
  "rasa": "^3.6.0",
  "spacy": "^3.5.0"
}
```

## Error Handling Strategy

### 1. Speech Recognition Failures:
- **Low confidence**: Request user to repeat command
- **No transcription**: Show error message
- **Server unavailable**: Disable voice button

### 2. NLP Processing Failures:
- **Unknown intent**: Show available commands
- **Missing entities**: Prompt for specific parameters
- **Low confidence**: Ask for clarification

### 3. Command Parsing Failures:
- **Invalid parameters**: Show error and suggest corrections
- **Out of range values**: Validate and cap values
- **Unsupported commands**: List available commands

## User Experience Flow

### 1. Voice Command Process:
```
User clicks "🎤 Voice Command" (localhost only)
    ↓
"Listening..." (visual feedback)
    ↓
User speaks: "drive forward 24 inches"
    ↓
"Processing..." (whisper + NLP)
    ↓
"Recognized: drive_forward intent, distance: 24"
    ↓
"Executing: chassis.driveDistance(24, 10)"
    ↓
Command sent to robot
```

### 2. Error Recovery:
```
Low confidence transcription
    ↓
"Could not understand. Please repeat."
    ↓
User repeats command
    ↓
Retry with different parameters
```

## Testing Strategy

### 1. Unit Tests:
- **Voice parsing**: Test command extraction accuracy
- **NLP integration**: Test intent recognition
- **Error handling**: Test error scenarios

### 2. Integration Tests:
- **End-to-end**: Complete voice → robot command flow
- **Performance**: Measure response times
- **Reliability**: Test with various audio qualities

## Prerequisites for Combined Whisper + NLP Server
### Required Software Installation

#### 1. Git
- **Purpose**: Clone repositories and manage version control
- **Installation**:
  ```bash
  # macOS (using Homebrew)
  brew install git
  
  # Ubuntu/Debian
  sudo apt-get update
  sudo apt-get install git
  
  # Windows
  # Download from https://git-scm.com/download/win
  ```
- **Verification**: `git --version`

#### 2. Node.js
- **Purpose**: Run the combined whisper + NLP server
- **Version**: 16.x or higher
- **Installation**:
  ```bash
  # macOS (using Homebrew)
  brew install node
  
  # Windows
  # Download from https://nodejs.org/
  ```
- **Verification**: `node --version` and `npm --version`

#### 3. Python
- **Purpose**: Run Rasa NLU for natural language processing
- **Version**: 3.8 or higher
- **Installation**:
  ```bash
  # macOS (using Homebrew)
  brew install python@3.11
  
  # Windows
  # Download from https://www.python.org/downloads/
  ```
- **Verification**: `python3 --version` and `pip3 --version`

#### 4. Development Tools
- **C++ Compiler**: Required for compiling whisper.cpp
  ```bash
  # macOS
  xcode-select --install
  
  # Windows
  # Install Visual Studio Build Tools
  ```

### Verification Commands
```bash
# Verify all prerequisites are installed
git --version
node --version
npm --version
python3 --version
pip3 --version
gcc --version  # or clang --version on macOS
```

### System Requirements
- **RAM**: Minimum 4GB, recommended 8GB+ for model loading
- **Storage**: 2GB+ free space for models and dependencies

### Whisper Setup

1. Clone whisper.cpp:
```
git clone https://github.com/ggerganov/whisper.cpp
cd whisper.cpp && make && cd ..
```

2. Download model:
```
mkdir -p model
curl -L -o model/ggml-base.en.bin https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-base.en.bin
```