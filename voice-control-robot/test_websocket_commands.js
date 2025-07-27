const WebSocket = require('ws');

// Configuration - update with your VEX device ID
const config = {
  id: "51583", // Replace with your actual device ID
  port: "7071",
  ip: "localhost"
};

const url = `ws://${config.ip}:${config.port}/vexrobotics.vexcode/device?id=${config.id}`;

console.log('🤖 VEX WebSocket Command Tester');
console.log('================================');
console.log(`Connecting to: ${url}`);
console.log('');

let ws = null;

function connect() {
  ws = new WebSocket(url);
  
  ws.on('open', () => {
    console.log('✅ WebSocket connected successfully!');
    console.log('💡 Make sure Button X is pressed on VEX controller');
    console.log('💡 Check controller screen for "Serial listening ON"');
    console.log('');
    promptForCommand();
  });
  
  ws.on('message', (data) => {
    console.log(`📥 Received from robot: ${data.toString()}`);
  });
  
  ws.on('error', (error) => {
    console.error('❌ WebSocket error:', error.message);
    console.log('💡 Make sure VEX Extension is running and WebSocket Server is enabled');
  });
  
  ws.on('close', (code, reason) => {
    console.log(`🔌 WebSocket closed. Code: ${code}, Reason: ${reason}`);
  });
}

function sendCommand(command) {
  if (ws && ws.readyState === WebSocket.OPEN) {
    console.log(`📤 Sending: ${command}`);
    ws.send(command);
    
    // Give user feedback about expected robot behavior
    switch (command) {
      case 'FORWARD':
        console.log('🤖 Expected: Robot drives forward');
        break;
      case 'BACKWARD':
        console.log('🤖 Expected: Robot drives backward');
        break;
      case 'LEFT':
        console.log('🤖 Expected: Robot turns left');
        break;
      case 'RIGHT':
        console.log('🤖 Expected: Robot turns right');
        break;
      case 'STOP':
        console.log('🤖 Expected: Robot stops and reports status');
        break;
      case 'INTAKE':
        console.log('🤖 Expected: Intake mechanism starts (continues until STOP)');
        break;
      case 'SCORE':
        console.log('🤖 Expected: Scoring mechanism starts (continues until STOP)');
        break;
    }
  } else {
    console.log('❌ WebSocket not connected');
  }
}

function promptForCommand() {
  const readline = require('readline');
  const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
  });
  
  const validCommands = ['FORWARD', 'BACKWARD', 'LEFT', 'RIGHT', 'STOP', 'INTAKE', 'SCORE'];
  
  function askCommand() {
    rl.question('Enter command (or "quit" to exit): ', (input) => {
      const command = input.trim().toUpperCase();
      
      if (command === 'QUIT') {
        console.log('👋 Goodbye!');
        ws.close();
        rl.close();
        return;
      }
      
      if (validCommands.includes(command)) {
        sendCommand(command);
      } else {
        console.log('❌ Invalid command. Valid commands are:');
        console.log('   FORWARD, BACKWARD, LEFT, RIGHT, STOP, INTAKE, SCORE');
      }
      
      console.log('');
      askCommand();
    });
  }
  
  askCommand();
  
  // Handle Ctrl+C gracefully
  rl.on('SIGINT', () => {
    console.log('\n👋 Goodbye!');
    ws.close();
    rl.close();
  });
}

// Start the connection
connect(); 