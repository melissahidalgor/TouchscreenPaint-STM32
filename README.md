# 🎨 Project: Touchscreen Paint Application (STM32F4)

## Project Summary

This project implements a simple "paint" application on an STM32F4 Discovery board, leveraging a TFT LCD screen and an STMPE811 capacitive touch controller. The user can draw directly on the screen by touching it, and select the drawing color using an external matrix keypad.

## 💻 Technical Stack and Hardware

| Component | Interface/Protocol | Function |
| :--- | :---: | :--- |
| **STM32F4 Discovery** | Microcontroller | Controller for  external device interfacing programmed with Bare-Metal/Register-Level programming |
| **TFT LCD Screen (ILI9341)** | SPI | Display the drawing canvas and UI elements |
| **STMPE811** | I2C | Capacitive Touch Controller (Touch Sensor) |
| **Matrix Keypad (4x4)** | GPIO (Polling method) | Input device for selecting a new drawing color |

## ⚙️ Key Firmware Features

1. **Touch and Drawing Logic**
   * **Touch Input:** Reads the FIFO buffer of the STMPE811 controller to check for touch events and retrieve raw X/Y coordinates.

    * **Coordinate Remapping:** The raw touch coordinates are mathematically rearranged based on the LCD's dimensions to ensure precise drawing location

   * **Drawing Loop:** While a touch is detected, the main loop continuously reads the remapped coordinates and draws a small rectangle at the touch point.
  
2. **Color Selection Interface**
   * **Keypad Polling:** Matrix scanning logic (activating columns and checking rows) to detect a key press.

   * **Color Input:** Pressing the 'C' key on the keypad activates the color selection mode. The user inputs six digits (RRGGBB).

   * **RGB to 16-bit Color Conversion:** The six decimal digits are converted into a single 16-bit color format (used by the ILI9341 display) using bitwise shifts.
