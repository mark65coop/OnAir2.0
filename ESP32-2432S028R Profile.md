Hardware Profile: ESP32-2432S028R (CYD USB-C Version)

📋 Metadata & Context

Brand: AITRIP

Product Name: 2 Pack ESP32 Development Board (2.8" Smart Display)

Common Alias: CYD (Cheap Yellow Display) - USB-C Variant

MCU: ESP32-D0WDQ6 (Dual-core, 240MHz)

Display Controller: ILI9341

⚙️ Technical Specifications

Feature

Details

CPU

Low-power dual-core 32-bit CPU (up to 240MHz, 600 DMIPS)

Memory

520 KB Internal SRAM

Storage

32Mbit SPI Flash (Default); TF Card support up to 4GB

Display

2.8" TFT LCD, 240x320 Resolution

Touch

Resistive Touch Screen (requires firm press/fingernail)

Connectivity

Wi-Fi 802.11b/g/n/e/i + Bluetooth 4.2 BR/EDR & BLE

Operating Voltage

4.75V - 5.25V

Power Consumption

6mA (Deep Sleep) to 310mA (Full Brightness + Flash)

Temp Range

-20 ℃ to 70 ℃ (Operating)

📍 Pinout & Bus Mapping

This version of the board utilizes two separate SPI buses: HSPI for the Display and VSPI for the Touch controller.

Display Interface (HSPI)

Function

GPIO Pin

Notes

TFT_MISO

12



TFT_MOSI

13



TFT_SCLK

14



TFT_CS

15



TFT_DC

2

Data/Command

TFT_RST

-1

(Handled via software/not wired to GPIO)

TFT_BL

21

Backlight (Active HIGH)

Touch Interface (VSPI)

Function

GPIO Pin

Notes

TOUCH_CS

33



TOUCH_MOSI

32



TOUCH_MISO

39



TOUCH_SCK

25



TOUCH_IRQ

36



🛠️ Software Configuration (TFT_eSPI)

For the Arduino environment, add the following to your User_Setup.h file:

#define USER_SETUP_INFO "User_Setup"
#define ILI9341_2_DRIVER
#define TFT_WIDTH 320
#define TFT_HEIGHT 240
#define TFT_BL 21
#define TFT_BACKLIGHT_ON HIGH

// SPI Bus Definitions
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS 15
#define TFT_DC 2
#define TFT_RST -1
#define TOUCH_CS 33
#define TFT_INVERSION_ON

// Performance Settings
#define SPI_FREQUENCY 55000000
#define SPI_READ_FREQUENCY 20000000
#define SPI_TOUCH_FREQUENCY 2500000
#define USE_HSPI_PORT


🚀 Key Features & Applications

Hardware Capabilities

Camera Support: Compatible with OV2640 and OV7670 (built-in flash).

Modes: Supports STA/AP/STA+AP working modes.

Provisioning: Smart Config and AirKiss one-click network configuration.

Frameworks: Embedded Lwip and FreeRTOS support.

Application Use Cases

Smart Home: Image transmission and APP remote control.

Agriculture: Environmental data collection and remote parameter setting.

Security: Wireless monitoring and QR recognition.

IoT: Signal positioning systems and telemetry.

💡 Developer Notes

Touch Performance: The resistive screen is spot-on with a stylus or fingernail but requires firm pressure for finger-touch.

Version Warning: This is the USB-C version. Pinouts differ from older Micro-USB variants (often referred to as the "Cheap Yellow Display" or CYD).

Bus Usage: Ensure your code initializes the Touch controller on the VSPI bus specifically if using the definitions provided above.