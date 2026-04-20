#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

// --- Touch Screen Pins (VSPI) ---
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

SPIClass mySpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);

TFT_eSPI tft = TFT_eSPI();

// Broadcast MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
esp_now_peer_info_t peerInfo;

// 0=Off, 1=Solid, 2=Pulse, 3=Strobe, 4=Neon, 5=Glow
typedef struct struct_message {
  uint8_t effect; 
} struct_message;

struct_message myData;
uint8_t currentEffect = 0;
const int maxEffects = 6;

unsigned long lastTouchTime = 0;
const int touchCooldown = 500; 

const char* getEffectName(uint8_t id) {
  switch(id) {
    case 0: return "OFF";
    case 1: return "SOLID ON";
    case 2: return "MEETING PULSE";
    case 3: return "URGENT STROBE";
    case 4: return "VINTAGE NEON";
    case 5: return "GENTLE GLOW";
    default: return "UNKNOWN";
  }
}

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void drawButton() {
  tft.fillScreen(TFT_BLACK);
  
  if (currentEffect != 0) {
    tft.fillRoundRect(20, 40, 280, 160, 20, TFT_RED);
    tft.setTextColor(TFT_WHITE);
  } else {
    tft.fillRoundRect(20, 40, 280, 160, 20, TFT_DARKGREY);
    tft.setTextColor(TFT_LIGHTGREY);
  }
  
  tft.setTextSize(3); // Slightly smaller than 4 to fit long names
  tft.drawCentreString(getEffectName(currentEffect), 160, 100, 1);
  
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString("Tap to Cycle Effects", 160, 210, 1);
}

void setup() {
  Serial.begin(115200);

  // Initialize Display
  tft.init();
  tft.setRotation(1); 
  
  // Initialize Touch Screen on separate VSPI bus
  mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  ts.begin(mySpi);
  ts.setRotation(1);

  // Set Wi-Fi to Station mode and force Channel 1
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_wifi_start();
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register the send callback
  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 1;  
  peerInfo.encrypt = false;
  peerInfo.ifidx = WIFI_IF_STA; // Explicitly set the interface
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  drawButton();
  Serial.println("CYD Remote Ready.");
}

void loop() {
  // Check if screen is touched anywhere
  if (ts.touched()) {
    if (millis() - lastTouchTime > touchCooldown) {
      
      // Cycle to the next effect
      currentEffect++;
      if (currentEffect >= maxEffects) {
        currentEffect = 0;
      }
      
      drawButton();
      
      // Blast the new effect ID over ESP-NOW
      myData.effect = currentEffect;
      esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
      
      Serial.print("Sent Effect: ");
      Serial.println(getEffectName(currentEffect));
      
      lastTouchTime = millis();
    }
  }
}
