/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-vs-code-platformio-spiffs/  
*********/

#include <ArduinoJson.h>
#include <Arduino.h>
#include "SPIFFS.h"
 
void readJson();

void setup() {
  Serial.begin(115200);
  if (!SPIFFS.begin(true)) {  // true -> format if failed
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  Serial.println("SPIFFS Mounted Successfully");
  readJson();
}
 
void loop() {

}


void readJson() {
  File file = SPIFFS.open("/config.json", FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) {
    Serial.println("Failed to parse JSON");
    return;
  }

  JsonArray SSIDs = doc["SSIDs"];  // Access the JSON arra

  Serial.println("Sensor Readings:");
    for (float value : SSIDs) {
        Serial.print(value);
    }

  String target = "Lithium";

  Serial.println("is there?");
  for (String SSID : SSIDs)
  {
    if (SSID == target)
    {
      Serial.print("yes");
      break;
    } else {
      continue;
    }
    Serial.print("no");
  }
}

