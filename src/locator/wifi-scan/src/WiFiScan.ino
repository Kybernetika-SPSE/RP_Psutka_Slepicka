/*
 *  This sketch demonstrates how to scan WiFi networks.
 *  The API is almost the same as with the WiFi Shield library,
 *  the most obvious difference being the different file you need to include:
 */
#include "WiFi.h"

int scan_number = 0;



void setup()
{
    Serial.begin(115200);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    Serial.println("Setup done");
}

void loop()
{


    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();

    Serial.print("scan number: ");
    Serial.println(scan_number);
    scan_number += 1;

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

    // Wait a bit before scanning again
}