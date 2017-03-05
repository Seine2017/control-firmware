// This file specifies the interface between the IMU interfacing code (written
// by Pawel) and the control code (written by Kier).

#ifndef __IMU_INTERFACE_H
#define __IMU_INTERFACE_H

// A structure to hold all of the data that 'read_sensors' retrieves.
// Definition of axes in the body coordinate system:
//   x axis: points directly between rotors A and B ("forwards")
//   y axis: points in the direction of thrust ("upwards")
//   z axis: points directly between rotors B and C ("to the right")
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
} measured_state_t;

// Read data from the accelerometer, gyroscope and possibly magnetometer, filter
// and process the data, and store the results into the structure pointed to by
// 'destination'.
void read_sensors(measured_state_t *destination);

#endif
