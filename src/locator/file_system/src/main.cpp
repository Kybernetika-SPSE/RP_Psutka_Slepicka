/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-vs-code-platformio-spiffs/  
*********/

#include <ArduinoJson.h>
#include <Arduino.h>
#include "SPIFFS.h"
#include "WiFi.h"

 
void auto_connect_to_wifi();

void setup() {
  Serial.begin(115200);
  if (!SPIFFS.begin(true)) {  // true -> format if failed
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  Serial.println("SPIFFS Mounted Successfully");
  auto_connect_to_wifi();
}
 
void loop() {

}


void auto_connect_to_wifi() {
  //scan networks
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    int n = WiFi.scanNetworks();
    if (n == 0) {
      Serial.println("no networks found");
    } else {
        //Serial.print(n);
        //Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print BSSID, RSSI and SSID for each network found
            Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x", WiFi.BSSID(i)[0], WiFi.BSSID(i)[1], WiFi.BSSID(i)[2], 
                                                            WiFi.BSSID(i)[3], WiFi.BSSID(i)[4], WiFi.BSSID(i)[5]);
            Serial.print(" - ");
            Serial.print(-WiFi.RSSI(i));
            Serial.print(" - ");
            Serial.print(WiFi.SSID(i));
            Serial.print("\n");
        }
    }
    Serial.println("");

  //open credential.json
    //open the file using SPIFFS
    File file = SPIFFS.open("/credential.json", FILE_READ); 

    //check if the file does not exist or cannot be opened
    if (!file) {
      Serial.println("Failed to open file for reading");
      return;
    }

    //read file
    JsonDocument doc;                                        //create a Json document
    DeserializationError error = deserializeJson(doc, file); //reads the file and parses the JSON into the doc object
    file.close();

    //check if file is valid json
    if (error) {
      Serial.println("Failed to parse JSON");
      return;
    }

    // Access the JSON arrays 
    JsonArray SSIDs = doc["SSIDs"];
    JsonArray PASSWORDs = doc["PASSWORDs"];

  //check if any saved networks match the scanned ones    
    for (size_t j = 0; j < SSIDs.size(); j++) {
      
      //file network credetials
      String SSID = SSIDs[j].as<const char*>();     //SSID
      String PASS = PASSWORDs[j].as<const char*>(); //password

      Serial.printf("Network \"%s\" ", SSID);

      //iterate through scanned networks
      for (int i = 0; i < n; ++i) {

        //check if they match
        if (WiFi.SSID(i) == SSID) {
          Serial.println("found");
          Serial.printf("Connecting to \"%s\"\n", SSID);

          //connect to that network
          WiFi.begin(SSID, PASS);
          while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.print(".");
          }
          Serial.print("\nsuccessfully connected\n");

          return;
        }
      }
      Serial.println("not found");
    }
    Serial.println("unable to connect to any network");
    return;
}