# Remote Control Setup Guide

## Overview

This guide explains how to enable WebSocket communication and find your robot's Device ID and IP address.

## Prerequisites

Before you can connect to your robot, you need:

- VEX V5 controller connected to your computer via USB
- VEX VS Code Extension installed

## Step 1: Enable WebSocket Server in VS Code

- Click on the Setting Icon at the bottom left of the VS Code User Interface and select "Settings"
- Type "vex" in the search box. Scroll down to vexrobotics.vexcode.WebsocketServer
- Select **Enable**. Set host address to localhost and port to 7071.

## Step 2: Find Your Device ID
- Upload your robot program to the v5 Brain to make sure the the brain connected to the computer via radio.
- Click on the VEX icon in the left bar of the VS Code User Interface.
- Expand the VEX Device Info view.
- Look for the 5 digit **id** at the end of the url.

![Device ID Location in VS Code](img/device_id.png)
*Example: ID shown in WebSocket entry*

## Step 3: Find your computer's IP address
- 

## Troubleshooting

### WebSocket Server Not Starting
- Make sure your controller is connected via USB to the computer
- Ensure your robot program is uploaded to the brain
- Try restarting VS Code
- Check that the VEX VS Code Extension is properly installed and running

### Connection Failed
- Double-check the Device ID is correct
- Double-check the IP address is correct
- Ensure your mobile device is on the same wifi network as the computer
- Try refreshing web page

## Important Notes

**Important:**  The Device ID is unique to your VSCode session and will remain the same unless you restart VS Code.
