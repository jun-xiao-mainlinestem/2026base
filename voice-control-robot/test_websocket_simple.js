const WebSocket = require('ws');

// Configuration
const config = {
  id: "51583",
  port: "7071",
  ip: "localhost"
};

const url = `ws://${config.ip}:${config.port}/vexrobotics.vexcode/device?id=${config.id}`;

console.log('🔧 Simple WebSocket Test');
console.log('========================');
console.log(`Connecting to: ${url}`);
console.log('');

const ws = new WebSocket(url);

ws.on('open', () => {
  console.log('✅ WebSocket connected successfully');
  console.log('📤 Sending test command: FORWARD');
  
  // Send a test command
  ws.send('FORWARD');
  
  // Wait 2 seconds then send STOP
  setTimeout(() => {
    console.log('📤 Sending STOP command');
    ws.send('STOP');
    
    // Wait another 2 seconds then close
    setTimeout(() => {
      console.log('🔌 Closing connection');
      ws.close();
    }, 2000);
  }, 2000);
});

ws.on('message', (data) => {
  console.log(`📥 Received message: ${data}`);
});

ws.on('error', (error) => {
  console.error('❌ WebSocket error:', error.message);
});

ws.on('close', (code, reason) => {
  console.log(`🔌 WebSocket closed. Code: ${code}, Reason: ${reason}`);
  console.log('');
  console.log('💡 Check the VEX controller screen for any messages');
  console.log('💡 If you see "Serial listening ON" but no commands, the issue is forwarding');
});

console.log('⏳ Waiting for connection...'); 