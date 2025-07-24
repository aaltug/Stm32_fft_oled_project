# FFT and OLED Visualization on STM32

This project demonstrates real-time Fast Fourier Transform (FFT) processing on pre-recorded audio data using the STM32F103C8T6 microcontroller and visualizes the frequency components on an SSD1306 OLED display via I2C communication.

## 🔧 Hardware Used
- STM32F103C8T6 ("Blue Pill")
- SSD1306 OLED Display (128x32, I2C)
- FTDI module (for programming/debugging)

## 📦 Libraries
- CMSIS-DSP (for FFT computation)
- SSD1306 OLED Library (modified for I2C rendering)
- STM32 HAL (generated with STM32CubeMX)

## 🔍 Features
- Converts 256-sample audio signal into frequency domain using `arm_cfft_f32`
- Magnitude spectrum calculated with `sqrt(real^2 + imag^2)`
- OLED displays bar chart of FFT results

## 📂 File Structure
Core/
├── Inc/
│ └── audio_data.h
└── Src/
├── main.c
└── audio_data.c

## 🚀 Setup & Run
1. Clone the repo
2. Open in STM32CubeIDE
3. Generate code with correct I2C pins configured
4. Flash the MCU in "boot mode"
5. Switch to "run mode" after programming

## 📷 Demo
<img width="451" height="339" alt="image" src="https://github.com/user-attachments/assets/f5cf2e1b-f38d-4869-a4be-aa457e71ab06" />


