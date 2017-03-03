#include <stdint.h>

#include "clock.h"
#include "control.h"
#include "imu_interface.h"
#include "sat.h"

// Y-factor PID parameters.
#define PID_GAIN_Y_P 0 // units=?, resolution=?, max=?
#define PID_GAIN_Y_I 0 // units=?, resolution=?, max=?
#define PID_GAIN_Y_D 0 // units=?, resolution=?, max=?

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
static int16_t control_compute_y_factor(const int16_t y_vel_desired,
                                        const int16_t y_vel_current,
                                        const clock_interval_t dt) {

  // units=metres*tick/second, resolution=2e-4, max=429496.7294
  static int32_t y_vel_error_integral = 0;
  // units=metres/second, resolution=2e-4, max=6.5534
  static int16_t y_vel_error_prev = 0;

  // Calculate current error.
  // units=metres/second, resolution=2e-4, max=6.5534
  const int16_t y_vel_error = sat_s16_sub(y_vel_desired, y_vel_current);

  // Integrate error.
  y_vel_error_integral =
    sat_s32_add(y_vel_error_integral, sat_s32_mul(y_vel_error, dt));

  // Differentiate error.
  // units=metres/tick*second, resolution=2e-4, max=6.5534
  const int16_t y_vel_error_deriv =
    sat_s16_div(sat_s16_sub(y_vel_error, y_vel_error_prev), dt);
  y_vel_error_prev = y_vel_error;

  // Compute y factor.
  const int16_t y_factor_p = sat_s32_mul(PID_GAIN_Y_P, y_vel_error);
  const int16_t y_factor_i = sat_s32_mul(PID_GAIN_Y_I, y_vel_error_integral);
  const int16_t y_factor_d = sat_s32_mul(PID_GAIN_Y_D, y_vel_error_deriv);
  const int16_t y_factor = sat_s32_add(sat_s32_add(y_factor_p, y_factor_i), y_factor_d);

  return y_factor;
}

static int16_t control_compute_roll_factor(const int16_t roll_desired,
                                           const int16_t roll_current,
                                           const clock_interval_t dt) {

  return 0;
}

static int16_t control_compute_pitch_factor(const int16_t pitch_desired,
                                            const int16_t pitch_current,
                                            const clock_interval_t dt) {

  return 0;
}

static int16_t control_compute_yaw_factor(const int16_t yaw_vel_desired,
                                          const int16_t yaw_vel_current,
                                          const clock_interval_t dt) {

  return 0;
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

  // Process sensor data and update state.
  //TODO

  // Find time interval (dt) since last control cycle.
  const clock_time_t curr_time = clock_get_time();
  const clock_interval_t dt = clock_diff(control_prev_time, curr_time);
  control_prev_time = curr_time;

  // Compute the contributions towards vertical, roll, pitch and yaw movements.
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
