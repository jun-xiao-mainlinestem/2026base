import './styles.css';
import { VEXWebSocket } from './websocket';
import { SpeechRecognition } from './speech';
import { CommandProcessor } from './commands';
import { VoiceCommand, RobotCommand, RobotAction, RobotStatus } from './types';

export class VoiceControlApp {
  private websocket: VEXWebSocket | null = null;
  private speech: SpeechRecognition;
  private commandProcessor: CommandProcessor;
  private isConnected: boolean = false;
  private deviceId: string = "";

  constructor() {
    this.speech = new SpeechRecognition();
    this.commandProcessor = new CommandProcessor();
    
    this.setupEventHandlers();
    this.initializeUI();
  }

  private setupEventHandlers(): void {
    // Speech recognition events
    this.speech.onResult((voiceCommand) => {
      this.handleVoiceCommand(voiceCommand);
    });

    this.speech.onError((error) => {
      this.displayError(`Speech error: ${error}`);
    });
  }

  private async handleVoiceCommand(voiceCommand: VoiceCommand): Promise<void> {
    console.log('Voice command received:', voiceCommand);
    this.displayMessage(`Heard: "${voiceCommand.command}"`);
    
    const robotCommand = this.commandProcessor.processVoiceCommand(voiceCommand);
    console.log('Processed robot command:', robotCommand);
    
    if (robotCommand) {
      this.executeRobotCommand(robotCommand);
    } else {
      this.displayError(`Unknown command: "${voiceCommand.command}"`);
    }
  }

  private executeRobotCommand(robotCommand: RobotCommand): void {
    if (!this.isConnected) {
      this.displayError('Not connected to robot');
      return;
    }

    const commandChar = this.formatRobotCommand(robotCommand);
    this.websocket?.send(commandChar);
    this.displayMessage(`Executing: ${robotCommand.action} (char: '${commandChar}')`);
  }

  private formatRobotCommand(robotCommand: RobotCommand): string {
    // Map RobotAction to text command
    switch (robotCommand.action) {
      case RobotAction.FORWARD:
        return 'FORWARD';
      case RobotAction.BACKWARD:
        return 'BACKWARD';
      case RobotAction.LEFT:
        return 'LEFT';
      case RobotAction.RIGHT:
        return 'RIGHT';
      case RobotAction.STOP:
        return 'STOP';
      case RobotAction.INTAKE:
        return 'ROLL';
      case RobotAction.SCORE:
        return 'SHOOT';
      default:
        return 'STOP';
    }
  }

  private updateConnectionStatus(connected: boolean): void {
    const statusElement = document.getElementById('connection-status');
    const listenBtn = document.getElementById('listen-btn') as HTMLButtonElement;
    
    if (statusElement) {
      statusElement.textContent = connected ? '🟢 Connected' : '🔴 Disconnected';
      statusElement.className = connected ? 'status connected' : 'status disconnected';
    }
    
    if (listenBtn) {
      listenBtn.disabled = !connected;
      if (!connected) {
        // If disconnected, also stop listening and reset button
        this.speech.stop();
        listenBtn.textContent = 'Start Listening';
        listenBtn.className = 'btn btn-success';
        this.updateListeningStatus(false);
      }
    }
  }

  private displayMessage(message: string): void {
    const outputElement = document.getElementById('output');
    if (outputElement) {
      const timestamp = new Date().toLocaleTimeString();
      outputElement.innerHTML += `<div>[${timestamp}] ${message}</div>`;
      outputElement.scrollTop = outputElement.scrollHeight;
    }
  }

  private displayError(error: string): void {
    this.displayMessage(`❌ ${error}`);
  }

  private updateListeningStatus(isListening: boolean): void {
    const statusElement = document.getElementById('listening-status');
    if (statusElement) {
      if (isListening) {
        statusElement.textContent = '🎤 Listening for voice commands...';
        statusElement.className = 'listening-status listening';
      } else {
        statusElement.textContent = '🔇 Not listening';
        statusElement.className = 'listening-status';
      }
    }
  }

  private displayCommandList(): void {
    const outputElement = document.getElementById('output');
    if (outputElement) {
      const timestamp = new Date().toLocaleTimeString();
      const commandList = `
        <div class="command-list-display">
          <strong>📋 Available Commands:</strong><br>
          <span class="command-tag">move</span>
          <span class="command-tag">back</span>
          <span class="command-tag">left</span>
          <span class="command-tag">right</span>
          <span class="command-tag">stop</span>
          <span class="command-tag">roll</span>
          <span class="command-tag">shoot</span>
        </div>
      `;
      outputElement.innerHTML += `<div>[${timestamp}] ${commandList}</div>`;
      outputElement.scrollTop = outputElement.scrollHeight;
    }
  }

  private clearCommandList(): void {
    const outputElement = document.getElementById('output');
    if (outputElement) {
      // Remove command list entries (entries that contain "Available Commands:")
      const entries = outputElement.querySelectorAll('div');
      entries.forEach(entry => {
        if (entry.innerHTML.includes('Available Commands:')) {
          entry.remove();
        }
      });
    }
  }

  private parseRobotStatus(data: string): void {
    console.log('parseRobotStatus called with data:', data);
    console.log('Data type:', typeof data);
    console.log('Data length:', data.length);
    
    // Ensure data is a string
    if (typeof data !== 'string') {
      console.warn('Received non-string data:', data);
      return;
    }
    
    // Parse status messages from robot
    if (data.includes('STATUS:')) {
      console.log('Found STATUS in data, parsing...');
      const parts = data.split(':');
      console.log('Split parts:', parts);
      if (parts.length >= 3) {
        const heading = parts[1].trim();
        const distance = parts[2].trim();
        console.log('Extracted heading:', heading, 'distance:', distance);
        
        const headingElement = document.getElementById('current-heading');
        const distanceElement = document.getElementById('distance-driven');
        
        console.log('Heading element found:', !!headingElement);
        console.log('Distance element found:', !!distanceElement);
        
        if (headingElement) {
          headingElement.textContent = heading;
          console.log('Updated heading element to:', heading);
        }
        if (distanceElement) {
          distanceElement.textContent = distance;
          console.log('Updated distance element to:', distance);
        }
      } else {
        console.warn('STATUS message has insufficient parts:', parts);
      }
    } else {
      console.log('No STATUS found in data');
    }
  }

  private initializeUI(): void {
    // Device ID input
    const deviceIdInput = document.getElementById('device-id-input') as HTMLInputElement;
    const connectBtn = document.getElementById('connect-btn') as HTMLButtonElement;
    if (deviceIdInput && connectBtn) {
      connectBtn.disabled = true;
      deviceIdInput.addEventListener('input', () => {
        this.deviceId = deviceIdInput.value.trim();
        connectBtn.disabled = this.deviceId.length === 0;
      });
      
      // Add click event listener for connect button
      connectBtn.addEventListener('click', async () => {
        try {
          this.displayMessage('Attempting to connect to VEX Extension...');
          this.websocket = new VEXWebSocket({
            id: this.deviceId,
            port: "7071",
            ip: "localhost"
          });
          this.setupWebSocketHandlers(); // Setup handlers after websocket is initialized
          await this.websocket.connect();
          this.displayMessage('✅ Successfully connected to VEX Extension!');
        } catch (error) {
          this.displayError(`Connection failed: ${error}`);
          this.displayMessage('💡 Make sure VEX Extension is running and WebSocket Server is enabled');
        }
      });
    }

    // Start/Stop listening button
    const listenBtn = document.getElementById('listen-btn') as HTMLButtonElement;
    if (listenBtn) {
      // Initially disable since we're not connected
      listenBtn.disabled = true;
      
      listenBtn.addEventListener('click', () => {
        if (listenBtn.textContent === 'Start Listening') {
          this.speech.start();
          listenBtn.textContent = 'Stop Listening';
          listenBtn.className = 'btn btn-danger';
          this.updateListeningStatus(true);
          this.displayMessage('🎤 Voice recognition started - speak commands continuously');
          this.displayCommandList();
        } else {
          this.speech.stop();
          listenBtn.textContent = 'Start Listening';
          listenBtn.className = 'btn btn-success';
          this.updateListeningStatus(false);
          this.displayMessage('🔇 Voice recognition stopped');
          this.clearCommandList();
        }
      });
    }

    // Clear output button
    const clearBtn = document.getElementById('clear-btn') as HTMLButtonElement;
    if (clearBtn) {
      clearBtn.addEventListener('click', () => {
        const outputElement = document.getElementById('output');
        if (outputElement) {
          outputElement.innerHTML = '';
        }
      });
    }
  }

  private setupWebSocketHandlers(): void {
    if (!this.websocket) return;
    this.websocket.onStatusChange((connected) => {
      this.isConnected = connected;
      this.updateConnectionStatus(connected);
    });
    this.websocket.onMessage((data) => {
      this.displayMessage(`Robot: ${data}`);
      this.parseRobotStatus(data);
    });
  }

  async start(): Promise<void> {
    // Don't auto-connect on startup - let user click connect button
    this.displayMessage('Voice control system ready. Click "Connect to Robot" to start.');
  }
}

// Initialize app when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
  const app = new VoiceControlApp();
  app.start();
}); 