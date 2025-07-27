const WebSocket = require('ws');

console.log('🤖 WebSocket Robot Command Test');
console.log('================================');
console.log('');

// Configuration
const config = {
  id: "51583",
  port: "7071",
  ip: "localhost"
};

const url = `ws://${config.ip}:${config.port}/vexrobotics.vexcode/device?id=${config.id}`;

console.log(`Connecting to: ${url}`);
console.log('');

let ws = null;
let testResults = {
  connected: false,
  commandsSent: 0,
  responsesReceived: 0
};

// Robot commands to test
const robotCommands = [
  'FORWARD',
  'STOP', 
  'LEFT',
  'STOP',
  'RIGHT',
  'STOP',
  'STATUS'
];

let currentCommandIndex = 0;

function sendNextCommand() {
  if (currentCommandIndex < robotCommands.length) {
    const command = robotCommands[currentCommandIndex];
    console.log(`📤 Sending command ${currentCommandIndex + 1}/${robotCommands.length}: ${command}`);
    ws.send(command);
    testResults.commandsSent++;
    currentCommandIndex++;
    
    // Wait 2 seconds before sending next command
    setTimeout(sendNextCommand, 2000);
  } else {
    console.log('');
    console.log('📊 Test Results:');
    console.log(`   WebSocket Connected: ${testResults.connected ? '✅' : '❌'}`);
    console.log(`   Commands Sent: ${testResults.commandsSent}`);
    console.log(`   Responses Received: ${testResults.responsesReceived}`);
    console.log('');
    
    if (testResults.connected && testResults.commandsSent > 0) {
      console.log('💡 Check the VEX controller screen for:');
      console.log('   - "Serial listening ON" message');
      console.log('   - "Received: COMMAND" messages');
      console.log('   - "Executing: COMMAND" messages');
      console.log('   - Robot movement');
      console.log('');
      
      if (testResults.responsesReceived === 0) {
        console.log('⚠️  No responses received from VEX brain');
        console.log('💡 This suggests the VEX Extension is not forwarding messages');
        console.log('💡 Try pressing Button X on the VEX controller to enable listening');
      }
    }
    
    ws.close();
  }
}

function runTest() {
  ws = new WebSocket(url);
  
  ws.on('open', () => {
    console.log('✅ WebSocket connected!');
    testResults.connected = true;
    
    // Start sending commands
    setTimeout(sendNextCommand, 1000);
  });
  
  ws.on('message', (data) => {
    const response = data.toString().trim();
    console.log(`📥 Received response: ${response}`);
    testResults.responsesReceived++;
    
    // Parse robot status if it's in the expected format
    if (response.startsWith('STATUS:')) {
      const parts = response.split(':');
      if (parts.length >= 3) {
        const heading = parseFloat(parts[1]);
        const distance = parseFloat(parts[2]);
        console.log(`🤖 Robot Status:`);
        console.log(`   Heading: ${heading}°`);
        console.log(`   Distance: ${distance} inches`);
      }
    }
  });
  
  ws.on('error', (error) => {
    console.error('❌ WebSocket error:', error.message);
    console.log('💡 Make sure VEX Extension is running and WebSocket Server is enabled');
  });
  
  ws.on('close', (code, reason) => {
    console.log(`🔌 WebSocket closed. Code: ${code}, Reason: ${reason}`);
    console.log('');
    console.log('💡 If robot didn\'t move, check:');
    console.log('   1. Button X is pressed on VEX controller');
    console.log('   2. Controller shows "Serial listening ON"');
    console.log('   3. VEX Extension WebSocket forwarding is enabled');
  });
}

console.log('⏳ Starting test...');
runTest(); 