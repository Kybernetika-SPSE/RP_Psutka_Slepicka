#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

#include <vector>
#include "matrix.h"

class KalmanFilter {
public:
    KalmanFilter();
    void predict(float dt);
    void update(const std::vector<float> &measurement);
    void adjustKalmanNoise();
    std::vector<float> getState();

private:
    Matrix X;  // State vector [x, y, z, vx, vy, vz]
    Matrix F;  // State transition matrix
    Matrix P;  // Covariance matrix
    Matrix Q;  // Process noise covariance
    Matrix H;  // Measurement matrix
    Matrix R;  // Measurement noise covariance
    Matrix I;  // Identity matrix
    float currentQScale; // Scale for Q matrix
};

#endif
