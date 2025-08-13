import { WebSocketManager } from './websocket';
import { UIManager } from './ui';
import { CommandValidator, CommandFormatter } from './commands';
import { ConnectionConfig, CommandType, ValidCommand } from './types';
import './styles.css';

export class RemoteControlApp {
  private websocketManager: WebSocketManager;
  private uiManager: UIManager;
  private currentCommandType: CommandType = CommandType.DRIVE;

  constructor() {
    this.uiManager = new UIManager();
    this.websocketManager = new WebSocketManager(() => {
      // Connection status updates are no longer needed
    });

    this.initializeEventListeners();
    this.uiManager.showConnectionForm();
  }

  private initializeEventListeners(): void {
    // Connection form submission
    const connectionForm = document.getElementById('connection-form') as HTMLFormElement;
    if (connectionForm) {
      connectionForm.addEventListener('submit', (e) => this.handleConnectionSubmit(e));
    }

    // IP address and device ID input changes for URL preview
    const ipAddressInput = document.getElementById('ipAddress') as HTMLInputElement;
    const deviceIdInput = document.getElementById('deviceId') as HTMLInputElement;
    
    if (ipAddressInput && deviceIdInput) {
      const updateURLPreview = () => {
        const ip = ipAddressInput.value.trim();
        const id = deviceIdInput.value.trim();
        if (ip && id) {
          this.uiManager.showURLPreview(ip, id);
        } else {
          this.uiManager.hideURLPreview();
        }
      };
      
      ipAddressInput.addEventListener('input', updateURLPreview);
      deviceIdInput.addEventListener('input', updateURLPreview);
    }

    // Command type selection
    const commandTypeSelect = document.getElementById('command-type') as HTMLSelectElement;
    if (commandTypeSelect) {
      commandTypeSelect.addEventListener('change', (e) => this.handleCommandTypeChange(e));
    }

    // Command form submission
    const commandForm = document.getElementById('command-form') as HTMLFormElement;
    if (commandForm) {
      commandForm.addEventListener('submit', (e) => this.handleCommandSubmit(e));
    }
  }

  private async handleConnectionSubmit(event: Event): Promise<void> {
    event.preventDefault();
    
    const form = event.target as HTMLFormElement;
    const formData = new FormData(form);
    
    const config: ConnectionConfig = {
      ipAddress: formData.get('ipAddress') as string,
      deviceId: formData.get('deviceId') as string
    };

    if (!config.ipAddress || !config.deviceId) {
      this.uiManager.showError('Please enter both IP address and device ID');
      return;
    }

    // Show the WebSocket URL before attempting connection
    this.uiManager.showWebSocketURL(config.ipAddress, config.deviceId);

    try {
      await this.websocketManager.connect(config);
      this.uiManager.hideConnectionForm();
      this.uiManager.hideURLPreview(); // Hide the URL preview
      this.uiManager.showCommandForm(); // Show the command form
    } catch (error) {
      // Keep the URL visible when connection fails
      this.uiManager.showConnectionError(`Connection failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
  }

  private handleCommandTypeChange(event: Event): void {
    const select = event.target as HTMLSelectElement;
    this.currentCommandType = select.value as CommandType;
    this.uiManager.updateCommandForm(this.currentCommandType);
  }

  private handleCommandSubmit(event: Event): Promise<void> {
    event.preventDefault();
    
    if (!this.websocketManager.isConnected()) {
      this.uiManager.showError('Not connected to robot');
      return Promise.resolve();
    }

    const params = this.uiManager.getCommandParameters(this.currentCommandType);
    let command: ValidCommand | null = null;

    // Validate command based on type
    switch (this.currentCommandType) {
      case CommandType.DRIVE:
        command = CommandValidator.validateDriveCommand(
          params.distance
        );
        break;
      case CommandType.TURN:
        command = CommandValidator.validateTurnCommand(
          params.heading
        );
        break;
      case CommandType.SET_HEADING:
        command = CommandValidator.validateSetHeadingCommand(params.heading);
        break;
    }

    if (!command) {
      this.uiManager.showError('Invalid command parameters');
      return Promise.resolve();
    }

    // Format and send command
    const commandString = CommandFormatter.formatCommand(command);
    const success = this.websocketManager.sendCommand(commandString);
    
    if (success) {
      this.uiManager.addCommandToHistory(commandString, true);
    } else {
      this.uiManager.addCommandToHistory(commandString, false);
      this.uiManager.showError('Failed to send command');
    }

    return Promise.resolve();
  }


}

// Initialize the application when the DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
  new RemoteControlApp();
});
