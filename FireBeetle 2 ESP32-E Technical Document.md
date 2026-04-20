Technical Reference: FireBeetle 2 ESP32-E (DFR0654)

This document provides a consolidated technical specification and pinout reference for the FireBeetle 2 ESP32-E IoT Microcontroller, optimized for AI-assisted development.

1. Device Overview

The FireBeetle 2 ESP32-E is a compact, low-power IoT controller based on the ESP-WROOM-32E module. It is specifically designed for smart homes, industrial IoT, and wearable devices.

Processor: Xtensa® Dual-Core 32-bit LX6 Microprocessor

Clock Speed: 240 MHz

Memory: 520 KB SRAM, 448 KB ROM

Flash: 4 MB (Onboard)

Wireless: Wi-Fi 802.11 b/g/n & Bluetooth 4.2 (BR/EDR/BLE)

2. Power Specifications

Parameter

Value / Range

Operating Voltage

3.3V

Input Voltage (USB-C)

5V DC

Input Voltage (PH2.0)

3.7V to 4.2V (Li-ion Battery)

Input Voltage (VCC Pin)

5V DC

Charging Current

500mA

Operating Temp

-40°C to +85°C

Low Power Consumption

~2mA (Standard Sleep); ~500µA (with Low Power Pad cut)

3. Pinout & Peripheral Mapping

The following table maps the physical GPIO to Arduino IDE identifiers and special functions.

GPIO

Arduino Pin

ADC Channel

Functions / Remarks

0

0 or D5

ADC2_CH1

Avoid Use: Occupied during USB transmission

1

1 or TXD

-

UART0 TX (USB Serial)

2

2 or D9

ADC2_CH2

Onboard Blue LED (Digital Output)

3

3 or RXD

-

UART0 RX (USB Serial)

4

4 or D12

ADC2_CH0

General IO

5

5 or D8

-

Onboard WS2812 RGB LED

12

12 or D13

ADC2_CH5

General IO

13

13 or D7

ADC2_CH4

General IO

14

14 or D6

ADC2_CH6

General IO

15

15 or A4

ADC2_CH3

General IO

18

18 or SCK

-

SPI Clock

19

19 or MISO

-

SPI MISO

21

21 or SDA

-

I2C SDA

22

22 or SCL

-

I2C SCL

23

23 or MOSI

-

SPI MOSI

25

25 or D2

ADC2_CH8

DAC_1 (Analog Output 0-3.3V)

26

26 or D3

ADC2_CH9

DAC_2 (Analog Output 0-3.3V)

27

27 or D4

-

Onboard User Button

34

34 or A2

ADC1_CH6

Input Only

35

35 or A3

ADC1_CH7

Input Only

36

36 or A0

ADC1_CH0

Input Only

39

39 or A1

ADC1_CH3

Input Only

4. Hardware Features

4.1 Onboard Peripherals

USB-C Port: For programming and power supply (CH340 Serial Chip).

GDI Interface: DFRobot dedicated display interface for solderless screen connection.

RGB LED: WS2812 controllable via GPIO 5.

User Button: Connected to GPIO 27.

Battery Charging: Red LED indicator (Off: Fully charged/Not charging; On: Charging; Flash: No battery connected).

4.2 Low Power Pad

On the back of the PCB, there is a low-power solder jumper.

Default: Connected.

Disconnected: Cutting the thin wire reduces static power consumption by 500 μA.

Note: If disconnected, the RGB LED can only be driven via USB power.

5. Programming Reference (Arduino IDE)

Board Manager URL: https://downloadcd.dfrobot.com.cn/FireBeetle/package_DFRobot_index.json

Board Selection: FireBeetle-ESP32 or FireBeetle 2 ESP32-E

Required Libraries:

FastLED or Adafruit_NeoPixel for onboard RGB.

DFRobot_GDL for GDI-connected displays.

Constant Definitions

#define ONBOARD_LED  2
#define RGB_LED_PIN  5
#define USER_BUTTON  27
#define I2C_SDA      21
#define I2C_SCL      22


6. Communication Protocols

UART: 2 hardware interfaces. UART0 (pins 1, 3) is used for debugging.

I2C: 1 hardware interface (pins 21, 22).

SPI: 1 hardware interface (pins 18, 19, 23).

Touch: 7 capacitive touch sensors available on IOs.