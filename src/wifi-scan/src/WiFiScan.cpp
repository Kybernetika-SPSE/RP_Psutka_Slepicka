#include "WiFi.h"

const int onboardledPin = 2;
const int ledOnDuration = 10;
const int ledOffDuration = 30;
const int scanDelay = 50;

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

// Function to blink the onboard LED
void blinkLED(int times, int onDuration, int offDuration)
{
    for (int i = 0; i < times; i++)
    {
        digitalWrite(onboardledPin, HIGH);
        delay(onDuration);
        digitalWrite(onboardledPin, LOW);
        delay(offDuration);
    }
}

// Function to handle scanning process
void handleScanning(int amount)
{
    for (int i = 0; i < amount; i++)
    {
        digitalWrite(onboardledPin, HIGH); // Turn onboard LED on
        scanNetworks(); // Scan for Wi-Fi networks
        digitalWrite(onboardledPin, LOW); // Turn onboard LED off
        delay(scanDelay);
    }
}

void setup()
{
    Serial.begin(115200); // Open Serial connection
    WiFi.mode(WIFI_STA);  // Set Wi-Fi to station mode

    pinMode(onboardledPin, OUTPUT); // Set onboard LED pin as output
    blinkLED(3, ledOnDuration, ledOffDuration); // Blink LED 3 times at successfull startup
}

void loop()
{
    if (Serial.available())
    {
        String amountStr = Serial.readStringUntil('\n'); // Read amount from Python
        amountStr.trim();                                // Remove any extra whitespace
        int amount = amountStr.toInt();                  // Convert to integer

        handleScanning(amount); // Handle the scanning process
    }
}