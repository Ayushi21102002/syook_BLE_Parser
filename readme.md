# BLE Device Data Parser

## Overview
This project is a BLE (Bluetooth Low Energy) device data parser written in C. It parses hex-encoded BLE data packets, extracts relevant information, and determines if the device is moving based on accelerometer data.

## Features
- Parses BLE data packets and extracts:
  - MAC Address
  - UUIDs
  - Frame Type
  - Version Number
  - Battery Level
  - Accelerometer Data (X, Y, Z axes)
- Prints parsed data in a readable format
- Determines if the device is moving based on accelerometer data

## Commands
### Compilation
To compile the code, use the following command:
```sh
gcc -o ble_parser main.c -lm
