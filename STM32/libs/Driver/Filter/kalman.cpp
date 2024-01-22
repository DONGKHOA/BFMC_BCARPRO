#include "kalman.hpp"



void KalmanFilter_Init(kalman *Kalman, float mea_e, float est_e, float q) {
  Kalman->err_measure = mea_e;
  Kalman->err_estimate = est_e;
  Kalman->q = q;
}

float updateEstimate(kalman *Kalman, float mea) {
  Kalman->kalman_gain = Kalman->err_estimate / (Kalman->err_estimate + Kalman->err_measure);
  Kalman->current_estimate = Kalman->last_estimate + Kalman->kalman_gain * (mea - Kalman->last_estimate);
  Kalman->err_estimate =  (1.0 - Kalman->kalman_gain) * Kalman->err_estimate + fabs(Kalman->last_estimate - Kalman->current_estimate) * Kalman->q;
  Kalman->last_estimate = Kalman->current_estimate;

  return Kalman->current_estimate;
}