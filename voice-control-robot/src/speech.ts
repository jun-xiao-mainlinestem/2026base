import { VoiceCommand } from './types';

export class SpeechRecognition {
  private recognition: any;
  private isListening: boolean = false;
  private onResultCallback?: (command: VoiceCommand) => void;
  private onErrorCallback?: (error: string) => void;

  constructor() {
    if ('webkitSpeechRecognition' in window || 'SpeechRecognition' in window) {
      const SpeechRecognition = (window as any).SpeechRecognition || 
                                (window as any).webkitSpeechRecognition;
      this.recognition = new SpeechRecognition();
      this.setupRecognition();
    } else {
      throw new Error('Speech recognition not supported');
    }
  }

  private setupRecognition(): void {
    this.recognition.continuous = false;
    this.recognition.interimResults = false;
    this.recognition.lang = 'en-US';
    this.recognition.maxAlternatives = 1;

    this.recognition.onresult = (event: any) => {
      const result = event.results[0];
      const command: VoiceCommand = {
        command: result[0].transcript.toLowerCase(),
        confidence: result[0].confidence,
        timestamp: new Date()
      };
      
      this.onResultCallback?.(command);
    };

    this.recognition.onerror = (event: any) => {
      this.onErrorCallback?.(event.error);
    };

    this.recognition.onend = () => {
      // Only restart if we're supposed to be listening
      if (this.isListening) {
        console.log('Speech recognition ended, restarting...');
        this.recognition.start();
      }
    };
  }

  start(): void {
    if (!this.isListening) {
      this.isListening = true;
      this.recognition.start();
    }
  }

  stop(): void {
    this.isListening = false;
    this.recognition.stop();
    console.log('Speech recognition stopped by user');
  }

  onResult(callback: (command: VoiceCommand) => void): void {
    this.onResultCallback = callback;
  }

  onError(callback: (error: string) => void): void {
    this.onErrorCallback = callback;
  }

  getListeningStatus(): boolean {
    return this.isListening;
  }
} 