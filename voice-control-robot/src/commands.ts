import { VoiceCommand, RobotCommand, RobotAction } from './types';

export class CommandProcessor {
  private commandMap: Map<string, RobotAction> = new Map([
    ['move', RobotAction.FORWARD],
    ['back', RobotAction.BACKWARD],
    ['left', RobotAction.LEFT],
    ['right', RobotAction.RIGHT],
    ['stop', RobotAction.STOP],
    ['roll', RobotAction.INTAKE],
    ['shoot', RobotAction.SCORE]
  ]);

  processVoiceCommand(voiceCommand: VoiceCommand): RobotCommand | null {
    console.log('Processing voice command:', voiceCommand.command);
    const words = voiceCommand.command.split(' ');
    console.log('Split words:', words);
    
    for (const word of words) {
      console.log('Checking word:', word);
      const action = this.commandMap.get(word);
      console.log('Found action:', action);
      if (action) {
        return {
          action,
          parameters: this.extractParameters(voiceCommand.command, action)
        };
      }
    }
    
    console.log('No matching command found');
    return null;
  }

  private extractParameters(command: string, action: RobotAction): any {
    // Extract additional parameters like distance, speed, etc.
    const numbers = command.match(/\d+/g);
    if (numbers && numbers.length > 0) {
      return { value: parseInt(numbers[0]) };
    }
    return {};
  }

  getSupportedCommands(): string[] {
    return Array.from(this.commandMap.keys());
  }

  isValidCommand(command: string): boolean {
    return this.commandMap.has(command.toLowerCase());
  }
} 