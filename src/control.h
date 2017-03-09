// This file declares the functions and variables defined in control.c.

#ifndef __CONTROL_H
#define __CONTROL_H

#include "comms_interface.h"
#include "escs_interface.h"
#include "imu_interface.h"

typedef struct {
  // Velocity along the world frame Z axis. Positive indicates upwards (away
  // from the ground). Units are metres/second.
  float z_vel;

  // Roll angle (rotation about the body frame X axis). Positive indicates
  // rolling to the right (the right-hand side of the vehicle is lower than the
  // left-hand side). Units are radians.
  float roll;

  // Pitch angle (rotation about the body frame Y axis). Positive indicates
  // pitching downwards (the front of the vehicle is lower than the rear). Units
  // are radians.
  float pitch;

  // Yaw velocity (angular velocity about the body frame Z axis). Positive
  // indicates counterclockwise rotation when the vehicle is viewed from above.
  // Units are radians/second.
  float yaw_vel;
} desired_state_t;

// Perform a full cycle of the control algorithm.
void control_cycle(measured_state_t *measured_state,
                   desired_state_t *desired_state,
                   rotor_speeds_t *rotor_speeds);

#endif
