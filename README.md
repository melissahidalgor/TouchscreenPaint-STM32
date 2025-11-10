# 🎨 Project: Touchscreen Paint Application (STM32F4)

## Project Summary

This project implements a simple "paint" application on an STM32F4 Discovery board, leveraging a TFT LCD screen and an STMPE811 capacitive touch controller. The user can draw directly on the screen by touching it, and select the drawing color using an external matrix keypad.

The application runs on the FreeRTOS real-time operating system, dividing the functionality into concurrent tasks: one for real-time drawing and one for color selection using an external matrix keypad.

## 💻 Technical Stack and Hardware

| Component | Interface/Protocol | Function |
| :--- | :---: | :--- |
| **STM32F4 Discovery** | Microcontroller | Controller for  external device interfacing programmed using FreeRTOS |
| **TFT LCD Screen (ILI9341)** | SPI | Display the drawing canvas and UI elements |
| **STMPE811** | I2C | Capacitive Touch Controller (Touch Sensor) |
| **Matrix Keypad (4x4)** | GPIO (Polling method) | Input device for selecting a new drawing color |

## ⚙️ RTOS Architecture and Multitasking

* **Task 1: Drawing Mode:** Continuously monitors the STMPE811 touch controller via a polling loop. If a touch is detected, it reads and maps the coordinates, and then draws a pixel/rectangle at the touch point.
  
* **Task 2: Color Selection:** Handles the external 4x4 matrix keypad input, waiting for the 'C' key to initiate the RRGGBB color entry sequence. The six decimal digits are converted into a single 16-bit color format used by the ILI9341 display.

## Explanatory Video (French)
[https://drive.google.com/file/d/1AYC2n2WXm69gvdXntyxIzc2F7kkmiZO7/view?usp=sharing](https://drive.google.com/file/d/1AYC2n2WXm69gvdXntyxIzc2F7kkmiZO7/view?usp=sharing)

![texto alternativo](TouchscreenPaint-STM32/Documentation/Screen.png)
