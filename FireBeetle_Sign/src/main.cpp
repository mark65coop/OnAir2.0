#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <FastLED.h>

// --- Configuration ---
#define LED_STRIP_PIN 12      // Pin connected to the MOSFET Gate
#define RGB_LED_PIN D8        // Onboard WS2812 RGB LED 
#define NUM_LEDS 1            // Number of RGB LEDs on the board

// PWM Settings for LED Strip
#define PWM_CHANNEL 0
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8 // 0-255

CRGB leds[NUM_LEDS];

// ESP-NOW Protocol
typedef struct struct_message {
  uint8_t effect; // 0=Off, 1=Solid, 2=Pulse, 3=Strobe, 4=Neon, 5=Glow
} struct_message;

struct_message incomingData;
uint8_t currentEffect = 0;

// Animation State Variables
unsigned long lastUpdate = 0;
int pwmValue = 0;
int fadeAmount = 5;

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingDataPtr, int len) {
  memcpy(&incomingData, incomingDataPtr, sizeof(incomingData));
  currentEffect = incomingData.effect;
  
  if (currentEffect == 0) {
    leds[0] = CRGB::Black;
    Serial.println("Sign Status: OFF");
  } else {
    leds[0] = CRGB::Red; 
    Serial.print("Effect Started: ");
    Serial.println(currentEffect);
  }
  FastLED.show();
}

void setup() {
  Serial.begin(115200);
  
  // Setup Hardware PWM for the MOSFET
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(LED_STRIP_PIN, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, 0);

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
  esp_wifi_start();
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("FireBeetle Sign Ready.");
}

void loop() {
  unsigned long currentMillis = millis();

  switch (currentEffect) {
    case 0: // OFF
      ledcWrite(PWM_CHANNEL, 0);
      break;

    case 1: // SOLID ON
      ledcWrite(PWM_CHANNEL, 255);
      break;

    case 2: // MEETING PULSE (Standard Breathing)
      if (currentMillis - lastUpdate > 15) {
        lastUpdate = currentMillis;
        pwmValue += fadeAmount;
        if (pwmValue <= 0 || pwmValue >= 255) {
          fadeAmount = -fadeAmount; // Reverse direction
          if (pwmValue <= 0) pwmValue = 0;
          if (pwmValue >= 255) pwmValue = 255;
        }
        ledcWrite(PWM_CHANNEL, pwmValue);
      }
      break;

    case 3: // URGENT STROBE
      if (currentMillis - lastUpdate > 100) {
        lastUpdate = currentMillis;
        pwmValue = (pwmValue == 0) ? 255 : 0;
        ledcWrite(PWM_CHANNEL, pwmValue);
      }
      break;

    case 4: // VINTAGE NEON FLICKER
      if (currentMillis - lastUpdate > random(50, 200)) {
        lastUpdate = currentMillis;
        // Simulate a dying tube: mostly bright, occasional dips
        if (random(0, 100) > 90) {
           pwmValue = random(100, 200); // Flicker dip
        } else {
           pwmValue = random(240, 255); // Solid high
        }
        ledcWrite(PWM_CHANNEL, pwmValue);
      }
      break;

    case 5: // GENTLE GLOW (5s smooth transition)
      // 5 seconds = 5000ms. 5000ms / 255 steps = ~20ms per step
      if (currentMillis - lastUpdate > 20) {
        lastUpdate = currentMillis;
        
        // Ensure fadeAmount is exactly 1 or -1 for maximum smoothness
        if (fadeAmount > 1 || fadeAmount < -1) fadeAmount = 1;
        
        pwmValue += fadeAmount;
        
        if (pwmValue <= 0) {
            pwmValue = 0;
            fadeAmount = 1; // Turn around and go up
        } else if (pwmValue >= 255) {
            pwmValue = 255;
            fadeAmount = -1; // Turn around and go down
        }
        ledcWrite(PWM_CHANNEL, pwmValue);
      }
      break;
  }
}
