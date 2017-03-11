#include <stdio.h>

#include "clock.h"
#include "control.h"
#include "imu_interface.h"
#include "pid.h"
#include "settings.h"

// Perform a full cycle of the control algorithm.
void control_cycle(measured_state_t *measured_state,
                   desired_state_t *desired_state,
                   rotor_speeds_t *rotor_speeds) {

  // Find time interval (dt) since last control cycle.
  static clock_time_t prev_time = 0;
  const clock_time_t curr_time = clock_get_time();
  const clock_interval_t dt = clock_diff(prev_time, curr_time);
  prev_time = curr_time;
  const float dt_float = ((float) dt) * ((float) SECONDS_PER_CLOCK_TICK);

  // Compute the contributions towards vertical, roll, pitch and yaw movements.
  static pid_state_t z_pid_state = INITIAL_PID_STATE;
  static pid_state_t roll_pid_state = INITIAL_PID_STATE;
  static pid_state_t pitch_pid_state = INITIAL_PID_STATE;
  static pid_state_t yaw_pid_state = INITIAL_PID_STATE;
  float z_factor = pid_cycle(
    desired_state->z_vel,  // Desired Z acceleration.
    measured_state->z_vel, // Actual (measured) Z acceleration.
    &z_pid_state,          // Structure in which to hold the PID controller's state.
    dt_float,              // The time that has passed since the last control cycle.
    PID_GAIN_Z_P,          // Proportional gain constant.
    PID_GAIN_Z_I,          // Integral gain constant.
    PID_GAIN_Z_D,          // Derivative gain constant.
    MIN_INTEGRAL_Z,
    MAX_INTEGRAL_Z);
  float roll_factor = pid_cycle(
    desired_state->roll,
    measured_state->roll,
    &roll_pid_state,
    dt_float,
    PID_GAIN_ROLL_P,
    PID_GAIN_ROLL_I,
    PID_GAIN_ROLL_D,
    MIN_INTEGRAL_ROLL,
    MAX_INTEGRAL_ROLL);
  float pitch_factor = pid_cycle(
    desired_state->pitch,
    measured_state->pitch,
    &pitch_pid_state,
    dt_float,
    PID_GAIN_PITCH_P,
    PID_GAIN_PITCH_I,
    PID_GAIN_PITCH_D,
    MIN_INTEGRAL_PITCH,
    MAX_INTEGRAL_PITCH);
  float yaw_factor = pid_cycle(
    desired_state->yaw_vel,
    measured_state->yaw_vel,
    &yaw_pid_state,
    dt_float,
    PID_GAIN_YAW_P,
    PID_GAIN_YAW_I,
    PID_GAIN_YAW_D,
    MIN_INTEGRAL_YAW,
    MAX_INTEGRAL_YAW);

  static float t = 0.0;
  z_factor = -0.3;
  roll_factor = 0.0;
  pitch_factor = 0.0;
  yaw_factor = 0.0;
  t += 0.00001;

  printf("LOG %f,%f,%f\n", dt_float, z_factor, measured_state->z_vel);

  //printf("z_vel error=%f-%f zfactor=%f\n", desired_state->z_vel, measured_state->z_vel, z_factor);

  // Combine contributions to produce rotor speeds.
  rotor_speeds->a = 0.4 + z_factor + roll_factor - pitch_factor + yaw_factor;
  rotor_speeds->b = 0.4 + z_factor - roll_factor - pitch_factor - yaw_factor;
  rotor_speeds->c = 0.4 + z_factor - roll_factor + pitch_factor + yaw_factor;
  rotor_speeds->d = 0.4 + z_factor + roll_factor + pitch_factor - yaw_factor;
}
