// This file specifies the interface between the IMU interfacing code (written
// by Pawel) and the control code (written by Kier).

#ifndef __IMU_INTERFACE_H
#define __IMU_INTERFACE_H

// A structure to hold all of the data that 'read_sensors' retrieves. All fields
// are signed 16-bit numbers.
// TO BE DEFINED: what is the resolution of each of these fields?
//   i.e. if gyro_x is set to 17, how many degrees/second does that represent?
typedef struct {
  int16_t accel_x;
  int16_t accel_y;
  int16_t accel_z;
  int16_t gyro_x;
  int16_t gyro_y;
  int16_t gyro_z;
  int16_t magneto_x;
  int16_t magneto_y;
  int16_t magneto_z;
} raw_readings_t;

// Read data from the accelerometer, gyroscope and magnetometer, and store the
// obtained values into the structure pointed to by 'destination'.
void read_sensors(raw_readings_t *destination);

#endif
