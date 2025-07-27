import { WebSocketConfig } from './types';

export class VEXWebSocket {
  private socket: WebSocket | null = null;
  private config: WebSocketConfig;
  private onMessageCallback?: (data: string) => void;
  private onStatusChangeCallback?: (connected: boolean) => void;

  constructor(config: WebSocketConfig) {
    this.config = config;
  }

  connect(): Promise<void> {
    return new Promise((resolve, reject) => {
      const url = `ws://${this.config.ip}:${this.config.port}/vexrobotics.vexcode/device?id=${this.config.id}`;
      
      console.log('Attempting to connect to:', url);
      this.socket = new WebSocket(url);
      
      this.socket.onopen = () => {
        console.log('WebSocket connected successfully');
        this.onStatusChangeCallback?.(true);
        resolve();
      };
      
      this.socket.onmessage = (event) => {
        console.log('WebSocket message received:', event.data);
        if (this.onMessageCallback) {
          this.onMessageCallback(event.data);
        }
      };
      
      this.socket.onerror = (error) => {
        console.error('WebSocket error:', error);
        // Don't reject immediately, let onclose handle it
      };
      
      this.socket.onclose = (event) => {
        console.log('WebSocket disconnected. Code:', event.code, 'Reason:', event.reason);
        this.onStatusChangeCallback?.(false);
        if (event.code !== 1000) { // Not a normal closure
          reject(new Error(`WebSocket closed with code ${event.code}: ${event.reason}`));
        }
      };
    });
  }

  send(message: string): void {
    if (this.socket && this.socket.readyState === WebSocket.OPEN) {
      this.socket.send(message);
    } else {
      console.error('WebSocket not connected');
    }
  }

  disconnect(): void {
    if (this.socket) {
      this.socket.close();
      this.socket = null;
    }
  }

  onMessage(callback: (data: string) => void): void {
    this.onMessageCallback = callback;
  }

  onStatusChange(callback: (connected: boolean) => void): void {
    this.onStatusChangeCallback = callback;
  }

  isConnected(): boolean {
    return this.socket?.readyState === WebSocket.OPEN;
  }
} 