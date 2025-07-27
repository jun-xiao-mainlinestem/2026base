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
    this.displayMessage(`Heard: "${voiceCommand.command}"`);
    
    const robotCommand = this.commandProcessor.processVoiceCommand(voiceCommand);
    
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
    // Map RobotAction to single-character command
    switch (robotCommand.action) {
      case RobotAction.FORWARD:
        return 'a';
      case RobotAction.BACKWARD:
        return 'b';
      case RobotAction.LEFT:
        return 'l';
      case RobotAction.RIGHT:
        return 'd';
      case RobotAction.STOP:
        return 'p';
      case RobotAction.INTAKE:
        return 'i';
      case RobotAction.SCORE:
        return 's';
      default:
        return 'p';
    }
  }

  private updateConnectionStatus(connected: boolean): void {
    const statusElement = document.getElementById('connection-status');
    if (statusElement) {
      statusElement.textContent = connected ? '🟢 Connected' : '🔴 Disconnected';
      statusElement.className = connected ? 'status connected' : 'status disconnected';
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

  private parseRobotStatus(data: string): void {
    // Parse status messages from robot
    if (data.includes('STATUS:')) {
      const parts = data.split(':');
      if (parts.length >= 3) {
        const heading = parts[1].trim();
        const distance = parts[2].trim();
        
        const headingElement = document.getElementById('current-heading');
        const distanceElement = document.getElementById('distance-driven');
        
        if (headingElement) headingElement.textContent = heading;
        if (distanceElement) distanceElement.textContent = distance;
      }
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
      listenBtn.addEventListener('click', () => {
        if (listenBtn.textContent === 'Start Listening') {
          this.speech.start();
          listenBtn.textContent = 'Stop Listening';
          listenBtn.className = 'btn btn-danger';
          this.updateListeningStatus(true);
          this.displayMessage('🎤 Voice recognition started - speak commands continuously');
        } else {
          this.speech.stop();
          listenBtn.textContent = 'Start Listening';
          listenBtn.className = 'btn btn-success';
          this.updateListeningStatus(false);
          this.displayMessage('🔇 Voice recognition stopped');
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