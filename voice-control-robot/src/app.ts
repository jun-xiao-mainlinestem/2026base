import './styles.css';
import { VEXWebSocket } from './websocket';
import { SpeechRecognition } from './speech';
import { CommandProcessor } from './commands';
import { VoiceCommand, RobotCommand, RobotAction, RobotStatus } from './types';

export class VoiceControlApp {
  private websocket: VEXWebSocket;
  private speech: SpeechRecognition;
  private commandProcessor: CommandProcessor;
  private isConnected: boolean = false;

  constructor() {
    // Configuration - update with your VEX device ID and port
    this.websocket = new VEXWebSocket({
      id: "51583", // Replace with your device ID
      port: "7071",
      ip: "localhost"
    });

    this.speech = new SpeechRecognition();
    this.commandProcessor = new CommandProcessor();
    
    this.setupEventHandlers();
    this.initializeUI();
  }

  private setupEventHandlers(): void {
    // WebSocket events
    this.websocket.onStatusChange((connected) => {
      this.isConnected = connected;
      this.updateConnectionStatus(connected);
    });

    this.websocket.onMessage((data) => {
      this.displayMessage(`Robot: ${data}`);
      this.parseRobotStatus(data);
    });

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

    const commandString = this.formatRobotCommand(robotCommand);
    this.websocket.send(commandString);
    this.displayMessage(`Executing: ${robotCommand.action}`);
  }

  private formatRobotCommand(robotCommand: RobotCommand): string {
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
        return 'INTAKE';
      case RobotAction.SCORE:
        return 'SCORE';
      default:
        return 'STOP';
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
    // Connect button
    const connectBtn = document.getElementById('connect-btn') as HTMLButtonElement;
    if (connectBtn) {
      connectBtn.addEventListener('click', async () => {
        try {
          this.displayMessage('Attempting to connect to VEX Extension...');
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