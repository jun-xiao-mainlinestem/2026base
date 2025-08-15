// Import the WebSocket and WebSocketServer classes from the 'ws' library.
const { WebSocketServer } = require('ws');

// Create a new WebSocket server instance that will listen on port 8080.
const wss = new WebSocketServer({ port: 7071 });

console.log('WebSocket server is running on port 7071');

// The 'connection' event is emitted when a new client connects to the server.
wss.on('connection', ws => {
  // Log a message to the console to confirm a connection was made.
  console.log('New client connected!');

  // The 'message' event is fired when the server receives a message from the client.
  ws.on('message', data => {
    // Convert the received data (which is a buffer) to a string and log it.
    const message = data.toString();
    console.log(`Received message from client: ${message}`);

    // Send a response back to the client. This is a simple echo server.
    ws.send(`Server received your message: "${message}"`);
  });

  // The 'close' event is emitted when the client disconnects.
  ws.on('close', () => {
    console.log('Client disconnected.');
  });

  // The 'error' event handles any errors that occur.
  ws.on('error', error => {
    console.error('WebSocket error:', error);
  });
});

