// This file declares the functions and variables defined in control.c.

#ifndef __CONTROL_H
#define __CONTROL_H

#include <stdint.h>

// A structure to represent the estimated current physical state of the vehicle.
typedef struct {
  int16_t roll;  // units=radians, resolution=5e-5, max=1.63835
  int16_t pitch; // units=radians, resolution=5e-5, max=1.63835
  int16_t y_vel; // units=metres/second, resolution=2e-4, max=6.5534
} control_state_t;

// Perform a full cycle of the control algorithm, which includes reading from
// sensors, performing control calculations and updating the duty cycle of the
// PWM outputs.
void control_cycle();

#endif
