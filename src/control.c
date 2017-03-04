#include <stdint.h>

#include "clock.h"
#include "control.h"
#include "imu_interface.h"
#include "sat.h"

// Y-factor PID parameters.
#define PID_GAIN_Y_P 0
#define PID_GAIN_Y_I 0
#define PID_GAIN_Y_D 0

// Roll-factor PID parameters.
#define PID_GAIN_ROLL_P 0
#define PID_GAIN_ROLL_I 0
#define PID_GAIN_ROLL_D 0

// Pitch-factor PID parameters.
#define PID_GAIN_PITCH_P 0
#define PID_GAIN_PITCH_I 0
#define PID_GAIN_PITCH_D 0

// Yaw-factor PID parameters.
#define PID_GAIN_YAW_P 0
#define PID_GAIN_YAW_I 0
#define PID_GAIN_YAW_D 0

// The estimated current state of the vehicle.
control_state_t control_state = {0, 0, 0};

// The time at which the last control cycle ran. During the next clock cycle,
// the time will be checked again, and the difference between the two will be
// used as the time interval for purposes of integrating and differentiating
// variables.
clock_time_t control_prev_time = 0;

// y_vel_desired: units=metres/second, resolution=2e-4, max=6.5534
// y_vel_current: units=metres/second, resolution=2e-4, max=6.5534
// dt: units=ticks, resolution=1, max=32767
// returns y_factor: units=none, resolution=3e-5, max=0.98301
static int16_t control_compute_y_factor(const int16_t y_vel_desired,
                                        const int16_t y_vel_current,
                                        const clock_interval_t dt) {

  // Calculate current error.
  // units=metres/second, resolution=2e-4, max=6.5534
  const int16_t y_vel_error = sat_s16_sub(y_vel_desired, y_vel_current);

  // Integrate error.
  // units=metres*tick/second, resolution=2e-4, max=429496.7294
  static int32_t y_vel_error_integral = 0;
  y_vel_error_integral =
    sat_s32_add(y_vel_error_integral, sat_s32_mul(y_vel_error, dt));
  const int16_t y_vel_error_integral_16bit = (int16_t) (y_vel_error_integral >> 16);

  // Differentiate error.
  // units=metres/second, resolution=2e-4, max=6.5534
  static int16_t y_vel_error_prev = 0;
  // units=metres/tick*second, resolution=2e-4, max=6.5534
  const int16_t y_vel_error_deriv =
    sat_s16_div(sat_s16_sub(y_vel_error, y_vel_error_prev), dt);
  y_vel_error_prev = y_vel_error;

  // Compute y factor.
  // units=none, resolution=3e-5, max=0.98301
  const int16_t y_factor_p = sat_s32_mul(PID_GAIN_Y_P, y_vel_error) >> 16;
  const int16_t y_factor_i = sat_s32_mul(PID_GAIN_Y_I, y_vel_error_integral_16bit) >> 16;
  const int16_t y_factor_d = sat_s32_mul(PID_GAIN_Y_D, y_vel_error_deriv) >> 16;
  const int16_t y_factor =
    sat_s32_add(sat_s32_add(y_factor_p, y_factor_i), y_factor_d);

  return y_factor;
}

// roll_desired: units=radians, resolution=5e-5, max=1.63835
// roll_current: units=radians, resolution=5e-5, max=1.63835
// dt: units=ticks, resolution=1, max=32767
// returns roll_factor: units=none, resolution=3e-5, max=0.98301
static int16_t control_compute_roll_factor(const int16_t roll_desired,
                                           const int16_t roll_current,
                                           const clock_interval_t dt) {

  // Calculate current error.
  // units=radians, resolution=5e-5, max=1.63835
  const int16_t roll_error = sat_s16_sub(roll_desired, roll_current);

  // Integrate error.
  // units=radians*ticks, resolution=5e-5, max=107374.18235
  static int32_t roll_error_integral = 0;
  roll_error_integral =
    sat_s32_add(roll_error_integral, sat_s32_mul(roll_error, dt));
  const int16_t roll_error_integral_16bit = (int16_t) (roll_error_integral >> 16);

  // Differentiate error.
  // units=radians, resolution=5e-5, max=1.63835
  static int16_t roll_error_prev = 0;
  // units=radians/tick, resolution=5e-5, max=1.63835
  const int16_t roll_error_deriv =
    sat_s16_div(sat_s16_sub(roll_error, roll_error_prev), dt);
  roll_error_prev = roll_error;

  // Compute roll factor.
  // units=none, resolution=3e-5, max=0.98301
  const int16_t roll_factor_p = sat_s32_mul(PID_GAIN_ROLL_P, roll_error) >> 16;
  const int16_t roll_factor_i = sat_s32_mul(PID_GAIN_ROLL_I, roll_error_integral_16bit) >> 16;
  const int16_t roll_factor_d = sat_s32_mul(PID_GAIN_ROLL_D, roll_error_deriv) >> 16;
  const int16_t roll_factor =
    sat_s32_add(sat_s32_add(roll_factor_p, roll_factor_i), roll_factor_d);

  return roll_factor;
}

// pitch_desired: units=radians, resolution=5e-5, max=1.63835
// pitch_current: units=radians, resolution=5e-5, max=1.63835
// dt: units=ticks, resolution=1, max=32767
// returns pitch_factor: units=none, resolution=3e-5, max=0.98301
static int16_t control_compute_pitch_factor(const int16_t pitch_desired,
                                           const int16_t pitch_current,
                                           const clock_interval_t dt) {

  // Calculate current error.
  // units=radians, resolution=5e-5, max=1.63835
  const int16_t pitch_error = sat_s16_sub(pitch_desired, pitch_current);

  // Integrate error.
  // units=radians*ticks, resolution=5e-5, max=107374.18235
  static int32_t pitch_error_integral = 0;
  pitch_error_integral =
    sat_s32_add(pitch_error_integral, sat_s32_mul(pitch_error, dt));
  const int16_t pitch_error_integral_16bit = (int16_t) (pitch_error_integral >> 16);

  // Differentiate error.
  // units=radians, resolution=5e-5, max=1.63835
  static int16_t pitch_error_prev = 0;
  // units=radians/tick, resolution=5e-5, max=1.63835
  const int16_t pitch_error_deriv =
    sat_s16_div(sat_s16_sub(pitch_error, pitch_error_prev), dt);
  pitch_error_prev = pitch_error;

  // Compute pitch factor.
  // units=none, resolution=3e-5, max=0.98301
  const int16_t pitch_factor_p = sat_s32_mul(PID_GAIN_PITCH_P, pitch_error) >> 16;
  const int16_t pitch_factor_i = sat_s32_mul(PID_GAIN_PITCH_I, pitch_error_integral_16bit) >> 16;
  const int16_t pitch_factor_d = sat_s32_mul(PID_GAIN_PITCH_D, pitch_error_deriv) >> 16;
  const int16_t pitch_factor =
    sat_s32_add(sat_s32_add(pitch_factor_p, pitch_factor_i), pitch_factor_d);

  return pitch_factor;
}

// yaw_vel_desired: units=radians/second, resolution=4e-4, max=13.1068
// yaw_vel_current: units=radians/second, resolution=4e-4, max=13.1068
// dt: units=ticks, resolution=1, max=32767
// returns yaw_factor: units=none, resolution=3e-5, max=0.98301
static int16_t control_compute_yaw_factor(const int16_t yaw_vel_desired,
                                          const int16_t yaw_vel_current,
                                          const clock_interval_t dt) {

  // Calculate current error.
  // units=radians/second, resolution=4e-4, max=13.1068
  const int16_t yaw_vel_error = sat_s16_sub(yaw_vel_desired, yaw_vel_current);

  // Integrate error.
  // units=radians*tick/second, resolution=4e-4, max=858993.4588
  static int32_t yaw_vel_error_integral = 0;
  yaw_vel_error_integral =
    sat_s32_add(yaw_vel_error_integral, sat_s32_mul(yaw_vel_error, dt));
  const int16_t yaw_vel_error_integral_16bit = (int16_t) (yaw_vel_error_integral >> 16);

  // Differentiate error.
  // units=radians/second, resolution=4e-4, max=13.1068
  static int16_t yaw_vel_error_prev = 0;
  // units=radians/tick*second, resolution=4e-4, max=13.1068
  const int16_t yaw_vel_error_deriv =
    sat_s16_div(sat_s16_sub(yaw_vel_error, yaw_vel_error_prev), dt);
  yaw_vel_error_prev = yaw_vel_error;

  // Compute yaw factor.
  // units=none, resolution=3e-5, max=0.98301
  const int16_t yaw_factor_p = sat_s32_mul(PID_GAIN_YAW_P, yaw_vel_error) >> 16;
  const int16_t yaw_factor_i = sat_s32_mul(PID_GAIN_YAW_I, yaw_vel_error_integral_16bit) >> 16;
  const int16_t yaw_factor_d = sat_s32_mul(PID_GAIN_YAW_D, yaw_vel_error_deriv) >> 16;
  const int16_t yaw_factor =
    sat_s32_add(sat_s32_add(yaw_factor_p, yaw_factor_i), yaw_factor_d);

  return yaw_factor;
}

// Initialise the control algorithm.
void control_init() {
  control_state.roll = 0;
  control_state.pitch = 0;
  control_state.y_vel = 0;
  control_prev_time = clock_get_time();
}

// Perform a full cycle of the control algorithm, which includes reading from
// sensors, performing control calculations and updating the duty cycle of the
// PWM outputs.
void control_cycle() {
  // Read from sensors.
  raw_readings_t raw_readings;
  //read_sensors(&raw_readings);

  // Find time interval (dt) since last control cycle.
  const clock_time_t curr_time = clock_get_time();
  const clock_interval_t dt = clock_diff(control_prev_time, curr_time);
  control_prev_time = curr_time;

  // Process sensor data and update state.
  //TODO

  // Compute the contributions towards vertical, roll, pitch and yaw movements.
  // units=none, resolution=3e-5, max=0.98301
  const int16_t y_factor = control_compute_y_factor(0, control_state.y_vel, dt);
  const int16_t roll_factor = control_compute_roll_factor(0, control_state.roll, dt);
  const int16_t pitch_factor = control_compute_pitch_factor(0, control_state.pitch, dt);
  const int16_t yaw_factor = control_compute_yaw_factor(0, 0, dt);

  // Combine contributions to produce duty cycles.
  // units=none, resolution=3e-5, max=0.98301
  const int16_t rotor_a_duty = sat_s16_add(sat_s16_add(y_factor, roll_factor),
                                           sat_s16_add(-pitch_factor, yaw_factor));
  const int16_t rotor_b_duty = sat_s16_add(sat_s16_add(y_factor, -roll_factor),
                                           sat_s16_add(-pitch_factor, -yaw_factor));
  const int16_t rotor_c_duty = sat_s16_add(sat_s16_add(y_factor, -roll_factor),
                                           sat_s16_add(pitch_factor, yaw_factor));
  const int16_t rotor_d_duty = sat_s16_add(sat_s16_add(y_factor, roll_factor),
                                           sat_s16_add(pitch_factor, -yaw_factor));

  // Set PWM outputs.
  //TODO
}
