#include <stdint.h>

#include "control.h"
#include "imu_interface.h"
#include "sat.h"

// The estimated current state of the vehicle.
control_state_t control_state;

static void control_compute_y_factor(int16_t yvel_desired, int16_t yvel_current) {
  int16_t yvel_error = sat_s16_sub(yvel_desired, yvel_current);

}

// Perform a full cycle of the control algorithm, which includes reading from
// sensors, performing control calculations and updating the duty cycle of the
// PWM outputs.
void control_tick() {
  // Read from sensors.
  raw_readings_t raw_readings;
  read_sensors(&raw_readings);

  // Process sensor data and update state.
  //TODO

  // Compute the contributions towards vertical, roll, pitch and yaw movements.
  y_factor = control_compute_y_factor(0, control_state.yvel);
  roll_factor = control_compute_roll_factor();
  pitch_factor = control_compute_pitch_factor();
  yaw_factor = control_compute_yaw_factor();

  // Combine 
}
