#include "wifi_utils.h"
#include <WiFi.h>

void setupWiFi(const char* ssid, const char* password) {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
}