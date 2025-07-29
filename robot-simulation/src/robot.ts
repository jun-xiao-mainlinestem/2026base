import { Point, RobotStatus, MovementConfig } from './types';

export class Robot {
  private x: number;        // X position in inches
  private y: number;        // Y position in inches
  private heading: number;  // Heading in degrees (0 = North, 90 = East)
  private length: number;   // Length in inches (17")
  private width: number;    // Width in inches (15")
  private isMoving: boolean = false;
  private movementConfig: MovementConfig;
  private field: any; // Reference to field for coordinate conversion

  constructor(field: any, initialX: number = 120, initialY: number = 120, initialHeading: number = 0) {
    this.field = field;
    this.x = initialX;
    this.y = initialY;
    this.heading = initialHeading;
    this.length = 15; // 15 inches length (front-to-back)
    this.width = 17;  // 17 inches width (side-to-side)
    
    this.movementConfig = {
      forwardSpeed: 0.1,    // inches per frame (much slower)
      backwardSpeed: 0.08,  // inches per frame (much slower)
      turnSpeed: 0.5,       // degrees per frame (continuous, much slower)
      turn90Speed: 90,      // degrees total for 90-degree turns
      animationFPS: 60
    };
  }

  public setPosition(x: number, y: number): void {
    // Ensure robot stays within field boundaries
    const halfLength = this.length / 2;
    const halfWidth = this.width / 2;
    this.x = Math.max(halfLength, Math.min(144 - halfLength, x));
    this.y = Math.max(halfWidth, Math.min(144 - halfWidth, y));
  }

  public setHeading(heading: number): void {
    // Normalize heading to 0-360 degrees
    this.heading = ((heading % 360) + 360) % 360;
  }

  public getPosition(): Point {
    return { x: this.x, y: this.y };
  }

  public getHeading(): number {
    return this.heading;
  }

  public isAtBoundary(): boolean {
    const halfLength = this.length / 2;
    const halfWidth = this.width / 2;
    return this.x <= halfLength || this.x >= 144 - halfLength || 
           this.y <= halfWidth || this.y >= 144 - halfWidth;
  }

  public move(distance: number): void {
    if (distance === 0) return;

    const radians = (this.heading * Math.PI) / 180;
    const newX = this.x + Math.sin(radians) * distance;
    const newY = this.y - Math.cos(radians) * distance;

    // Check if new position would be within bounds
    const halfLength = this.length / 2;
    const halfWidth = this.width / 2;
    if (newX >= halfLength && newX <= 144 - halfLength && 
        newY >= halfWidth && newY <= 144 - halfWidth) {
      this.x = newX;
      this.y = newY;
    }
  }

  public turn(angle: number): void {
    this.heading = ((this.heading + angle) % 360 + 360) % 360;
  }

  public draw(ctx: CanvasRenderingContext2D): void {
    // Convert field coordinates to screen coordinates
    const screenPos = this.field.fieldToScreen(this.x, this.y);
    
    // Calculate robot dimensions in screen coordinates
    // Get the actual field screen dimensions and calculate robot size proportionally
    const fieldDimensions = this.field.getDimensions();
    const fieldScreenPos1 = this.field.fieldToScreen(0, 0);
    const fieldScreenPos2 = this.field.fieldToScreen(fieldDimensions.width, fieldDimensions.height);
    const fieldScreenWidth = fieldScreenPos2.x - fieldScreenPos1.x;
    const screenLength = (this.length / fieldDimensions.width) * fieldScreenWidth;
    const screenWidth = (this.width / fieldDimensions.width) * fieldScreenWidth;

    // Save context state
    ctx.save();

    // Move to robot center and rotate
    ctx.translate(screenPos.x, screenPos.y);
    ctx.rotate((this.heading * Math.PI) / 180);

    // Draw robot body (rectangle) - Bright Yellow
    ctx.fillStyle = '#FFD700'; // Bright yellow
    ctx.fillRect(-screenLength / 2, -screenWidth / 2, screenLength, screenWidth);

    // Draw robot border
    ctx.strokeStyle = '#FF8C00'; // Dark orange border
    ctx.lineWidth = 3;
    ctx.strokeRect(-screenLength / 2, -screenWidth / 2, screenLength, screenWidth);

    // Draw orientation indicator (arrow pointing forward)
    ctx.fillStyle = '#FF0000'; // Red arrow
    ctx.beginPath();
    ctx.moveTo(0, -screenLength / 2);
    ctx.lineTo(-screenWidth / 4, -screenLength / 2 + screenLength / 3);
    ctx.lineTo(screenWidth / 4, -screenLength / 2 + screenLength / 3);
    ctx.closePath();
    ctx.fill();

    // Restore context state
    ctx.restore();
  }

  public getStatus(): RobotStatus {
    return {
      x: this.x,
      y: this.y,
      heading: this.heading,
      distanceTraveled: 0, // Will be calculated by StatusLogger
      isMoving: this.isMoving
    };
  }

  public setMoving(moving: boolean): void {
    this.isMoving = moving;
  }

  public getMovementConfig(): MovementConfig {
    return this.movementConfig;
  }

  public updateMovementConfig(config: Partial<MovementConfig>): void {
    this.movementConfig = { ...this.movementConfig, ...config };
  }
} 