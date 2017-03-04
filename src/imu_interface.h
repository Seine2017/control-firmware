// This file specifies the interface between the IMU interfacing code (written
// by Pawel) and the control code (written by Kier).

#ifndef __IMU_INTERFACE_H
#define __IMU_INTERFACE_H

#include <stdint.h>

// A structure to hold all of the data that 'read_sensors' retrieves. All fields
// are signed 16-bit numbers.
// TO BE DEFINED: what is the resolution of each of these fields?
//   i.e. if gyro_x is set to 17, how many degrees/second does that represent?
// Definition of axes in the body coordinate system:
//   x axis: points directly between rotors A and B ("forwards")
//   y axis: points in the direction of thrust ("upwards")
//   z axis: points directly between rotors B and C ("to the right")
// Angles:
//   roll: rotation about the x axis
//   yaw: rotation about the y axis
//   pitch: rotation about the z axis
typedef struct {
  int16_t accel_x; // Linear acceleration in the x direction.
  int16_t accel_y; // Linear acceleration in the y direction.
  int16_t accel_z; // Linear acceleration in the z direction.
  int16_t gyro_x; // Angular velocity around the x axis (roll velocity).
  int16_t gyro_y; // Angular velocity around the y axis (yaw velocity).
  int16_t gyro_z; // Angular velocity around the z axis (pitch velocity).
  int16_t magneto_x;
  int16_t magneto_y;
  int16_t magneto_z;
} raw_readings_t;

// Read data from the accelerometer, gyroscope and magnetometer, and store the
// obtained values into the structure pointed to by 'destination'.
void read_sensors(raw_readings_t *destination);

#endif
