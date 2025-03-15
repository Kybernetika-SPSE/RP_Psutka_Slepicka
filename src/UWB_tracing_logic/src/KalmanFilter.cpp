#include "KalmanFilter.h"

KalmanFilter::KalmanFilter()

    : X({{0, 0, 0, 0, 0, 0}}), // [x, y, z, vx, vy, vz]
      F{6, 6},                 // State transition matrix
      P(6, 6),                 // Covariance matrix
      Q(6, 6),                 // Process noise covariance
      H(6, 3),                 // Measurement matrix
      R(3, 3),                 // Measurement noise covariance
      I(6, 6)                  // Identity matrix
{
    F.set_identity();

    P.set_identity(10);

    Q.set_identity();

    H.set_identity();

    R.set_identity(1);

    I.set_identity();
}


void KalmanFilter::predict(float dt)
{
    // Update state transition matrix (F) for dt
    F[0][3] = dt;
    F[1][4] = dt;
    F[2][5] = dt;

    // Predict next state
    X = F * X;

    // Predict covariance
    P = F * P * F.transpose() + Q;
}


void KalmanFilter::update(const std::vector<float> &measurement) {
    Matrix M({measurement});
    Matrix Y = M - (H * X); // Measurement residual
    Matrix S = H * P * H.transpose() + R; // Residual covariance
    Matrix K = P * H.transpose() * S.inverse(); // Kalman gain

    // Update state
    X = X + K * Y;

    // Update covariance
    P = (I - K * H) * P;
}

std::vector<float> KalmanFilter::getState() {
    return {X[0][0], X[0][1], X[0][2]}; // Return position (x, y, z)
}

void KalmanFilter::adjustKalmanNoise() {
    static const float Q_MIN = 0.5f;   // Minimum process noise (stationary)
    static const float Q_MAX = 20.0f;  // Maximum process noise (fast movement)
    static const float SCALE_FACTOR = 10.0f;

    float speed = sqrt(pow(X[0][3], 2) + pow(X[0][4], 2) + pow(X[0][5], 2));

    currentQScale = Q_MIN + (Q_MAX - Q_MIN) * (speed / SCALE_FACTOR);

    Q.set_identity(currentQScale);
}
