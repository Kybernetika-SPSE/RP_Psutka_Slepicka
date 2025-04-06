#include <Arduino.h>
#include "matrix.h"
#include "trilateration.h"

trilateration trilat;
float x, y, z, d;   // Temporary variables for parsing input

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        ; // Wait for the serial port to connect. Needed for native USB
    }

    return;
}

void loop()
{

    // Read serial input for data/instructions
    while (Serial.available())
    {
        String input = Serial.readStringUntil('\n');
        Serial.println(input);
        if (sscanf(input.c_str(), "cords[%f,%f,%f],%f", &x, &y, &z, &d) == 4) // Distance is temporarily parsed as well for testing
        {
            Serial.printf("Anchor: x=%.2f, y=%.2f, z=%.2f, d=%.2f\n", x, y, z, d);

            trilat.update({x, y, z, d});

            Matrix state = trilat.getState();
            Serial.printf("Current state: x=%.2f, y=%.2f, z=%.2f\n", state[0][0], state[1][0], state[2][0]);
            Serial.println();
        }
        else
        {
            Serial.println("Parsing failed!");
        }
    }
}
