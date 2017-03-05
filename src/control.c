#include "clock.h"
#include "control.h"
#include "imu_interface.h"
#include "pid.h"

// Y-factor PID parameters.
#define PID_GAIN_Z_P 0
#define PID_GAIN_Z_I 0
#define PID_GAIN_Z_D 0

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

// Perform a full cycle of the control algorithm.
void control_cycle(measured_state_t *measured_state,
                   desired_state_t *desired_state,
                   duty_cycles_t *duty_cycles) {

  // Find time interval (dt) since last control cycle.
  static clock_time_t prev_time = 0;
  const clock_time_t curr_time = clock_get_time();
  const clock_interval_t dt = clock_diff(prev_time, curr_time);
  prev_time = curr_time;
  const float dt_float = ((float) dt) * ((float) CLOCK_SECONDS_PER_TICK);

  // Compute the contributions towards vertical, roll, pitch and yaw movements.
  static pid_state_t z_pid_state = INITIAL_PID_STATE;
  static pid_state_t roll_pid_state = INITIAL_PID_STATE;
  static pid_state_t pitch_pid_state = INITIAL_PID_STATE;
  static pid_state_t yaw_pid_state = INITIAL_PID_STATE;
  const float z_factor = pid_cycle(
    desired_state->z_vel,  // Desired Z velocity.
    measured_state->z_vel, // Actual (measured) Z velocity.
    &z_pid_state,          // Structure in which to hold the PID controller's state.
    dt_float,              // The time that has passed since the last control cycle.
    PID_GAIN_Z_P,          // Proportional gain constant.
    PID_GAIN_Z_I,          // Integral gain constant.
    PID_GAIN_Z_D);         // Derivative gain constant.
  const float roll_factor = pid_cycle(
    desired_state->roll,
    measured_state->roll,
    &roll_pid_state,
    dt_float,
    PID_GAIN_ROLL_P,
    PID_GAIN_ROLL_I,
    PID_GAIN_ROLL_D);
  const float pitch_factor = pid_cycle(
    desired_state->pitch,
    measured_state->pitch,
    &pitch_pid_state,
    dt_float,
    PID_GAIN_PITCH_P,
    PID_GAIN_PITCH_I,
    PID_GAIN_PITCH_D);
  const float yaw_factor = pid_cycle(
    desired_state->yaw_vel,
    measured_state->yaw_vel,
    &yaw_pid_state,
    dt_float,
    PID_GAIN_YAW_P,
    PID_GAIN_YAW_I,
    PID_GAIN_YAW_D);

  // Combine contributions to produce duty cycles.
  duty_cycles->a = z_factor + roll_factor - pitch_factor + yaw_factor;
  duty_cycles->b = z_factor - roll_factor - pitch_factor - yaw_factor;
  duty_cycles->c = z_factor - roll_factor + pitch_factor + yaw_factor;
  duty_cycles->d = z_factor + roll_factor + pitch_factor - yaw_factor;
}
