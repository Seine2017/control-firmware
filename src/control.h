// This file declares the functions and variables defined in control.c.

#ifndef __CONTROL_H
#define __CONTROL_H

#include <stdint.h>

// A structure to represent the estimated current physical state of the vehicle.
typedef struct {
  int16_t roll;  // units: 5e-5 radians
  int16_t pitch; // units: 5e-5 radians
  int16_t yvel;  // units: 2e-4 radians
} control_state_t;

// Perform a full cycle of the control algorithm, which includes reading from
// sensors, performing control calculations and updating the duty cycle of the
// PWM outputs.
void control_tick();

#endif
