/*
 * ATTENTION: The "eval" devtool has been used (maybe by default in mode: "development").
 * This devtool is neither made for production nor for readable output files.
 * It uses "eval()" calls to create a separate source file in the browser devtools.
 * If you are trying to read the output file, select a different devtool (https://webpack.js.org/configuration/devtool/)
 * or disable the default devtool with "devtool: false".
 * If you are looking for production-ready output files, see mode: "production" (https://webpack.js.org/configuration/mode/).
 */
/******/ (() => { // webpackBootstrap
/******/ 	"use strict";
/******/ 	var __webpack_modules__ = ({

/***/ "./src/app.ts":
/*!********************!*\
  !*** ./src/app.ts ***!
  \********************/
/***/ ((__unused_webpack_module, __webpack_exports__, __webpack_require__) => {

eval("{__webpack_require__.r(__webpack_exports__);\n/* harmony export */ __webpack_require__.d(__webpack_exports__, {\n/* harmony export */   VoiceControlApp: () => (/* binding */ VoiceControlApp)\n/* harmony export */ });\n/* harmony import */ var _styles_css__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! ./styles.css */ \"./src/styles.css\");\n/* harmony import */ var _websocket__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(/*! ./websocket */ \"./src/websocket.ts\");\n/* harmony import */ var _speech__WEBPACK_IMPORTED_MODULE_2__ = __webpack_require__(/*! ./speech */ \"./src/speech.ts\");\n/* harmony import */ var _commands__WEBPACK_IMPORTED_MODULE_3__ = __webpack_require__(/*! ./commands */ \"./src/commands.ts\");\n/* harmony import */ var _types__WEBPACK_IMPORTED_MODULE_4__ = __webpack_require__(/*! ./types */ \"./src/types.ts\");\n\n\n\n\n\nclass VoiceControlApp {\n    constructor() {\n        this.isConnected = false;\n        // Configuration - update with your VEX device ID and port\n        this.websocket = new _websocket__WEBPACK_IMPORTED_MODULE_1__.VEXWebSocket({\n            id: \"51583\", // Replace with your device ID\n            port: \"7071\",\n            ip: \"localhost\"\n        });\n        this.speech = new _speech__WEBPACK_IMPORTED_MODULE_2__.SpeechRecognition();\n        this.commandProcessor = new _commands__WEBPACK_IMPORTED_MODULE_3__.CommandProcessor();\n        this.setupEventHandlers();\n        this.initializeUI();\n    }\n    setupEventHandlers() {\n        // WebSocket events\n        this.websocket.onStatusChange((connected) => {\n            this.isConnected = connected;\n            this.updateConnectionStatus(connected);\n        });\n        this.websocket.onMessage((data) => {\n            this.displayMessage(`Robot: ${data}`);\n            this.parseRobotStatus(data);\n        });\n        // Speech recognition events\n        this.speech.onResult((voiceCommand) => {\n            this.handleVoiceCommand(voiceCommand);\n        });\n        this.speech.onError((error) => {\n            this.displayError(`Speech error: ${error}`);\n        });\n    }\n    async handleVoiceCommand(voiceCommand) {\n        this.displayMessage(`Heard: \"${voiceCommand.command}\"`);\n        const robotCommand = this.commandProcessor.processVoiceCommand(voiceCommand);\n        if (robotCommand) {\n            this.executeRobotCommand(robotCommand);\n        }\n        else {\n            this.displayError(`Unknown command: \"${voiceCommand.command}\"`);\n        }\n    }\n    executeRobotCommand(robotCommand) {\n        if (!this.isConnected) {\n            this.displayError('Not connected to robot');\n            return;\n        }\n        const commandString = this.formatRobotCommand(robotCommand);\n        this.websocket.send(commandString);\n        this.displayMessage(`Executing: ${robotCommand.action}`);\n    }\n    formatRobotCommand(robotCommand) {\n        switch (robotCommand.action) {\n            case _types__WEBPACK_IMPORTED_MODULE_4__.RobotAction.FORWARD:\n                return 'FORWARD';\n            case _types__WEBPACK_IMPORTED_MODULE_4__.RobotAction.BACKWARD:\n                return 'BACKWARD';\n            case _types__WEBPACK_IMPORTED_MODULE_4__.RobotAction.LEFT:\n                return 'LEFT';\n            case _types__WEBPACK_IMPORTED_MODULE_4__.RobotAction.RIGHT:\n                return 'RIGHT';\n            case _types__WEBPACK_IMPORTED_MODULE_4__.RobotAction.STOP:\n                return 'STOP';\n            case _types__WEBPACK_IMPORTED_MODULE_4__.RobotAction.INTAKE:\n                return 'INTAKE';\n            case _types__WEBPACK_IMPORTED_MODULE_4__.RobotAction.SCORE:\n                return 'SCORE';\n            default:\n                return 'STOP';\n        }\n    }\n    updateConnectionStatus(connected) {\n        const statusElement = document.getElementById('connection-status');\n        if (statusElement) {\n            statusElement.textContent = connected ? '🟢 Connected' : '🔴 Disconnected';\n            statusElement.className = connected ? 'status connected' : 'status disconnected';\n        }\n    }\n    displayMessage(message) {\n        const outputElement = document.getElementById('output');\n        if (outputElement) {\n            const timestamp = new Date().toLocaleTimeString();\n            outputElement.innerHTML += `<div>[${timestamp}] ${message}</div>`;\n            outputElement.scrollTop = outputElement.scrollHeight;\n        }\n    }\n    displayError(error) {\n        this.displayMessage(`❌ ${error}`);\n    }\n    updateListeningStatus(isListening) {\n        const statusElement = document.getElementById('listening-status');\n        if (statusElement) {\n            if (isListening) {\n                statusElement.textContent = '🎤 Listening for voice commands...';\n                statusElement.className = 'listening-status listening';\n            }\n            else {\n                statusElement.textContent = '🔇 Not listening';\n                statusElement.className = 'listening-status';\n            }\n        }\n    }\n    parseRobotStatus(data) {\n        // Parse status messages from robot\n        if (data.includes('STATUS:')) {\n            const parts = data.split(':');\n            if (parts.length >= 3) {\n                const heading = parts[1].trim();\n                const distance = parts[2].trim();\n                const headingElement = document.getElementById('current-heading');\n                const distanceElement = document.getElementById('distance-driven');\n                if (headingElement)\n                    headingElement.textContent = heading;\n                if (distanceElement)\n                    distanceElement.textContent = distance;\n            }\n        }\n    }\n    initializeUI() {\n        // Connect button\n        const connectBtn = document.getElementById('connect-btn');\n        if (connectBtn) {\n            connectBtn.addEventListener('click', async () => {\n                try {\n                    this.displayMessage('Attempting to connect to VEX Extension...');\n                    await this.websocket.connect();\n                    this.displayMessage('✅ Successfully connected to VEX Extension!');\n                }\n                catch (error) {\n                    this.displayError(`Connection failed: ${error}`);\n                    this.displayMessage('💡 Make sure VEX Extension is running and WebSocket Server is enabled');\n                }\n            });\n        }\n        // Start/Stop listening button\n        const listenBtn = document.getElementById('listen-btn');\n        if (listenBtn) {\n            listenBtn.addEventListener('click', () => {\n                if (listenBtn.textContent === 'Start Listening') {\n                    this.speech.start();\n                    listenBtn.textContent = 'Stop Listening';\n                    listenBtn.className = 'btn btn-danger';\n                    this.updateListeningStatus(true);\n                    this.displayMessage('🎤 Voice recognition started - speak commands continuously');\n                }\n                else {\n                    this.speech.stop();\n                    listenBtn.textContent = 'Start Listening';\n                    listenBtn.className = 'btn btn-success';\n                    this.updateListeningStatus(false);\n                    this.displayMessage('🔇 Voice recognition stopped');\n                }\n            });\n        }\n        // Clear output button\n        const clearBtn = document.getElementById('clear-btn');\n        if (clearBtn) {\n            clearBtn.addEventListener('click', () => {\n                const outputElement = document.getElementById('output');\n                if (outputElement) {\n                    outputElement.innerHTML = '';\n                }\n            });\n        }\n    }\n    async start() {\n        // Don't auto-connect on startup - let user click connect button\n        this.displayMessage('Voice control system ready. Click \"Connect to Robot\" to start.');\n    }\n}\n// Initialize app when DOM is loaded\ndocument.addEventListener('DOMContentLoaded', () => {\n    const app = new VoiceControlApp();\n    app.start();\n});\n\n\n//# sourceURL=webpack://voice-control-robot/./src/app.ts?\n}");

/***/ }),

/***/ "./src/commands.ts":
/*!*************************!*\
  !*** ./src/commands.ts ***!
  \*************************/
/***/ ((__unused_webpack_module, __webpack_exports__, __webpack_require__) => {

eval("{__webpack_require__.r(__webpack_exports__);\n/* harmony export */ __webpack_require__.d(__webpack_exports__, {\n/* harmony export */   CommandProcessor: () => (/* binding */ CommandProcessor)\n/* harmony export */ });\n/* harmony import */ var _types__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! ./types */ \"./src/types.ts\");\n\nclass CommandProcessor {\n    constructor() {\n        this.commandMap = new Map([\n            ['forward', _types__WEBPACK_IMPORTED_MODULE_0__.RobotAction.FORWARD],\n            ['backward', _types__WEBPACK_IMPORTED_MODULE_0__.RobotAction.BACKWARD],\n            ['back', _types__WEBPACK_IMPORTED_MODULE_0__.RobotAction.BACKWARD],\n            ['left', _types__WEBPACK_IMPORTED_MODULE_0__.RobotAction.LEFT],\n            ['right', _types__WEBPACK_IMPORTED_MODULE_0__.RobotAction.RIGHT],\n            ['stop', _types__WEBPACK_IMPORTED_MODULE_0__.RobotAction.STOP],\n            ['intake', _types__WEBPACK_IMPORTED_MODULE_0__.RobotAction.INTAKE],\n            ['score', _types__WEBPACK_IMPORTED_MODULE_0__.RobotAction.SCORE]\n        ]);\n    }\n    processVoiceCommand(voiceCommand) {\n        const words = voiceCommand.command.split(' ');\n        for (const word of words) {\n            const action = this.commandMap.get(word);\n            if (action) {\n                return {\n                    action,\n                    parameters: this.extractParameters(voiceCommand.command, action)\n                };\n            }\n        }\n        return null;\n    }\n    extractParameters(command, action) {\n        // Extract additional parameters like distance, speed, etc.\n        const numbers = command.match(/\\d+/g);\n        if (numbers && numbers.length > 0) {\n            return { value: parseInt(numbers[0]) };\n        }\n        return {};\n    }\n    getSupportedCommands() {\n        return Array.from(this.commandMap.keys());\n    }\n    isValidCommand(command) {\n        return this.commandMap.has(command.toLowerCase());\n    }\n}\n\n\n//# sourceURL=webpack://voice-control-robot/./src/commands.ts?\n}");

/***/ }),

/***/ "./src/speech.ts":
/*!***********************!*\
  !*** ./src/speech.ts ***!
  \***********************/
/***/ ((__unused_webpack_module, __webpack_exports__, __webpack_require__) => {

eval("{__webpack_require__.r(__webpack_exports__);\n/* harmony export */ __webpack_require__.d(__webpack_exports__, {\n/* harmony export */   SpeechRecognition: () => (/* binding */ SpeechRecognition)\n/* harmony export */ });\nclass SpeechRecognition {\n    constructor() {\n        this.isListening = false;\n        if ('webkitSpeechRecognition' in window || 'SpeechRecognition' in window) {\n            const SpeechRecognition = window.SpeechRecognition ||\n                window.webkitSpeechRecognition;\n            this.recognition = new SpeechRecognition();\n            this.setupRecognition();\n        }\n        else {\n            throw new Error('Speech recognition not supported');\n        }\n    }\n    setupRecognition() {\n        this.recognition.continuous = false;\n        this.recognition.interimResults = false;\n        this.recognition.lang = 'en-US';\n        this.recognition.maxAlternatives = 1;\n        this.recognition.onresult = (event) => {\n            const result = event.results[0];\n            const command = {\n                command: result[0].transcript.toLowerCase(),\n                confidence: result[0].confidence,\n                timestamp: new Date()\n            };\n            this.onResultCallback?.(command);\n        };\n        this.recognition.onerror = (event) => {\n            this.onErrorCallback?.(event.error);\n        };\n        this.recognition.onend = () => {\n            // Only restart if we're supposed to be listening\n            if (this.isListening) {\n                console.log('Speech recognition ended, restarting...');\n                this.recognition.start();\n            }\n        };\n    }\n    start() {\n        if (!this.isListening) {\n            this.isListening = true;\n            this.recognition.start();\n        }\n    }\n    stop() {\n        this.isListening = false;\n        this.recognition.stop();\n        console.log('Speech recognition stopped by user');\n    }\n    onResult(callback) {\n        this.onResultCallback = callback;\n    }\n    onError(callback) {\n        this.onErrorCallback = callback;\n    }\n    getListeningStatus() {\n        return this.isListening;\n    }\n}\n\n\n//# sourceURL=webpack://voice-control-robot/./src/speech.ts?\n}");

/***/ }),

/***/ "./src/styles.css":
/*!************************!*\
  !*** ./src/styles.css ***!
  \************************/
/***/ ((__unused_webpack_module, __webpack_exports__, __webpack_require__) => {

eval("{__webpack_require__.r(__webpack_exports__);\n// extracted by mini-css-extract-plugin\n\n\n//# sourceURL=webpack://voice-control-robot/./src/styles.css?\n}");

/***/ }),

/***/ "./src/types.ts":
/*!**********************!*\
  !*** ./src/types.ts ***!
  \**********************/
/***/ ((__unused_webpack_module, __webpack_exports__, __webpack_require__) => {

eval("{__webpack_require__.r(__webpack_exports__);\n/* harmony export */ __webpack_require__.d(__webpack_exports__, {\n/* harmony export */   RobotAction: () => (/* binding */ RobotAction)\n/* harmony export */ });\nvar RobotAction;\n(function (RobotAction) {\n    RobotAction[\"FORWARD\"] = \"FORWARD\";\n    RobotAction[\"BACKWARD\"] = \"BACKWARD\";\n    RobotAction[\"LEFT\"] = \"LEFT\";\n    RobotAction[\"RIGHT\"] = \"RIGHT\";\n    RobotAction[\"STOP\"] = \"STOP\";\n    RobotAction[\"INTAKE\"] = \"INTAKE\";\n    RobotAction[\"SCORE\"] = \"SCORE\";\n})(RobotAction || (RobotAction = {}));\n\n\n//# sourceURL=webpack://voice-control-robot/./src/types.ts?\n}");

/***/ }),

/***/ "./src/websocket.ts":
/*!**************************!*\
  !*** ./src/websocket.ts ***!
  \**************************/
/***/ ((__unused_webpack_module, __webpack_exports__, __webpack_require__) => {

eval("{__webpack_require__.r(__webpack_exports__);\n/* harmony export */ __webpack_require__.d(__webpack_exports__, {\n/* harmony export */   VEXWebSocket: () => (/* binding */ VEXWebSocket)\n/* harmony export */ });\nclass VEXWebSocket {\n    constructor(config) {\n        this.socket = null;\n        this.config = config;\n    }\n    connect() {\n        return new Promise((resolve, reject) => {\n            const url = `ws://${this.config.ip}:${this.config.port}/vexrobotics.vexcode/device?id=${this.config.id}`;\n            console.log('Attempting to connect to:', url);\n            this.socket = new WebSocket(url);\n            this.socket.onopen = () => {\n                console.log('WebSocket connected successfully');\n                this.onStatusChangeCallback?.(true);\n                resolve();\n            };\n            this.socket.onmessage = (event) => {\n                console.log('WebSocket message received:', event.data);\n                if (this.onMessageCallback) {\n                    this.onMessageCallback(event.data);\n                }\n            };\n            this.socket.onerror = (error) => {\n                console.error('WebSocket error:', error);\n                // Don't reject immediately, let onclose handle it\n            };\n            this.socket.onclose = (event) => {\n                console.log('WebSocket disconnected. Code:', event.code, 'Reason:', event.reason);\n                this.onStatusChangeCallback?.(false);\n                if (event.code !== 1000) { // Not a normal closure\n                    reject(new Error(`WebSocket closed with code ${event.code}: ${event.reason}`));\n                }\n            };\n        });\n    }\n    send(message) {\n        if (this.socket && this.socket.readyState === WebSocket.OPEN) {\n            this.socket.send(message);\n        }\n        else {\n            console.error('WebSocket not connected');\n        }\n    }\n    disconnect() {\n        if (this.socket) {\n            this.socket.close();\n            this.socket = null;\n        }\n    }\n    onMessage(callback) {\n        this.onMessageCallback = callback;\n    }\n    onStatusChange(callback) {\n        this.onStatusChangeCallback = callback;\n    }\n    isConnected() {\n        return this.socket?.readyState === WebSocket.OPEN;\n    }\n}\n\n\n//# sourceURL=webpack://voice-control-robot/./src/websocket.ts?\n}");

/***/ })

/******/ 	});
/************************************************************************/
/******/ 	// The module cache
/******/ 	var __webpack_module_cache__ = {};
/******/ 	
/******/ 	// The require function
/******/ 	function __webpack_require__(moduleId) {
/******/ 		// Check if module is in cache
/******/ 		var cachedModule = __webpack_module_cache__[moduleId];
/******/ 		if (cachedModule !== undefined) {
/******/ 			return cachedModule.exports;
/******/ 		}
/******/ 		// Create a new module (and put it into the cache)
/******/ 		var module = __webpack_module_cache__[moduleId] = {
/******/ 			// no module.id needed
/******/ 			// no module.loaded needed
/******/ 			exports: {}
/******/ 		};
/******/ 	
/******/ 		// Execute the module function
/******/ 		__webpack_modules__[moduleId](module, module.exports, __webpack_require__);
/******/ 	
/******/ 		// Return the exports of the module
/******/ 		return module.exports;
/******/ 	}
/******/ 	
/************************************************************************/
/******/ 	/* webpack/runtime/define property getters */
/******/ 	(() => {
/******/ 		// define getter functions for harmony exports
/******/ 		__webpack_require__.d = (exports, definition) => {
/******/ 			for(var key in definition) {
/******/ 				if(__webpack_require__.o(definition, key) && !__webpack_require__.o(exports, key)) {
/******/ 					Object.defineProperty(exports, key, { enumerable: true, get: definition[key] });
/******/ 				}
/******/ 			}
/******/ 		};
/******/ 	})();
/******/ 	
/******/ 	/* webpack/runtime/hasOwnProperty shorthand */
/******/ 	(() => {
/******/ 		__webpack_require__.o = (obj, prop) => (Object.prototype.hasOwnProperty.call(obj, prop))
/******/ 	})();
/******/ 	
/******/ 	/* webpack/runtime/make namespace object */
/******/ 	(() => {
/******/ 		// define __esModule on exports
/******/ 		__webpack_require__.r = (exports) => {
/******/ 			if(typeof Symbol !== 'undefined' && Symbol.toStringTag) {
/******/ 				Object.defineProperty(exports, Symbol.toStringTag, { value: 'Module' });
/******/ 			}
/******/ 			Object.defineProperty(exports, '__esModule', { value: true });
/******/ 		};
/******/ 	})();
/******/ 	
/************************************************************************/
/******/ 	
/******/ 	// startup
/******/ 	// Load entry module and return exports
/******/ 	// This entry module can't be inlined because the eval devtool is used.
/******/ 	var __webpack_exports__ = __webpack_require__("./src/app.ts");
/******/ 	
/******/ })()
;