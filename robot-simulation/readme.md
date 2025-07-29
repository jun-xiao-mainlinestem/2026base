# VEX Robot Simulation

A web-based simulation of VEX robot movement on a 12'x12' field. This application allows users to place a robot on the field and control its movement through command buttons, providing real-time visual feedback and movement logging.

## Features

- **Interactive Robot Placement**: Drag and drop the robot anywhere on the field
- **Robot Rotation**: Use mouse wheel to rotate the robot to any angle
- **Movement Commands**: 7 different movement commands with realistic physics
- **Real-time Status**: Live updates of position, heading, and distance traveled
- **Activity Logging**: Complete log of all commands and movements
- **Responsive Design**: Works on desktop and mobile devices
- **Keyboard Controls**: Full keyboard support for movement commands

## Movement Commands

- **Drive Forward (slowly)**: Move robot forward in the direction it's facing
- **Drive Backward (slowly)**: Move robot backward opposite to facing direction
- **Turn Left (90°)**: Rotate robot 90 degrees counter-clockwise
- **Turn Right (90°)**: Rotate robot 90 degrees clockwise
- **Turn Clockwise (slowly)**: Continuous clockwise rotation
- **Turn Counter-clockwise (slowly)**: Continuous counter-clockwise rotation
- **Stop**: Stop all movement and reset distance tracking

## Controls

### Mouse/Touch
- **Drag**: Click and drag the robot to reposition it
- **Scroll**: Use mouse wheel to rotate the robot
- **Touch**: On mobile devices, touch and drag to move robot

### Keyboard
- **W/↑**: Drive Forward
- **S/↓**: Drive Backward
- **A/←**: Turn Left (90°)
- **D/→**: Turn Right (90°)
- **Q**: Turn Counter-clockwise
- **E**: Turn Clockwise
- **Space**: Stop

## Technical Details

### Field Specifications
- **Size**: 12 feet by 12 feet (144 inches by 144 inches)
- **Robot Size**: 15 inches length by 17 inches width
- **Coordinate System**: Inches with origin at top-left corner

### Movement Physics
- **Forward Speed**: 2 inches per frame
- **Backward Speed**: 1.5 inches per frame (slower)
- **Continuous Turn Speed**: 3 degrees per frame
- **90-degree Turn Speed**: 15 degrees per frame
- **Animation**: 60 FPS target

## Getting Started

### Prerequisites
- Node.js (version 14 or higher)
- npm or yarn

### Installation

1. Clone the repository:
```bash
git clone <repository-url>
cd robot-simulation
```

2. Install dependencies:
```bash
npm install
```

3. Start the development server:
```bash
npm run dev
```

4. Open your browser and navigate to `http://localhost:3001`

### Building for Production

```bash
npm run build
```

The built files will be in the `dist/` directory.

### Deploying to Netlify

This project can be easily deployed to Netlify for free hosting. Here are the steps:

#### Option 1: Deploy via Netlify CLI

1. Install Netlify CLI globally:
```bash
npm install -g netlify-cli
```

2. Login to Netlify:
```bash
netlify login
```

3. Initialize and deploy the project:
```bash
netlify init
```

4. Follow the prompts:
   - Create & configure a new site? → `Y`
   - Team → Select your team
   - Site name → `robot-simulation` (or your preferred name)
   - Publish directory → `dist`
   - Functions directory → (leave blank, press Enter)
   - Deploy to production? → `Y`

5. Your site will be deployed and you'll get a URL like `https://robot-simulation-xxx.netlify.app`

#### Option 2: Deploy via GitHub Integration

1. Push your code to a GitHub repository

2. Go to [netlify.com](https://netlify.com) and sign in with GitHub

3. Click "New site from Git"

4. Choose GitHub and select your repository

5. Configure the build settings:
   - **Build command**: `npm run build`
   - **Publish directory**: `dist`
   - **Base directory**: (leave blank)

6. Click "Deploy site"

#### Option 3: Deploy via Netlify Dashboard (Drag & Drop)

1. Build your project locally:
```bash
npm run build
```

2. Go to [netlify.com](https://netlify.com) and sign in

3. Drag and drop the `dist` folder to the deploy area

4. Your site will be deployed instantly

#### Custom Domain (Optional)

After deployment, you can add a custom domain:

1. Go to your site dashboard on Netlify
2. Click "Domain settings"
3. Click "Add custom domain"
4. Enter your domain and follow the DNS configuration instructions

#### Environment Variables

If you need to add environment variables:

1. Go to your site dashboard on Netlify
2. Click "Site settings" → "Environment variables"
3. Add your variables
4. Redeploy the site

#### Automatic Deployments

With GitHub integration, Netlify will automatically deploy:
- Every push to the main branch
- Every pull request (preview deployments)
- Every tag (production deployments)

#### Netlify.toml Configuration (Optional)

For more control, you can create a `netlify.toml` file in your project root:

```toml
[build]
  command = "npm run build"
  publish = "dist"

[build.environment]
  NODE_VERSION = "18"

[[redirects]]
  from = "/*"
  to = "/index.html"
  status = 200
```

## Project Structure

```
robot-simulation/
├── src/
│   ├── index.html          # Main HTML structure
│   ├── styles.css          # CSS styling
│   ├── app.ts              # Main application logic
│   ├── robot.ts            # Robot class and rendering
│   ├── field.ts            # Field class and coordinate conversion
│   ├── commands.ts         # Command system and execution
│   ├── status-logger.ts    # Status tracking and logging
│   └── types.ts            # TypeScript type definitions
├── assets/                 # Static assets (field images)
├── dist/                   # Built files
├── package.json
├── tsconfig.json
├── webpack.config.js
└── README.md
```

## Development

### Architecture

The application is built with a modular architecture:

- **Field Class**: Handles field image loading and coordinate conversion
- **Robot Class**: Manages robot state, movement, and rendering
- **Command System**: Processes and executes movement commands
- **Status Logger**: Tracks robot status and movement history
- **Main App**: Orchestrates all components and handles user interactions

### Key Classes

#### Robot
- Position and heading management
- Movement physics and boundary detection
- Canvas rendering with orientation indicator

#### Field
- Image loading and scaling
- Coordinate system conversion (screen ↔ field)
- Boundary validation

#### CommandSystem
- Command queue management
- Movement execution
- Realistic physics simulation

#### StatusLogger
- Real-time status tracking
- Movement history logging
- Distance calculation

## Browser Support

- Chrome (recommended)
- Firefox
- Safari
- Edge

## Future Enhancements

- Multiple robots support
- Obstacles and barriers on field
- Path visualization
- Adjustable movement speeds
- Save/load robot configurations
- Export movement data
- 3D perspective view
- Sound effects

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## License

This project is licensed under the ISC License.

## Acknowledgments

- VEX Robotics for inspiration
- HTML5 Canvas API for graphics rendering
- TypeScript for type safety
- Webpack for build system 