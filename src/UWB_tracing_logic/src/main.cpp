#include <Arduino.h>
#include "matrix.h"
#include "trilateration.h"

float x, y, z, d; // Temporary variables for parsing input

trilateration trilat;

bool is2D = true;     // Default assumption
bool modeSet = false; // Flag to lock mode after first input

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        ; // Wait for the serial port to connect. Needed for native USB
    }

    // Setup the trilateration object
    if (modeSet)
    {
        if (is2D)
        {
            Serial.println("2D mode set.");
            trilat = trilateration(2);
        }
        else
        {
            Serial.println("3D mode set.");
            trilat = trilateration(3);
        }
    }
    return;
}

void loop()
{
    // Read input from Serial
    while (Serial.available())
    {
        String input = Serial.readStringUntil('\n');
        Serial.println(input);

        // First, determine if the input is 2D or 3D
        if (!modeSet && sscanf(input.c_str(), "cords[%f,%f,%f],%f", &x, &y, &z, &d) == 4)
        {
            is2D = false;
            modeSet = true;
            Serial.println("3D mode set.");
            trilat = trilateration(3); // Initialize 3D trilateration
        }
        else if (!modeSet && sscanf(input.c_str(), "cords[%f,%f],%f", &x, &y, &d) == 3)
        {
            is2D = true;
            modeSet = true;
            Serial.println("2D mode set.");
            trilat = trilateration(2); // Initialize 2D trilateration
        }

        // After mode is determined, parse using the right pattern
        if (sscanf(input.c_str(), "cords[%f,%f,%f],%f", &x, &y, &z, &d) == 4)
        {
            if (is2D)
            {
                Serial.println("Warning: Input is 3D but mode is set to 2D. Ignoring z-coordinate.");
                trilat.update({x, y, d});
            }
            else
            {
                trilat.update({x, y, z, d});
            }
        }
        else if (sscanf(input.c_str(), "cords[%f,%f],%f", &x, &y, &d) == 3)
        {
            if (!is2D)
            {
                Serial.println("Warning: Input is 2D but mode is set to 3D. Providing default z=0.");
                z = 0;
                trilat.update({x, y, z, d});
            }
            else
            {
                trilat.update({x, y, d});
            }
        }
        else
        {
            Serial.println("Parsing failed!");
            continue;
        }

        // Print the current state of the trilateration object
        Matrix state = trilat.getState();
        Serial.print("Current state: ");
        if (is2D)
        {
            Serial.printf("x: %.2f, y: %.2f\n", state[0][0], state[1][0]);
        }
        else
        {
            Serial.printf("x: %.2f, y: %.2f, z: %.2f\n", state[0][0], state[1][0], state[2][0]);
        }
    }
}
