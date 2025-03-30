#include <Arduino.h>
#include "matrix.h"
#include "leastSquare.h"
#include "KalmanFilter.h"

#define BUFFER_SIZE 10 // Number of stored data points

struct DataPoint
{
    float x, y, z;
    float d;
};

DataPoint buffer[BUFFER_SIZE]; // Circular buffer
int bufferIndex = 0;           // Points to the next insertion position
int count = 0;                 // Number of data points in the buffer

float x, y, z; // Anchor coordinates
float d;       // Distance to the target

KalmanFilter kf; // Kalman filter object