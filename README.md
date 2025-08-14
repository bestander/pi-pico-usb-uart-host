# USB-UART Keyboard Controller for Raspberry Pi Pico

This project implements a USB-to-UART keyboard controller for the Raspberry Pi Pico. It allows you to connect a USB keyboard to the Pico and transmit key events over UART to another device, such as an ESP32 or microcontroller.

## Features
- Uses the Pico's USB port in host mode (requires an OTG cable)
- Receives HID keyboard input and sends raw key reports over UART
- Simple integration with ESP32 or other UART-capable devices
- LED feedback for key events

## Hardware Requirements
- Raspberry Pi Pico
- USB OTG cable (USB-A to micro-USB or USB-C, depending on your keyboard and Pico)
- USB keyboard
- UART connection to target device

## Wiring
- **UART TX (Pico GPIO 8)** → Connect to RX pin of target device
- **UART RX (Pico GPIO 9)** → Connect to TX pin of target device (optional, for bidirectional communication)
- **LED (Pico GPIO 25)** → Onboard LED for feedback
- **Power**: Power the Pico via USB (OTG cable)

## UART Settings
- **Baud Rate**: 9600
- **Data Format**: 8 data bits, 1 stop bit, no parity

## Firmware Upload & Debugging
- **Important:** Since the Pico's USB port is used in host mode for keyboard connection, you cannot use it for firmware upload or debugging while the firmware is running.
- For subsequent uploads and debugging, use a **Pico Probe** (SWD interface).
- The **first upload** can be done via USB (bootloader mode), but after flashing this firmware, use Pico Probe for further development.

## Usage
1. Flash the firmware to your Raspberry Pi Pico.
2. Connect your USB keyboard to the Pico using an OTG cable.
3. Connect GPIO 8 (TX) and GPIO 9 (RX) to your target device's UART pins.
4. Power the Pico via USB.
5. Key events from the keyboard will be sent as raw HID reports over UART.

## Code Overview
- `main.c`: Handles USB host initialization, HID keyboard input, and UART transmission.
- Uses TinyUSB for USB host stack.

## Example Applications
- Integrate with ESP32 or other microcontrollers to receive keyboard input for custom projects.
- Use as a standalone USB-to-UART keyboard adapter.

## License
MIT
