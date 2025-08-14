import { ConnectionConfig, ConnectionStatus } from './types';

export class WebSocketManager {
  private socket: WebSocket | null = null;
  private connectionStatus: ConnectionStatus = {
    connected: false,
    connecting: false
  };
  private onStatusChange: ((status: ConnectionStatus) => void) | null = null;
  private reconnectAttempts = 0;
  private maxReconnectAttempts = 5;
  private reconnectDelay = 1000;

  constructor(onStatusChange?: (status: ConnectionStatus) => void) {
    this.onStatusChange = onStatusChange || null;
  }

  connect(config: ConnectionConfig): Promise<void> {
    return new Promise((resolve, reject) => {
      if (this.socket && this.socket.readyState === WebSocket.OPEN) {
        resolve();
        return;
      }

      this.updateStatus({ connected: false, connecting: true, error: undefined });

              try {
          const wsUrl = `ws://localhost:7071/vexrobotics.vexcode/device?id=${config.deviceId}`;
          this.socket = new WebSocket(wsUrl);

        this.socket.onopen = () => {
          this.reconnectAttempts = 0;
          this.updateStatus({ connected: true, connecting: false, error: undefined });
          resolve();
        };

        this.socket.onclose = (event) => {
          this.updateStatus({ connected: false, connecting: false, error: 'Connection closed' });
          
          if (!event.wasClean && this.reconnectAttempts < this.maxReconnectAttempts) {
            this.attemptReconnect(config);
          }
        };

        this.socket.onerror = (error) => {
          this.updateStatus({ 
            connected: false, 
            connecting: false, 
            error: 'Connection error' 
          });
          reject(new Error('WebSocket connection failed'));
        };

        this.socket.onmessage = (event) => {
          // Handle any messages from the server if needed
          console.log('Received message:', event.data);
        };

      } catch (error) {
        this.updateStatus({ 
          connected: false, 
          connecting: false, 
          error: 'Failed to create connection' 
        });
        reject(error);
      }
    });
  }

  private attemptReconnect(config: ConnectionConfig): void {
    this.reconnectAttempts++;
    setTimeout(() => {
      if (this.reconnectAttempts < this.maxReconnectAttempts) {
        this.connect(config).catch(() => {
          // Reconnection failed, will try again
        });
      }
    }, this.reconnectDelay * this.reconnectAttempts);
  }

  disconnect(): void {
    if (this.socket) {
      this.socket.close();
      this.socket = null;
    }
    this.updateStatus({ connected: false, connecting: false, error: undefined });
  }

  sendCommand(command: string): boolean {
    if (!this.socket || this.socket.readyState !== WebSocket.OPEN) {
      return false;
    }

    try {
      this.socket.send(command);
      return true;
    } catch (error) {
      console.error('Failed to send command:', error);
      return false;
    }
  }

  private updateStatus(status: ConnectionStatus): void {
    this.connectionStatus = status;
    if (this.onStatusChange) {
      this.onStatusChange(status);
    }
  }

  getStatus(): ConnectionStatus {
    return { ...this.connectionStatus };
  }

  isConnected(): boolean {
    return this.connectionStatus.connected;
  }
}
