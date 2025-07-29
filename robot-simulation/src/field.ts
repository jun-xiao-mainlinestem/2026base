import { Point, FieldDimensions } from './types';

export class Field {
  private width: number;  // 144 inches
  private height: number; // 144 inches
  private image: HTMLImageElement | null = null;
  private canvas: HTMLCanvasElement;
  private ctx: CanvasRenderingContext2D;
  private scale: number = 1;

  constructor(canvas: HTMLCanvasElement) {
    this.canvas = canvas;
    this.ctx = canvas.getContext('2d')!;
    this.width = 144;  // 12 feet = 144 inches
    this.height = 144; // 12 feet = 144 inches
  }

  public async loadImage(src: string): Promise<void> {
    return new Promise((resolve, reject) => {
      this.image = new Image();
      this.image.onload = () => {
        this.calculateScale();
        resolve();
      };
      this.image.onerror = () => {
        reject(new Error(`Failed to load field image: ${src}`));
      };
      this.image.src = src;
    });
  }

  private calculateScale(): void {
    if (!this.image) return;

    // Calculate scale to fit field in canvas while maintaining aspect ratio
    const canvasAspect = this.canvas.width / this.canvas.height;
    const imageAspect = this.image.width / this.image.height;

    if (canvasAspect > imageAspect) {
      // Canvas is wider than image
      this.scale = this.canvas.height / this.image.height;
    } else {
      // Canvas is taller than image
      this.scale = this.canvas.width / this.image.width;
    }
  }

  public draw(): void {
    if (!this.image) return;

    // Clear canvas
    this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);

    // Calculate centered position
    const scaledWidth = this.image.width * this.scale;
    const scaledHeight = this.image.height * this.scale;
    const x = (this.canvas.width - scaledWidth) / 2;
    const y = (this.canvas.height - scaledHeight) / 2;

    // Draw field image
    this.ctx.drawImage(this.image, x, y, scaledWidth, scaledHeight);
  }

  public screenToField(screenX: number, screenY: number): Point {
    if (!this.image) return { x: 0, y: 0 };

    // Calculate field offset (centered in canvas)
    const scaledWidth = this.image.width * this.scale;
    const scaledHeight = this.image.height * this.scale;
    const fieldX = (this.canvas.width - scaledWidth) / 2;
    const fieldY = (this.canvas.height - scaledHeight) / 2;

    // Convert screen coordinates to field coordinates
    const fieldCoordX = (screenX - fieldX) / this.scale;
    const fieldCoordY = (screenY - fieldY) / this.scale;

    // Convert to inches (assuming image represents 144x144 inches)
    const inchesX = (fieldCoordX / this.image.width) * this.width;
    const inchesY = (fieldCoordY / this.image.height) * this.height;

    return { x: inchesX, y: inchesY };
  }

  public fieldToScreen(fieldX: number, fieldY: number): Point {
    if (!this.image) return { x: 0, y: 0 };

    // Calculate field offset (centered in canvas)
    const scaledWidth = this.image.width * this.scale;
    const scaledHeight = this.image.height * this.scale;
    const fieldOffsetX = (this.canvas.width - scaledWidth) / 2;
    const fieldOffsetY = (this.canvas.height - scaledHeight) / 2;

    // Convert inches to image coordinates
    const imageX = (fieldX / this.width) * this.image.width;
    const imageY = (fieldY / this.height) * this.image.height;

    // Convert to screen coordinates
    const screenX = fieldOffsetX + (imageX * this.scale);
    const screenY = fieldOffsetY + (imageY * this.scale);

    return { x: screenX, y: screenY };
  }

  public getDimensions(): FieldDimensions {
    return {
      width: this.width,
      height: this.height,
      robotLength: 15, // 15 inches length (front-to-back)
      robotWidth: 17   // 17 inches width (side-to-side)
    };
  }

  public isPointInBounds(point: Point): boolean {
    return point.x >= 0 && point.x <= this.width && 
           point.y >= 0 && point.y <= this.height;
  }

  public getScale(): number {
    return this.scale;
  }

  public getContext(): CanvasRenderingContext2D {
    return this.ctx;
  }
} 