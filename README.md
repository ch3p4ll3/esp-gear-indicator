# ESP Gear Indicator
A gear indicator for your ESP-32 to use in your car.

## Overview
This project aims to create a gear indicator using an ESP-32 microcontroller. The gear indicator displays the current gear of the car on an Matrix LED display, providing real-time information.

The project is developed using PlatformIO, an open-source ecosystem for IoT development with a focus on embedded systems.

## Features

- Displays the current gear of the motorcycle on an OLED display.
- Supports up to 5 gears (1 to 5) and a neutral indicator. (can easly be configured for >=6 gears)
- Configurable gear ratios for different motorcycle models.

## Hardware Requirements
- ESP-32 microcontroller
- MAX72XX LED matrix display (e.g., MAX7219, MAX7221)
- OBD-II Bluetooth adapter

## Wiring
|     Component    |   GPIO Pin    |
|------------------|---------------|
| DIN (Data Input) | GPIO14        |
| CS (Chip Select) | GPIO12        |
| CLK (Clock)      | GPIO13        |

Connect the components as shown in the table above.

You can also use other pins by editing the pins from the config.h file

## Installation
- Clone the repository: `git clone https://github.com/ch3p4ll3/esp-gear-indicator.git`

- Open the project in PlatformIO IDE or Visual Studio Code with PlatformIO extension installed.

- Connect the ESP32 microcontroller, MAX72XX display following the [wiring](#wiring) instructions.
- Adjust the [settings](#configuration)
- Build and upload the firmware to the ESP32 microcontroller.
- Once uploaded, the gear indicator should start displaying the current gear on the MAX72XX LED matrix display.

## Configuration
Edit `config.h` to adjust settings such as GPIO pin configurations, gear ratios, display settings, etc.

## Future Features
- Automatic Calibration: Implement a feature where the gear indicator can calibrate itself autonomously without the need to specify any gear range. This will enhance user convenience and simplify the setup process.
- Support for Reverse Gear: Add support for detecting and displaying the reverse gear on the MAX72XX LED matrix display, providing comprehensive gear information.
- Additional Customization Options: Explore options to customize the display format, add animation effects, or integrate with other systems for enhanced functionality.

## Contributing
Contributions are welcome! Please feel free to submit pull requests, open issues, or suggest improvements.
