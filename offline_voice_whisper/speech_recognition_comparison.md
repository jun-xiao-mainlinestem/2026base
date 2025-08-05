# Speech Recognition Integration for Mobile Web Robot Controller

This document outlines five potential approaches for incorporating speech recognition into a mobile web app for controlling a robot (e.g., recognizing commands like "drive forward 12 inches"). Each approach is described in terms of workflow, followed by a comparative analysis of key criteria: recognition reliability, speed, ease of integration, security, and cost.

---

## Option 1: Web Speech API + Regular Expressions

### Workflow:
1. JavaScript in the browser uses the Web Speech API (`webkitSpeechRecognition`).
2. When the user taps the mic button, speech recognition starts.
3. The recognized transcript is returned to the client.
4. JavaScript uses regular expressions (e.g., `/drive forward (\d+) inches/`) to extract command intent and parameters.
5. Command is sent to the robot via WebSocket or HTTP.

### Requirements:
- Runs entirely in the browser
- No backend needed

---

## Option 2: Whisper Local Model + RASA NLP (Local Installation)

### Workflow:
1. Client records audio using MediaRecorder API.
2. Audio is sent to a local server running Whisper (e.g., via Python or Node.js backend).
3. Whisper transcribes the audio to text.
4. Transcribed text is passed to a locally hosted RASA NLU model.
5. RASA parses intent (e.g., `drive`) and entities (e.g., `12`, `inches`).
6. Command is sent to the robot.

### Requirements:
- Backend server (local or LAN-hosted)
- Whisper model + RASA installed locally

---

## Option 3: Google Cloud Speech-to-Text API

### Workflow:
1. Audio recorded on client (web).
2. Sent to a backend or directly (via OAuth-secured HTTPS) to Google's Speech API.
3. Google transcribes the text.
4. Your backend processes the text using rules or an NLP tool (like Dialogflow or regex).
5. Command sent to robot.

### Requirements:
- Google Cloud project setup
- API key management
- Backend or secure client code

---

## Option 4: Picovoice SDK

### Workflow:
1. Use Picovoice's Porcupine (wake word) and Rhino (command inference) engines.
2. Speech is processed locally in the browser using WASM and models trained with Picovoice Console.
3. Extracted intent and parameters are returned directly.
4. Command sent to the robot.

### Requirements:
- No backend needed
- Requires compiling model with Picovoice Console

---

## Option 5: Microsoft Azure Speech SDK (JavaScript)

### Workflow:
1. Use Azure Cognitive Services Speech SDK in the browser.
2. Audio streamed to Microsoft Azure for transcription and/or intent detection.
3. Transcribed result returned via SDK.
4. Use regex or client-side NLP to extract command.
5. Send command to robot.

### Requirements:
- Azure account + API key
- Works in browser with no custom backend

---

## Comparative Analysis

| Feature                       | Web Speech API           | Whisper + RASA               | Google Speech API         | Picovoice SDK               | Azure Speech SDK           |
|------------------------------|---------------------------|-------------------------------|---------------------------|-----------------------------|----------------------------|
| **Recognition Reliability** | Medium (Chrome-only)     | High (state-of-art)          | High (Google-grade)       | High (command-level)       | High                       |
| **Speed (Latency)**         | Low                      | Medium (local processing)     | Medium-High               | Low (local processing)     | Medium                     |
| **Ease of Integration**     | Easy (1 file JS)         | Complex (Whisper + RASA)      | Moderate (OAuth, backend) | Moderate (model training)  | Easy (JS SDK, no backend)  |
| **Security**                | Medium (browser only)    | High (offline/local)          | Medium (needs secure key) | High (fully offline)       | Medium (cloud account)     |
| **Cost**                    | Free                     | Free (offline)                | Paid (after free tier)    | Free for small projects    | Paid (with free tier)      |
| **Mobile Support**          | Android only (Chrome)    | Full (if local server used)   | Full                      | Full (WASM)                | Full                       |

---

## Summary Recommendations:
- For **fast prototyping and Android-only web apps**: Use **Web Speech API**.
- For **privacy and full offline control**: Use **Whisper + RASA** or **Picovoice**.
- For **enterprise-grade transcription**: Use **Azure Speech SDK** (if avoiding backend) or **Google Speech API** (if backend is acceptable).

Would you like code samples or a prototype template for any of these?

