const WebSocket = require('ws');

console.log('🔧 Basic Serial Communication Test');
console.log('==================================');
console.log('');

// Configuration
const config = {
  id: "51583", // Your device ID
  port: "7071",
  ip: "localhost"
};

const url = `ws://${config.ip}:${config.port}/vexrobotics.vexcode/device?id=${config.id}`;

console.log(`Connecting to: ${url}`);
console.log('');

let ws = null;
let testResults = {
  connected: false,
  messageSent: false,
  messageReceived: false
};

function runTest() {
  ws = new WebSocket(url);
  
  ws.on('open', () => {
    console.log('✅ WebSocket connected!');
    testResults.connected = true;
    
    // Send a test message
    console.log('📤 Sending test message: "TEST"');
    ws.send('TEST');
    testResults.messageSent = true;
    
    // Wait 3 seconds then close
    setTimeout(() => {
      console.log('');
      console.log('📊 Test Results:');
      console.log(`   WebSocket Connected: ${testResults.connected ? '✅' : '❌'}`);
      console.log(`   Message Sent: ${testResults.messageSent ? '✅' : '❌'}`);
      console.log(`   Message Received: ${testResults.messageReceived ? '✅' : '❌'}`);
      console.log('');
      
      if (testResults.connected && testResults.messageSent) {
        console.log('💡 If controller screen shows "Received: TEST", serial communication is working!');
        console.log('💡 If controller screen shows nothing, check Button X and USB connection.');
      } else {
        console.log('❌ WebSocket connection failed. Check VEX Extension settings.');
      }
      
      ws.close();
      process.exit(0);
    }, 3000);
  });
  
  ws.on('message', (data) => {
    console.log(`📥 Received: ${data.toString()}`);
    testResults.messageReceived = true;
  });
  
  ws.on('error', (error) => {
    console.error('❌ WebSocket error:', error.message);
    console.log('💡 Make sure VEX Extension is running and WebSocket Server is enabled');
  });
  
  ws.on('close', (code, reason) => {
    console.log(`🔌 WebSocket closed. Code: ${code}, Reason: ${reason}`);
  });
}

runTest(); 