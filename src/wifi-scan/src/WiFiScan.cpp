#include "WiFi.h"

const int onboardledPin = 2;

// Function to scan for available Wi-Fi networks
void scanNetworks()
{
    int n = WiFi.scanNetworks();
    Serial.print("["); // Start JSON array
    for (int i = 0; i < n; i++)
    {
        Serial.printf("{\"bssid\":\"%s\", \"ssid\":\"%s\", \"rssi\":%d}%s",
                      WiFi.BSSIDstr(i).c_str(), WiFi.SSID(i), WiFi.RSSI(i), (i < n - 1) ? "," : "");
    }
    Serial.println("]"); // End JSON array
}


void setup()
{
    Serial.begin(115200); // Open Serial connection
    WiFi.mode(WIFI_STA);  // Set Wi-Fi to station mode

    pinMode(onboardledPin, OUTPUT); // Set onboard LED pin as output
    for (size_t i = 0; i < 3; i++)
    {
        digitalWrite(onboardledPin, HIGH);
        delay(10);
        digitalWrite(onboardledPin, LOW);
        delay(30);
    }
    
   
}


void loop()
{
    if (Serial.available())
    {
        String amount = Serial.readStringUntil('\n'); // Read amount from Python
        amount.trim();                                // Remove any extra whitespace

        for (int i = 0; i < amount.toInt(); i++)
        {
            digitalWrite(onboardledPin, HIGH); // Turn onboard LED on
            scanNetworks(); // Scan for Wi-Fi networks
            digitalWrite(onboardledPin, LOW); // Turn onboard LED off
            delay(50);
        }
    }
}