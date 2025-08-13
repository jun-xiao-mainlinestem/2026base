import { ConnectionStatus, CommandType, CommandHistory } from './types';

export class UIManager {
  private connectionStatusElement: HTMLElement;
  private commandFormElement: HTMLElement;
  private commandHistoryElement: HTMLElement;
  private connectionFormElement: HTMLElement;

  constructor() {
    // Note: connection-status element was removed from HTML
    this.connectionStatusElement = null as any;
    this.commandFormElement = document.getElementById('command-form')!;
    this.commandHistoryElement = document.getElementById('command-history')!;
    this.connectionFormElement = document.getElementById('connection-form')!;
  }



  showConnectionForm(): void {
    if (this.connectionFormElement) {
      this.connectionFormElement.style.display = 'block';
    }
    if (this.commandFormElement) {
      this.commandFormElement.style.display = 'none';
    }
    if (this.commandHistoryElement) {
      this.commandHistoryElement.style.display = 'none';
    }
  }

  showWebSocketURL(ipAddress: string, deviceId: string): void {
    const url = `ws://${ipAddress}:7071/vexrobotics.vexcode/device?id=${deviceId}`;
    
    // Create or update the URL display
    let urlDisplay = document.getElementById('websocket-url-display');
    if (!urlDisplay) {
      urlDisplay = document.createElement('div');
      urlDisplay.id = 'websocket-url-display';
      urlDisplay.className = 'websocket-url-display';
      
      // Insert after the connection form
      const connectionForm = document.getElementById('connection-form');
      if (connectionForm) {
        connectionForm.appendChild(urlDisplay);
      }
    }
    
    urlDisplay.innerHTML = `
      <div class="url-info">
        <strong>Connecting to:</strong>
        <code class="url-code">${url}</code>
        <div class="connection-status connecting">🔄 Connecting...</div>
      </div>
    `;
  }

  showURLPreview(ipAddress: string, deviceId: string): void {
    const urlPreview = document.getElementById('url-preview');
    const urlPreviewCode = document.getElementById('url-preview-code');
    
    if (urlPreview && urlPreviewCode) {
      const url = `ws://${ipAddress}:7071/vexrobotics.vexcode/device?id=${deviceId}`;
      urlPreviewCode.textContent = url;
      urlPreview.style.display = 'block';
    }
  }

  hideURLPreview(): void {
    const urlPreview = document.getElementById('url-preview');
    if (urlPreview) {
      urlPreview.style.display = 'none';
    }
  }

  hideConnectionForm(): void {
    if (this.connectionFormElement) {
      this.connectionFormElement.style.display = 'none';
    }
    
    // Remove the WebSocket URL display when hiding the form
    const urlDisplay = document.getElementById('websocket-url-display');
    if (urlDisplay) {
      urlDisplay.remove();
    }
    
    // Hide URL preview when hiding connection form
    this.hideURLPreview();
  }

  showCommandForm(): void {
    if (this.commandFormElement) {
      this.commandFormElement.style.display = 'block';
    }
    if (this.commandHistoryElement) {
      this.commandHistoryElement.style.display = 'block';
    }
    // Initialize with drive command form
    this.updateCommandForm(CommandType.DRIVE);
  }

  hideCommandForm(): void {
    if (this.commandFormElement) {
      this.commandFormElement.style.display = 'none';
    }
    if (this.commandHistoryElement) {
      this.commandHistoryElement.style.display = 'none';
    }
  }

  updateCommandForm(commandType: CommandType): void {
    const form = document.getElementById('command-params') as HTMLElement;
    if (!form) return;

    // Clear existing form
    form.innerHTML = '';

    switch (commandType) {
      case CommandType.DRIVE:
        this.createDriveForm(form);
        break;
      case CommandType.TURN:
        this.createTurnForm(form);
        break;
      case CommandType.SET_HEADING:
        this.createSetHeadingForm(form);
        break;
    }
  }

  private createDriveForm(container: HTMLElement): void {
    container.innerHTML = `
      <div class="form-group">
        <label for="distance">Distance (inches):</label>
        <input type="number" id="distance" value="0" min="-100" max="100" step="1" inputmode="text">
        <small class="form-help">Negative for backward, positive for forward</small>
      </div>
    `;
  }

  private createTurnForm(container: HTMLElement): void {
    container.innerHTML = `
      <div class="form-group">
        <label for="heading">Heading (0-360°):</label>
        <input type="number" id="heading" value="0" min="0" max="360" step="1" inputmode="numeric">
      </div>
    `;
  }

  private createSetHeadingForm(container: HTMLElement): void {
    container.innerHTML = `
      <div class="form-group">
        <label for="heading">Heading (0-360°):</label>
        <input type="number" id="heading" value="0" min="0" max="360" step="1" inputmode="numeric">
      </div>
    `;
  }

  getCommandParameters(commandType: CommandType): any {
    switch (commandType) {
      case CommandType.DRIVE:
        return {
          distance: parseInt((document.getElementById('distance') as HTMLInputElement)?.value || '0')
        };
      case CommandType.TURN:
        return {
          heading: parseInt((document.getElementById('heading') as HTMLInputElement)?.value || '0')
        };
      case CommandType.SET_HEADING:
        return {
          heading: parseInt((document.getElementById('heading') as HTMLInputElement)?.value || '0')
        };
      default:
        return {};
    }
  }

  addCommandToHistory(command: string, success: boolean): void {
    if (!this.commandHistoryElement) return;

    const historyItem = document.createElement('div');
    historyItem.className = `history-item ${success ? 'success' : 'error'}`;
    
    const timestamp = new Date().toLocaleTimeString();
    historyItem.innerHTML = `
      <span class="timestamp">${timestamp}</span>
      <span class="command">${command.trim()}</span>
      <span class="status">${success ? '✓' : '✗'}</span>
    `;

    // Find the history container within the command history section
    const historyContainer = this.commandHistoryElement.querySelector('.history-container');
    if (historyContainer) {
      historyContainer.insertBefore(historyItem, historyContainer.firstChild);

      // Keep only last 10 commands
      const items = historyContainer.querySelectorAll('.history-item');
      if (items.length > 10) {
        items[items.length - 1].remove();
      }
    }
  }

  showError(message: string): void {
    // Create a simple error notification
    const errorDiv = document.createElement('div');
    errorDiv.className = 'error-notification';
    errorDiv.textContent = message;
    
    document.body.appendChild(errorDiv);
    
    // Remove after 3 seconds
    setTimeout(() => {
      if (errorDiv.parentNode) {
        errorDiv.parentNode.removeChild(errorDiv);
      }
    }, 3000);
  }

  showConnectionError(message: string): void {
    // Show error notification but keep the URL visible
    this.showError(message);
    
    // Update the URL display to show it's a failed connection
    const urlDisplay = document.getElementById('websocket-url-display');
    if (urlDisplay) {
      urlDisplay.classList.add('connection-failed');
      const urlInfo = urlDisplay.querySelector('.url-info');
      if (urlInfo) {
        const statusElement = urlInfo.querySelector('.connection-status');
        if (statusElement) {
          // Update existing status element
          statusElement.className = 'connection-status error';
          statusElement.textContent = '❌ Connection Failed';
        }
      }
    }
  }

  showSuccess(message: string): void {
    // Create a simple success notification
    const successDiv = document.createElement('div');
    successDiv.className = 'success-notification';
    successDiv.textContent = message;
    
    document.body.appendChild(successDiv);
    
    // Remove after 3 seconds
    setTimeout(() => {
      if (successDiv.parentNode) {
        successDiv.parentNode.removeChild(successDiv);
      }
    }, 3000);
  }
}
