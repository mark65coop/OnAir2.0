#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <FastLED.h>

// --- Configuration ---
#define LED_STRIP_PIN 12      // Pin connected to the MOSFET Gate
#define RGB_LED_PIN D8        // Onboard WS2812 RGB LED (Using official D8 pin definition)
#define NUM_LEDS 1            // Number of RGB LEDs on the board

CRGB leds[NUM_LEDS];

// Message structure to match the sender
typedef struct struct_message {
  bool commandOn;
} struct_message;

struct_message incomingData;

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingDataPtr, int len) {
  memcpy(&incomingData, incomingDataPtr, sizeof(incomingData));
  
  if (incomingData.commandOn) {
    digitalWrite(LED_STRIP_PIN, HIGH);
    leds[0] = CRGB::Red; // Turn onboard LED Red
    FastLED.show();
    Serial.println("Command Received: ON");
  } else {
    digitalWrite(LED_STRIP_PIN, LOW);
    leds[0] = CRGB::Black; // Turn onboard LED Off
    FastLED.show();
    Serial.println("Command Received: OFF");
  }
}

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_STRIP_PIN, OUTPUT);
  digitalWrite(LED_STRIP_PIN, LOW);

  // Initialize FastLED for the onboard RGB LED
  FastLED.addLeds<NEOPIXEL, RGB_LED_PIN>(leds, NUM_LEDS);
  
  // Diagnostic: Blink Green on startup
  leds[0] = CRGB::Green;
  FastLED.show();
  delay(1000);
  leds[0] = CRGB::Black;
  FastLED.show();

  // Set device as a Wi-Fi Station and force Channel 1
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  // Start the Wi-Fi hardware before setting channel
  esp_wifi_start();
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register callback to receive data
  esp_now_register_recv_cb(OnDataRecv);
  
  Serial.println("FireBeetle Receiver Ready. Waiting for signals...");
}

void loop() {
  delay(100);
}
