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

## Compile and Flash Instructions

### Prerequisites
- Install the Raspberry Pi Pico SDK and dependencies (see official documentation).
- Ensure CMake and Ninja are installed.

### Build
1. Open a terminal in the project directory.
2. Run:
	```bash
	mkdir -p build
	cd build
	cmake ..
	ninja
	```
	This will generate the firmware files (e.g., `.uf2`, `.elf`).

### Flash (First Time)
1. Hold down the BOOTSEL button on the Pico, plug it into your computer via USB.
2. Drag and drop the generated `.uf2` file (e.g., `host_hid.uf2`) onto the Pico's USB mass storage device.

### Flash (Subsequent Uploads)
If the firmware is running and using USB host mode, you must use a **Pico Probe** (SWD interface) for flashing and debugging:
1. Connect Pico Probe to the SWD pins of your Pico.
2. Use OpenOCD or your IDE to flash the `.elf` file.

## Firmware Upload & Debugging


## UART Command: Set GPIO Pin

You can send a 4-byte command over UART to set a GPIO pin's output value:

**Format:**
```
<cmd_type><pin><value>\n
```
- `cmd_type`: Command type (0x01 for set pin)
- `pin`: GPIO pin number (e.g., 2 for GPIO2)
- `value`: 0x00 for LOW, 0x01 for HIGH
- `\n`: Newline character (0x0A)

**Example:**
To set GPIO 2 HIGH:
```
0x01 0x02 0x01 0x0A
```
To set GPIO 2 LOW:
```
0x01 0x02 0x00 0x0A
```

Multiple commands can be sent back-to-back; the firmware will process each one in order.

## Code Overview
- `main.c`: Handles USB host initialization, HID keyboard input, and UART transmission.
- Uses TinyUSB for USB host stack.

## Example Applications
- Integrate with ESP32 or other microcontrollers to receive keyboard input for custom projects.
- Use as a standalone USB-to-UART keyboard adapter.

## License
MIT
