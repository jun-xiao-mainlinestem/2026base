import { RobotStatus, LogEvent, Point } from './types';
import { Robot } from './robot';

export class StatusLogger {
  private robot: Robot;
  private distanceTraveled: number = 0;
  private lastStopPosition: Point;
  private logEvents: LogEvent[] = [];
  private maxLogEvents: number = 100;

  constructor(robot: Robot) {
    this.robot = robot;
    const initialPos = robot.getPosition();
    this.lastStopPosition = { x: initialPos.x, y: initialPos.y };
  }

  public updateStatus(): void {
    // Update distance traveled since last stop
    const currentPos = this.robot.getPosition();
    const dx = currentPos.x - this.lastStopPosition.x;
    const dy = currentPos.y - this.lastStopPosition.y;
    this.distanceTraveled = Math.sqrt(dx * dx + dy * dy);
  }

  public resetDistance(): void {
    this.distanceTraveled = 0;
    this.lastStopPosition = this.robot.getPosition();
  }

  public getCurrentStatus(): RobotStatus {
    return {
      x: this.robot.getPosition().x,
      y: this.robot.getPosition().y,
      heading: this.robot.getHeading(),
      distanceTraveled: this.distanceTraveled,
      isMoving: this.robot.getStatus().isMoving
    };
  }

  public logEvent(event: LogEvent): void {
    this.logEvents.push(event);
    
    // Keep only the last maxLogEvents
    if (this.logEvents.length > this.maxLogEvents) {
      this.logEvents = this.logEvents.slice(-this.maxLogEvents);
    }
  }

  public logCommand(command: string): void {
    this.logEvent({
      type: 'command',
      message: `Command executed: ${command}`,
      timestamp: Date.now()
    });
  }

  public logMovement(distance: number, direction: string): void {
    this.logEvent({
      type: 'movement',
      message: `Moved ${distance.toFixed(1)} inches ${direction}`,
      timestamp: Date.now(),
      data: { distance, direction }
    });
  }

  public logStatus(status: RobotStatus): void {
    this.logEvent({
      type: 'status',
      message: `Position: (${status.x.toFixed(1)}, ${status.y.toFixed(1)}), Heading: ${status.heading.toFixed(1)}°, Distance: ${status.distanceTraveled.toFixed(1)} inches`,
      timestamp: Date.now(),
      data: status
    });
  }

  public logError(error: string): void {
    this.logEvent({
      type: 'error',
      message: `Error: ${error}`,
      timestamp: Date.now()
    });
  }

  public getLogEvents(): LogEvent[] {
    return [...this.logEvents];
  }

  public clearLog(): void {
    this.logEvents = [];
  }

  public getFormattedStatus(): string {
    const status = this.getCurrentStatus();
    return `Position: (${status.x.toFixed(1)}, ${status.y.toFixed(1)}) | Heading: ${status.heading.toFixed(1)}° | Distance: ${status.distanceTraveled.toFixed(1)} inches`;
  }

  public getFormattedLog(): string[] {
    return this.logEvents.map(event => {
      const time = new Date(event.timestamp).toLocaleTimeString();
      return `[${time}] ${event.message}`;
    });
  }
} 