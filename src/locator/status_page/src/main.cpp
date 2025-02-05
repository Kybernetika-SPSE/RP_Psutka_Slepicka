#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "ssid";
const char* password = "password";


// Set LED GPIO
const int ledPin = 2;
String ledState = "OFF";

// Create WiFi server on port 80
WiFiServer server(80);


void wifi_scan();


void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    //wifi_scan();
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();  // Ensure no active connections

    // Connect to WiFi
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);


    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    ledState = "ON";
    digitalWrite(ledPin, HIGH);

    server.begin();
}

void loop() {
    WiFiClient client = server.available(); // Listen for incoming clients

    if (client) {
        Serial.println("New Client connected.");
        String header = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                header += c;
                if (c == '\n') {
                    if (header.indexOf("GET /led/on") >= 0) {
                        Serial.println("Turning LED ON");
                        ledState = "ON";
                        digitalWrite(ledPin, HIGH);
                    } else if (header.indexOf("GET /led/off") >= 0) {
                        Serial.println("Turning LED OFF");
                        ledState = "OFF";
                        digitalWrite(ledPin, LOW);
                    } else if (header.indexOf("GET /status") >= 0) {
                        // Send JSON response
                        String json = "{ \"led\": \"" + ledState + "\" }";
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: application/json");
                        client.println("Connection: close");
                        client.println();
                        client.println(json);
                        break;
                    } else if (header.indexOf("GET /scan") >= 0) {
                      wifi_scan();
                    }
                    

                    // Send HTML Response
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-type:text/html");
                    client.println("Connection: close");
                    client.println();
                    client.println("<!DOCTYPE html><html><head><title>ESP32 LED Control</title>");
                    client.println("<style>body { font-family: Helvetica; text-align: center; margin-top: 50px; }");
                    client.println(".button { border: none; color: white; padding: 16px 40px; font-size: 30px; cursor: pointer; margin: 10px; }");
                    client.println(".scan { background-color:rgb(187, 47, 169); }");
                    client.println(".on { background-color: #4CAF50; }");
                    client.println(".off { background-color: #555555; }</style></head>");
                    client.println("<body><h2>ESP32 LED Control</h2>");
                    
                    client.println("<p>LED Status: <span id='ledStatus'>" + ledState + "</span></p>");
                    client.println("<button class='button on' onclick='sendCommand(\"/led/on\")'>ON</button>");
                    client.println("<button class='button off' onclick='sendCommand(\"/led/off\")'>OFF</button>");
                    client.println("<br>");
                    client.println("<button class='button scan' onclick='sendCommand(\"/scan\")'>scan</button>");

                    // JavaScript for fetch requests
                    client.println("<script>");
                    client.println("function sendCommand(url) {fetch(url).then(response => response.json()).then(data => updateStatus(data)).catch(error => console.error(error));}");
                    client.println("function updateStatus(data) {document.getElementById('ledStatus').innerText = data.led;}");

                    // Keep auto-refreshing every 2 seconds, just in case
                    client.println("setInterval(() => fetch('/status').then(response => response.json()).then(data => updateStatus(data)), 2000);");
                    client.println("</script>");

                    client.println("</body></html>");
                    break;
                }
            }
        }

        client.stop();
        Serial.println("Client disconnected.");
    }
}


void wifi_scan() {
    Serial.println("Scanning for Wi-Fi networks...");

    // Start Wi-Fi in station mode (to scan networks)
    WiFi.mode(WIFI_STA);
    //WiFi.disconnect();  // Ensure no active connections
    delay(100);

    int numNetworks = WiFi.scanNetworks(); // Scan for Wi-Fi networks

    if (numNetworks == 0) {
        Serial.println("No Wi-Fi networks found.");
    } else {
        Serial.println("Available Wi-Fi networks:");
        for (int i = 0; i < numNetworks; i++) {
            Serial.printf("%d: %s (Signal Strength: %d dBm) %s\n",
                          i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i),
                          WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "[Open]" : "[Secured]");
        }
    }
}