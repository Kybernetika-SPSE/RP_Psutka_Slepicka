#include "KalmanFilter.h"

/**
 * @brief Kalman filter constructor.
 */
KalmanFilter::KalmanFilter()
    : X({{0, 0, 0, 0, 0, 0}}), // [x, y, z, vx, vy, vz]
      F{6, 6},                 // State transition matrix
      P(6, 6),                 // Covariance matrix
      Q(6, 6),                 // Process noise covariance
      H(3, 6),                 // Measurement matrix
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

/**
 * @brief Predict the next state of the system.
 *
 * @param dt Time step
 */
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

/**
 * @brief Update the state of the system based on the measurement.
 *
 * @param measurement Measurement vector
 */
void KalmanFilter::update(const Matrix &measurement)
{
    Matrix Y = measurement - (H * X);           // Measurement residual
    Matrix S = H * P * H.transpose() + R;       // Residual covariance
    Matrix K = P * H.transpose() * S.inverseQR(); // Kalman gain

    // Update state
    X = X + K * Y;

    // Update covariance
    P = (I - K * H) * P;
}

/**
 * @brief Get the current state of the system.
 *
 * @return Matrix State vector [x, y, z]
 */
Matrix KalmanFilter::getState() const
{
    return X; // Return position (x, y, z)
}

/**
 * @brief Adjust the process noise covariance matrix based on the current speed.
 */
void KalmanFilter::adjustKalmanNoise()
{
    static const float Q_MIN = 0.5f;  // Minimum process noise (stationary)
    static const float Q_MAX = 20.0f; // Maximum process noise (fast movement)
    static const float SCALE_FACTOR = 10.0f;

    float speed = sqrt(pow(X[0][3], 2) + pow(X[0][4], 2) + pow(X[0][5], 2));

    currentQScale = Q_MIN + (Q_MAX - Q_MIN) * (speed / SCALE_FACTOR);

    Q.set_identity(currentQScale);
}
